#ifndef CPP_TASK_SCHEDULE_H
#define CPP_TASK_SCHEDULE_H

#include <queue>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include <sys/types.h>
#include <unistd.h>

#include "task_schedule_interface.h"

class TaskInfo;

// 调度单元, 管理单个拓扑
class ScheduleUnit {
public:
    // 初始化调度单元
    ScheduleUnit(TaskGraph* graph);
    // 销毁调度单元
    ~ScheduleUnit();
    // 任务执行成功
    void success(const std::string& taskName);
    // 任务执行失败
    void fail(const std::string& taskName);
    // 提供资源, 返回要执行的任务
    void offer(int offerCount, std::vector<TaskInfo*>* toScheduleTasks);
    // 调度单元是否完成(全部成功或者某个任务失败)
    bool done();
    // 获取graph
    TaskGraph* getGraph() {return graph_;}

private:
    // 当todo_和doing_均为空的时候, 整个拓扑调度完成
    TaskGraph* graph_;

    std::set<std::string> all_; // all_=todo_+doing_+done_
    std::set<std::string> todo_; // 等待执行的任务
    std::set<std::string> doing_; // 正在执行的任务
    std::set<std::string> done_; // 执行完成的任务

    // 是否某个任务彻底失败
    bool finalFail_;
    // 每个任务的失败次数
    std::map<std::string, int> failTimes_;
};

// 调度器, 管理多个拓扑
class TaskSchedule: public TaskScheduleInterface {
public:
    // 初始化调度器
    TaskSchedule(int maxParallel);

    // 添加新的调度任务
    void addGraph(TaskGraph *graph);

    // 启动调度
    void run();

private:
    // 检查执行单元状态
    void checkUnitStatus(ScheduleUnit* unit);

    // 限制最大并发任务数
    int maxParallel_;
    // 剩余并发配额
    int leftOffer_;

    // 维护所有的调度单元
    std::set<ScheduleUnit*> all_units_;

    // 每个任务在1个子进程中执行, 记录上下文信息
    std::map<pid_t, std::pair<std::string, ScheduleUnit*> > scheduleInfo_;
};


#endif //CPP_TASK_SCHEDULE_H
