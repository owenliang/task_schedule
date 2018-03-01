# task_schedule

## 项目介绍

基于DAG图的调度DEMO，基于图数据结构与拓扑序列理论，实现一个任务调度类。

## 功能

当前目标是做一个命令行工具，传入一个JSON配置拓扑，然后进行调度，所有任务完成后退出。

当前没有实现JSON解析，仍旧是main函数中代码建立的拓扑图，有时间补上。

## 输入

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

## 输出

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