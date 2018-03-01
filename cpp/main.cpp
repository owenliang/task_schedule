#include <iostream>
#include <vector>
#include "task_graph.h"
#include "task_schedule.h"

int main() {
    // 初始化调度器，任务并行度3
    TaskSchedule scheduler(3);

    // 第一个拓扑
    {

        /*
         *  拓扑1:
         *
         *             J O B 1
         *          /    \       \
         *        V      V      V
         *     JOB2  JOB3  JOB5
         *         \     /
         *         V   V
         *         JOB4
         */
        TaskGraph *graph = new TaskGraph();

        // JOB5: 没有依赖
        std::vector<std::string> job5Deps;
        graph->addTask("graph1-job5", job5Deps, "sleep 2");

        // JOB4: 没有依赖
        std::vector<std::string> job4Deps;
        graph->addTask("graph1-job4", job4Deps, "sleep 2");

        // JOB3: 依赖JOB4
        std::vector<std::string> job3Deps;
        job3Deps.push_back("graph1-job4");
        graph->addTask("graph1-job3", job3Deps, "sleep 2");

        // JOB2: 依赖JOB4
        std::vector<std::string> job2Deps;
        job2Deps.push_back("graph1-job4");
        graph->addTask("graph1-job2", job2Deps, "sleep 2");

        // JOB1: 依赖JOB2和JOB3和JOB5
        std::vector<std::string> job1Deps;
        job1Deps.push_back("graph1-job2");
        job1Deps.push_back("graph1-job3");
        job1Deps.push_back("graph1-job5");
        graph->addTask("graph1-job1", job1Deps, "sleep 2");

        // 初始化拓扑
        if (!graph->initGraph()) {
            std::cerr << "initGraph fail" << std::endl;
            return -1;
        }

        // 注册拓扑
        scheduler.addGraph(graph);
    }

    // 第二个拓扑
    {
        /**
         *      J  O  B  1
         *     /    |      \
         *    V    V      V
         *  JOB2 JOB3 JOB4  <- JOB5
         *
         */

        TaskGraph *graph = new TaskGraph();

        // JOB5: 依赖JOB4
        std::vector<std::string> job5Deps;
        job5Deps.push_back("graph2-job4");
        graph->addTask("graph2-job5", job5Deps, "sleep 2");

        // JOB4: 没有依赖
        std::vector<std::string> job4Deps;
        graph->addTask("graph2-job4", job4Deps, "sleep 2");

        // JOB3: 没有依赖
        std::vector<std::string> job3Deps;
        graph->addTask("graph2-job3", job3Deps, "sleep 2");

        // JOB2: 没有依赖
        std::vector<std::string> job2Deps;
        graph->addTask("graph2-job2", job2Deps, "sleep 2");

        // JOB1: 依赖JOB2,JOB3,JOB4
        std::vector<std::string> job1Deps;
        job1Deps.push_back("graph2-job2");
        job1Deps.push_back("graph2-job3");
        job1Deps.push_back("graph2-job4");
        graph->addTask("graph2-job1", job1Deps, "sleep 2");

        // 初始化拓扑
        if (!graph->initGraph()) {
            std::cerr << "initGraph fail" << std::endl;
            return -1;
        }

        // 注册拓扑
        scheduler.addGraph(graph);
    }

    // 执行调度
    scheduler.run();

    return 0;
}