#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "json/json.h"
#include "task_graph.h"
#include "task_schedule.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "scheduler ./example.json" << std::endl;
        return -1;
    }
    Json::Value jsonRoot;
    Json::Reader jsonReader;

    std::ifstream ifs(argv[1]);
    if (!ifs) {
        return -1;
    }
    if (!jsonReader.parse(ifs, jsonRoot)) {
        return -1;
    }

    if (!jsonRoot.isMember("graph") || !jsonRoot["graph"].isObject()) {
        return -1;
    }

    if (!jsonRoot.isMember("maxParallel") || !jsonRoot["maxParallel"].isInt()) {
        return -1;
    }

    TaskSchedule scheduler(jsonRoot["maxParallel"].asInt());

    // 遍历所有拓扑图
    Json::Value::Members members = jsonRoot["graph"].getMemberNames();
    for (Json::Value::Members::iterator iter = members.begin(); iter != members.end(); ++iter) {
        TaskGraph *graph = new TaskGraph();

        do {
            std::string graphName = *iter;

            Json::Value jsonGraph = jsonRoot["graph"][graphName];

            Json::Value jsonTask = jsonGraph.get("task", Json::Value(Json::arrayValue));
            if (!jsonTask.isArray()) {
                break;
            }

            // 遍历拓扑内所有任务
            Json::ArrayIndex i = 0;
            for (; i < jsonTask.size(); ++i) {
                Json::Value jsonTaskElem = jsonTask.get(i, Json::Value(Json::nullValue));
                if (!jsonTaskElem.isObject()) {
                    break;
                }

                Json::Value jsonTaskName = jsonTaskElem.get("name", Json::Value(Json::nullValue));
                Json::Value jsonTaskCmd = jsonTaskElem.get("cmd", Json::Value(Json::nullValue));
                Json::Value jsonTaskRetry = jsonTaskElem.get("retry", Json::Value(Json::intValue));
                Json::Value jsonTaskDeps = jsonTaskElem.get("deps", Json::Value(Json::arrayValue));
                if (!jsonTaskName.isString() || !jsonTaskCmd.isString() || !jsonTaskRetry.isInt() || !jsonTaskDeps.isArray()) {
                    break;
                }

                Json::ArrayIndex j = 0;
                std::vector<std::string> deps;
                for (; j != jsonTaskDeps.size(); ++j) {
                    if (!jsonTaskDeps[j].isString()) {
                        break;
                    }
                    deps.push_back(jsonTaskDeps[j].asString());
                }
                if (j != jsonTaskDeps.size()) {
                    break;
                }
                if (!graph->addTask(jsonTaskName.asString(), deps, jsonTaskCmd.asString(), jsonTaskRetry.asInt())) {
                    break;
                }
            }

            // 注册拓扑
            if (i == jsonTask.size() && graph->initGraph()) {
                scheduler.addGraph(graph);
            } else {
                delete graph;
                return -1;
            }
        } while (0);
    }
    scheduler.run();

    return 0;
}