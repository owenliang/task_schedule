# task_schedule

## 项目介绍

基于DAG图的调度DEMO，基于图数据结构与拓扑序列理论，实现一个任务调度工具。

## 功能

当前是一个命令行工具，通过JSON文件配置任务依赖关系，即可由工具完成调度。

现阶段只是一个DEMO, 有时间会把调度结果作为JSON输出, 并且利用工具的进程退出码作为调度成功与否的标识, 方便使用.

## 配置

```

{
  "maxParallel": 5,
  "graph": {
    "myGraph": {
      "task": [
        {
          "name": "createFile",
          "cmd": "touch /tmp/my_file",
          "retry": 2
        },
        {
          "name": "appendFile",
          "cmd": "echo hello >> /tmp/my_file",
          "deps": ["createFile"]
        }
      ]
    }
  }
}

```

## 运行与日志
```

./scheduler ./example.json

---------------
任务名:appendFile
SHELL命令:echo hello >> /tmp/my_file
最大重试次数0
是否完成:NO
（当前）依赖这些任务: createFile
（当前）被这些任务依赖:
任务名:createFile
SHELL命令:touch /tmp/my_file
最大重试次数2
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: appendFile
---------------
---------------
任务名:appendFile
SHELL命令:echo hello >> /tmp/my_file
最大重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:createFile
SHELL命令:touch /tmp/my_file
最大重试次数2
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: appendFile
---------------
---------------
任务名:appendFile
SHELL命令:echo hello >> /tmp/my_file
最大重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:createFile
SHELL命令:touch /tmp/my_file
最大重试次数2
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
---------------

```

## 

## 原理展示 

### 输入

```
    /*
     *  拓扑1
     *
     *          J  O  B 1
     *          /   \   \
     *        V      V    V
     *     JOB2    JOB3   JOB5
     *         \    /
     *         V   V
     *         JOB4
     */
     
     /** 
      * 拓扑2 
      *
      *      J  O  B  1
      *     /    |     \
      *    V    V      V
      *  JOB2 JOB3   JOB4  <- JOB5
      *
      */
```

### 输出

```

---------------
任务名:graph1-job1
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph1-job2  graph1-job3  graph1-job5 
（当前）被这些任务依赖:
任务名:graph1-job2
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph1-job4 
（当前）被这些任务依赖: graph1-job1 
任务名:graph1-job3
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph1-job4 
（当前）被这些任务依赖: graph1-job1 
任务名:graph1-job4
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job2  graph1-job3 
任务名:graph1-job5
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
---------------
---------------
任务名:graph2-job1
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph2-job2  graph2-job3  graph2-job4 
（当前）被这些任务依赖:
任务名:graph2-job2
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job3
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job4
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1  graph2-job5 
任务名:graph2-job5
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph2-job4 
（当前）被这些任务依赖:
---------------
---------------
任务名:graph2-job1
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph2-job3  graph2-job4 
（当前）被这些任务依赖:
任务名:graph2-job2
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job3
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job4
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1  graph2-job5 
任务名:graph2-job5
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph2-job4 
（当前）被这些任务依赖:
---------------
---------------
任务名:graph1-job1
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph1-job2  graph1-job3 
（当前）被这些任务依赖:
任务名:graph1-job2
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph1-job4 
（当前）被这些任务依赖: graph1-job1 
任务名:graph1-job3
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph1-job4 
（当前）被这些任务依赖: graph1-job1 
任务名:graph1-job4
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job2  graph1-job3 
任务名:graph1-job5
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
---------------
---------------
任务名:graph1-job1
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph1-job2  graph1-job3 
（当前）被这些任务依赖:
任务名:graph1-job2
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
任务名:graph1-job3
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
任务名:graph1-job4
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job2  graph1-job3 
任务名:graph1-job5
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
---------------
---------------
任务名:graph1-job1
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph1-job2 
（当前）被这些任务依赖:
任务名:graph1-job2
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
任务名:graph1-job3
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
任务名:graph1-job4
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job2 
任务名:graph1-job5
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
---------------
---------------
任务名:graph1-job1
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph1-job2
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
任务名:graph1-job3
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
任务名:graph1-job4
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph1-job5
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph1-job1 
---------------
---------------
任务名:graph2-job1
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph2-job4 
（当前）被这些任务依赖:
任务名:graph2-job2
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job3
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job4
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1  graph2-job5 
任务名:graph2-job5
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务: graph2-job4 
（当前）被这些任务依赖:
---------------
---------------
任务名:graph1-job1
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph1-job2
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph1-job3
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph1-job4
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph1-job5
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
---------------
---------------
任务名:graph2-job1
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph2-job2
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job3
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job4
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1  graph2-job5 
任务名:graph2-job5
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖:
---------------
---------------
任务名:graph2-job1
命令:sleep 2
重试次数0
是否完成:NO
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph2-job2
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job3
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job4
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖: graph2-job1 
任务名:graph2-job5
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
---------------
---------------
任务名:graph2-job1
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph2-job2
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph2-job3
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph2-job4
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
任务名:graph2-job5
命令:sleep 2
重试次数0
是否完成:YES
（当前）依赖这些任务:
（当前）被这些任务依赖:
---------------

```