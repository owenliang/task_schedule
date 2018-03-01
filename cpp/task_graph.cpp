#include "task_graph.h"
#include <stack>
#include <iostream>

TaskGraph::~TaskGraph() {
    for (Graph::iterator iter = graph_.begin(); iter != graph_.end(); ++iter) {
        delete iter->second;
    }
}

// 添加任务, 声明依赖
bool TaskGraph::addTask(const std::string& taskName, const std::vector<std::string> &deps) {
    Graph::iterator iter = graph_.find(taskName);
    if (iter != graph_.end()) {
        return false;
    }

    TaskNode* node = new TaskNode(deps);
    graph_.insert(Graph::value_type(taskName, node));
    return true;
}

// 初始化拓扑
bool TaskGraph::initGraph() {
    // 逆拓扑栈
    std::stack<std::string> topStack;
    // 每个任务的出度
    std::map<std::string, int> tmpOutCounter;

    // 检查是否所有依赖任务都存在
    for (Graph::iterator iter = graph_.begin(); iter != graph_.end(); ++iter) {
        TaskNode* node = iter->second;
        for (std::map<std::string, bool>::const_iterator depIter = node->outEdge.begin(); depIter != node->outEdge.end(); ++depIter) {
            Graph::iterator destIter = graph_.find(depIter->first);
            if (destIter == graph_.end()) { // 依赖任务不存在
                return false;
            }
            // 增加依赖任务的入度
            TaskNode* dest = destIter->second;
            dest->inEdge.insert(make_pair(iter->first, false));
            ++dest->inCounter;
        }

        // 复制一份出度
        tmpOutCounter[iter->first] = node->outCounter;

        // 没有依赖任务的进入逆拓扑栈
        if (!node->outCounter) {
            topStack.push(iter->first);
        }
    }

    // 检查图中是否有环
    int topCount = 0;
    while (!topStack.empty()) {
        std::string taskName = topStack.top();
        topStack.pop();
        ++topCount;

        TaskNode* node = graph_[taskName];
        // 为每个入边的源点的出度-1
        for (std::map<std::string, bool>::iterator iter = node->inEdge.begin(); iter != node->inEdge.end(); ++iter) {
            if (--tmpOutCounter[iter->first] == 0) { // 出度减少为0, 加入栈中
                topStack.push(iter->first);
            }
        }
    }
    if (topCount != graph_.size()) { // 存在环
        return false;
    }

    // 生成初始待办任务
    for (Graph::iterator iter = graph_.begin(); iter != graph_.end(); ++iter) {
        TaskNode* node = iter->second;
        if (!node->outCounter) {
            todo_.insert(iter->first);
        }
    }
    return true;
}

// 获取所有待执行任务
void TaskGraph::getTodoTasks(std::vector<std::string> *todo) {
    for (std::set<std::string>::iterator iter = todo_.begin(); iter != todo_.end(); ++iter) {
        todo->push_back(*iter);
    }
}

// 标识任务完成
bool TaskGraph::markTaskDone(const std::string& taskName) {
    std::set<std::string>::iterator iter = todo_.find(taskName);
    if (iter == todo_.end()) {
        return false;
    }

    // 删除todo_任务
    todo_.erase(iter);

    TaskNode* node = graph_[taskName];
    node->done = true;

    // 找到入边的源点, 减少它们的出度
    for (std::map<std::string, bool>::iterator inIter = node->inEdge.begin(); inIter != node->inEdge.end(); ++inIter) {
        TaskNode* from = graph_[inIter->first];
        from->outEdge[taskName] = true; // 标记出边删除
        if (--from->outCounter == 0) { // 出度为0, 依赖完全, 进入待办
            todo_.insert(inIter->first);
        }
    }

    // 找到出边的终点, 减少它们的入度
    for (std::map<std::string, bool>::iterator outIter = node->outEdge.begin(); outIter != node->outEdge.end(); ++outIter) {
        TaskNode* dest = graph_[outIter->first];
        dest->inEdge[taskName] = true; // 标记入边删除
        --dest->inCounter;
    }

    return true;
}

// 打印所有任务信息
void TaskGraph::printGraph(std::ostream& ostream) {
    ostream << "---------------" << std::endl;
    for (Graph::iterator iter = graph_.begin(); iter != graph_.end(); ++iter) {
        TaskNode* node = iter->second;

        ostream << "任务名:" << iter->first << std::endl;
        ostream << "是否完成:" << (node->done ? "YES" : "NO") << std::endl;
        ostream << "（当前）依赖这些任务:";
        for (std::map<std::string, bool>::iterator outIter = node->outEdge.begin(); outIter != node->outEdge.end(); ++outIter) {
            if (!outIter->second) {
                ostream << " " << outIter->first << " ";
            }
        }
        ostream << std::endl;

        ostream << "（当前）被这些任务依赖:";
        for (std::map<std::string, bool>::iterator inIter = node->inEdge.begin(); inIter != node->inEdge.end(); ++inIter) {
            if (!inIter->second) {
                ostream << " " << inIter->first << " ";
            }
        }
        ostream << std::endl;
    }
    ostream << "---------------" << std::endl;
}