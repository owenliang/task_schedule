#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include "task_graph.h"
#include "task_schedule.h"

// 初始化调度单元
ScheduleUnit::ScheduleUnit(TaskGraph* graph) : graph_(graph) {
    std::vector<std::string> todoTasks;

    graph_->getTodoTasks(&todoTasks);
    for (size_t i = 0; i < todoTasks.size(); ++i) {
        all_.insert(todoTasks[i]);
        todo_.insert(todoTasks[i]);
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

    std::vector<std::string> todoTasks;
    graph_->getTodoTasks(&todoTasks);
    for (size_t i = 0; i < todoTasks.size(); ++i) {
        if (!all_.count(todoTasks[i])) {
            all_.insert(todoTasks[i]);
            todo_.insert(todoTasks[i]);
        }
    }
}

// 任务执行失败
void ScheduleUnit::fail(const std::string& taskName) {
    doing_.erase(taskName);
    todo_.insert(taskName);
}

// 提供资源, 返回要执行的任务
void ScheduleUnit::offer(int offerCount, std::vector<std::string>* toScheduleTasks) {
    for (int i = 0; i < offerCount; ++i) {
        if (todo_.begin() == todo_.end()) {
            break;
        }
        std::string taskName = *todo_.begin();
        toScheduleTasks->push_back(taskName);
        todo_.erase(taskName);
        doing_.insert(taskName);
    }
}

// 调度单元是否完成
bool ScheduleUnit::done() {
    return todo_.empty() && doing_.empty();
}

// 初始化调度器
TaskSchedule::TaskSchedule(int maxParallel) : maxParallel_(maxParallel), leftOffer_(maxParallel) {
    pthread_mutex_init(&queueMutex_, NULL);
}

// 添加新的调度任务
void TaskSchedule::addGraph(TaskGraph *graph) {
    pthread_mutex_lock(&queueMutex_);
    newGraph_.push(graph);
    pthread_mutex_unlock(&queueMutex_);
}

// 启动调度
//
// 为了简化采用了轮询形式, 建议生产环境采用epoll+pipe监听增加拓扑和任务执行完成的事件, 避免while+sleep的调度形式
//
void TaskSchedule::run() {
    std::queue<TaskGraph*> newGraph;

    do {
        // 1, 获取新调度任务
        pthread_mutex_lock(&queueMutex_);
        newGraph.swap(newGraph_);
        pthread_mutex_unlock(&queueMutex_);

        while (!newGraph.empty()) {
            TaskGraph* graph = newGraph.front();
            newGraph.pop();
            all_units_.insert(new ScheduleUnit(graph));
            std::cout << "新增单元,信息如下:" << std::endl;
            graph->printGraph(std::cout);
        }

        // 2, 检测结束的任务与单元
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
                    if (unit->done()) {
                        std::cout << "单元完成,信息如下:" << std::endl;
                        unit->getGraph()->printGraph(std::cout);
                        all_units_.erase(unit);
                        delete unit;
                    }
                }
            } else if (pid == 0 || errno == ECHILD) { // 当前没有其他子进程退出, 或者没有子进程了
                break;
            }
        } while(true);

        // 3, 配额offer给调度单元
        std::vector<std::string> toSchedule;
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

            std::cout << "准备新建并行任务: " << toSchedule.size() << "个" << std::endl;

            // 尝试启动每个任务, 多进程并行处理
            for (size_t i = 0; i < toSchedule.size(); ++i) {
                pid_t pid = fork();
                if (pid == -1) {
                    leftOffer_++;
                    unit->fail(toSchedule[i]);
                } else if (pid > 0) { // 父进程
                    scheduleInfo_.insert(make_pair(pid, make_pair(toSchedule[i], unit)));
                } else { // 子进程
                    if (execlp("echo", "echo", "执行任务:", toSchedule[i].c_str(), NULL) == -1) { // 调起失败,exit(-1)
                        exit(-1);
                    }
                }
            }
        }

        // 4, 休眠10毫秒
        usleep(10 * 1000);
    } while (true);
}
