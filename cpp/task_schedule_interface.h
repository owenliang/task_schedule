#ifndef CPP_TASK_SCHEDULE_INTERFACE_H
#define CPP_TASK_SCHEDULE_INTERFACE_H

class TaskGraph;

class TaskScheduleInterface {
public:
    // 添加新的调度任务
    virtual void addGraph(TaskGraph*) = 0;

    // 启动调度
    virtual void run() = 0;
};

#endif //CPP_TASK_SCHEDULE_INTERFACE_H
