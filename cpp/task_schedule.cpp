#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>
#include "task_graph.h"
#include "task_schedule.h"

// 初始化调度单元
ScheduleUnit::ScheduleUnit(TaskGraph* graph) : graph_(graph), finalFail_(false) {
    std::vector<TaskInfo*> todoTasks;

    graph_->getTodoTasks(&todoTasks);
    for (size_t i = 0; i < todoTasks.size(); ++i) {
        all_.insert(todoTasks[i]->taskName);
        todo_.insert(todoTasks[i]->taskName);
    }
}

// 销毁调度单元
ScheduleUnit::~ScheduleUnit() {
    delete graph_;
}

// 任务执行成功
void ScheduleUnit::success(const std::string& taskName) {
    doing_.erase(taskName);
    done_.insert(taskName);

    graph_->markTaskDone(taskName);

    std::vector<TaskInfo*> todoTasks;
    graph_->getTodoTasks(&todoTasks);
    for (size_t i = 0; i < todoTasks.size(); ++i) {
        if (!all_.count(todoTasks[i]->taskName)) {
            all_.insert(todoTasks[i]->taskName);
            todo_.insert(todoTasks[i]->taskName);
        }
    }
}

// 任务执行失败
void ScheduleUnit::fail(const std::string& taskName) {
    doing_.erase(taskName);
    todo_.insert(taskName);

    int failTimes;

    if (!failTimes_.count(taskName)) {
        failTimes = failTimes_[taskName] = 0;
    } else {
        failTimes = failTimes_[taskName]++;
    }

    TaskInfo* info = graph_->getTaskInfo(taskName);
    if (failTimes >= info->maxRetry) { // 超过任务指定失败次数
        finalFail_ = true;
    }
}

// 提供资源, 返回要执行的任务
void ScheduleUnit::offer(int offerCount, std::vector<TaskInfo*>* toScheduleTasks) {
    if (finalFail_) { // 单元失败, 不再继续调度更多任务
        return;
    }
    for (int i = 0; i < offerCount; ++i) {
        if (todo_.begin() == todo_.end()) {
            break;
        }
        std::string taskName = *todo_.begin();
        toScheduleTasks->push_back(graph_->getTaskInfo(taskName));
        todo_.erase(taskName);
        doing_.insert(taskName);
    }
}

// 调度单元是否完成
bool ScheduleUnit::done() {
    // 全部调度成功
    if (todo_.empty() && doing_.empty()) {
        return true;
    }
    // 调度失败超过次数, 没有其他未完成的任务, 立即结束调度
    if (doing_.empty() && finalFail_) {
        return true;
    }
    return false;
}

// 初始化调度器
TaskSchedule::TaskSchedule(int maxParallel) : maxParallel_(maxParallel), leftOffer_(maxParallel) {
}

// 添加新的调度任务
void TaskSchedule::addGraph(TaskGraph *graph) {
    all_units_.insert(new ScheduleUnit(graph));
    graph->printGraph(std::cout);
}

// 检查执行单元状态
void TaskSchedule::checkUnitStatus(ScheduleUnit* unit) {
    // DEBUG
    unit->getGraph()->printGraph(std::cout);

    if (unit->done()) {
        all_units_.erase(unit);
        delete unit;
    }
}

// 启动调度
//
// 为了简化采用了轮询形式, 建议生产环境采用epoll+pipe监听增加拓扑和任务执行完成的事件, 避免while+sleep的调度形式
//
void TaskSchedule::run() {
    std::queue<TaskGraph*> newGraph;

    do {
        // 1, 检测结束的任务与单元
        do {
            int status = 0;
            pid_t pid = waitpid(-1, &status, WNOHANG);
            if (pid > 0) {
                if (WIFEXITED(status)) {
                    bool success = !WEXITSTATUS(status);
                    std::pair<std::string, ScheduleUnit*>& info = scheduleInfo_[pid];
                    if (success) {
                        info.second->success(info.first);
                    } else {
                        info.second->fail(info.first);
                    }
                    leftOffer_++;
                    ScheduleUnit* unit = info.second;
                    scheduleInfo_.erase(pid);
                    checkUnitStatus(unit);
                }
            } else if (pid == 0 || errno == ECHILD) { // 当前没有其他子进程退出, 或者没有子进程了
                break;
            }
        } while(true);

        // 2, 配额offer给调度单元
        std::vector<TaskInfo*> toSchedule;
        for (std::set<ScheduleUnit*>::iterator iter = all_units_.begin(); iter != all_units_.end(); ++iter) {
            if (!leftOffer_) { // 没有配额
                break;
            }

            ScheduleUnit* unit = *iter;
            toSchedule.clear();
            unit->offer(leftOffer_, &toSchedule);
            if (toSchedule.size()) {
                leftOffer_ -= toSchedule.size();
            }

            // 尝试启动每个任务, 多进程并行处理
            for (size_t i = 0; i < toSchedule.size(); ++i) {
                pid_t pid = fork();
                if (pid == -1) {
                    leftOffer_++;
                    unit->fail(toSchedule[i]->taskName);
                    checkUnitStatus(unit);
                } else if (pid > 0) { // 父进程
                    scheduleInfo_.insert(make_pair(pid, make_pair(toSchedule[i]->taskName, unit)));
                } else { // 子进程
                    int nullFd = open("/dev/null", O_APPEND);
                    if (nullFd != -1) {
                        dup2(nullFd, STDOUT_FILENO);
                        dup2(nullFd, STDERR_FILENO);
                        close(nullFd);
                    }
                    if (execlp("/bin/bash", "bash", "-c", toSchedule[i]->shellCmd.c_str(), NULL) == -1) { // 调起失败,exit(-1)
                        exit(-1);
                    }
                }
            }
        }

        // 3, 所有单元结束，退出
        if (all_units_.empty()) {
            break;
        }
    } while (true);
}