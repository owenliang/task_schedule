#include <iostream>
#include <vector>
#include "task_graph.h"

int main() {
    TaskGraph graph;

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
    graph.addTask("job5", job5Deps);

    // JOB4: 没有依赖
    std::vector<std::string> job4Deps;
    graph.addTask("job4", job4Deps);

    // JOB3: 依赖JOB4
    std::vector<std::string> job3Deps;
    job3Deps.push_back("job4");
    graph.addTask("job3", job3Deps);

    // JOB2: 依赖JOB4
    std::vector<std::string> job2Deps;
    job2Deps.push_back("job4");
    graph.addTask("job2", job2Deps);

    // JOB1: 依赖JOB2和JOB3和JOB5
    std::vector<std::string> job1Deps;
    job1Deps.push_back("job2");
    job1Deps.push_back("job3");
    job1Deps.push_back("job5");
    graph.addTask("job1", job1Deps);

    // 初始化调度
    if (!graph.initGraph()) {
        std::cerr << "initGraph fail" << std::endl;
        return -1;
    }

    // 打印任务调度图
    graph.printGraph(std::cout);

    // 串行执行这些任务
    std::vector<std::string> todo;
    do {
        todo.clear();

        // 获取当前待办任务
        graph.getTodoTasks(&todo);
        if (!todo.size()) { // 没有待办任务, 结束调度
            break;
        }

        // 输出可以并行处理的待办任务
        std::cout << "<<<<<<<<< 可执行待办任务列表: ";
        for (size_t i = 0; i != todo.size(); ++i) {
            std::cout << " " << todo[i] << " ";
        }
        std::cout << std::endl;

        // 串行执行待办任务(可以通过程序并行调度, 不做演示)
        for (size_t i = 0; i != todo.size(); ++i) {
            // 模拟任务执行
            std::cout << ">>>>>>>>>" <<  todo[i] << " 被执行" << std::endl;
            // 标记任务完成
            if (!graph.markTaskDone(todo[i])) {
                std::cerr << "markTaskDone fail" << std::endl;
                return -1;
            }
            // 再次打印任务调度图
            graph.printGraph(std::cout);
        }
    } while (true);

    return 0;
}