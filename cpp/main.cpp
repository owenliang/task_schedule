#include <iostream>
#include <vector>
#include "task_graph.h"
#include "task_schedule.h"

int main() {
    TaskGraph* graph = new TaskGraph();

    /*
     *
     *             J O B 1
     *          /    \       \
     *        V      V      V
     *     JOB2  JOB3  JOB5
     *         \     /
     *         V   V
     *         JOB4
     */

    // JOB5: 没有依赖
    std::vector<std::string> job5Deps;
    graph->addTask("job5", job5Deps);

    // JOB4: 没有依赖
    std::vector<std::string> job4Deps;
    graph->addTask("job4", job4Deps);

    // JOB3: 依赖JOB4
    std::vector<std::string> job3Deps;
    job3Deps.push_back("job4");
    graph->addTask("job3", job3Deps);

    // JOB2: 依赖JOB4
    std::vector<std::string> job2Deps;
    job2Deps.push_back("job4");
    graph->addTask("job2", job2Deps);

    // JOB1: 依赖JOB2和JOB3和JOB5
    std::vector<std::string> job1Deps;
    job1Deps.push_back("job2");
    job1Deps.push_back("job3");
    job1Deps.push_back("job5");
    graph->addTask("job1", job1Deps);

    // 初始化拓扑
    if (!graph->initGraph()) {
        std::cerr << "initGraph fail" << std::endl;
        return -1;
    }

    // 初始化调度
    TaskSchedule scheduler(2);
    scheduler.addGraph(graph);
    scheduler.run();

    return 0;
}