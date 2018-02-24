package main

import (
	"github.com/owenliang/task_schedule/task_schedule"
	"fmt"
)

func main() {
	var taskGraph task_schedule.TaskGraph
	taskGraph = taskGraph.New()

	job5Deps := [] string{}
	taskGraph.AddTask("job5", job5Deps)

	job4Deps := [] string{}
	taskGraph.AddTask("job4", job4Deps)

	job3Deps := [] string{"job4"}
	taskGraph.AddTask("job3", job3Deps)

	job2Deps := [] string{"job4"}
	taskGraph.AddTask("job2", job2Deps)

	job1Deps := [] string{"job2", "job3", "job5"}
	taskGraph.AddTask("job1", job1Deps)

	if !taskGraph.InitGraph() {
		fmt.Print("error")
		return
	}

	taskGraph.PrintGraph()

	for true {
		todo := taskGraph.GetTodoTasks()

		if len(todo) <= 0 {
			break
		}

		fmt.Print("<<<<<<<<< 可执行待办任务列表: ")
		for i := 0; i < len(todo); i++ {
			fmt.Print(" ", todo[i], " ")
		}
		fmt.Println()

		for i := 0; i < len(todo); i++ {
			fmt.Println(" ", todo[i], " ")
			if !taskGraph.MarkTaskDone(todo[i]) {
				fmt.Println(" markTaskDone fail")
				return
			}
			taskGraph.PrintGraph()
		}
	}
}
