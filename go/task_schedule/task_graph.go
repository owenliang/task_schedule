package task_schedule

import (
	"github.com/deckarep/golang-set"
	"container/list"
	"fmt"
)

type TaskGraph struct {
	graph map[string]*TaskNode
	todo  mapset.Set
}

func (taskGraph TaskGraph) New() TaskGraph {
	if taskGraph.graph == nil {
		taskGraph.graph = make(map[string]*TaskNode)
	}
	if taskGraph.todo == nil {
		taskGraph.todo = mapset.NewSet()
	}
	return taskGraph
}

func (taskGraph *TaskGraph) AddTask(taskName string, deps []string) bool {
	if _, v := taskGraph.graph[taskName]; v {
		return false
	}
	taskNode := NewNode(deps)
	taskGraph.graph[taskName] = taskNode
	return true
}

func (taskGraph *TaskGraph) InitGraph() bool {
	graph := taskGraph.graph
	topStack := list.New()
	tmpOutCounter := make(map[string]int)

	for taskName, node := range graph {
		for depIter := range node.OutEdge {
			destIter := graph[depIter]
			if destIter == nil {
				return false
			}
			destIter.InEdge[taskName] = false
			destIter.InCounter += 1
		}
		tmpOutCounter[taskName] = node.OutCounter

		if node.OutCounter == 0 {
			topStack.PushBack(taskName)
		}
	}

	topCount := 0
	for ; topStack.Len() > 0; {
		topCount++

		item := topStack.Front()
		topStack.Remove(item)
		taskName := item.Value.(string)

		node := graph[taskName]

		for iter := range node.InEdge {
			tmpOutCounter[iter] -= 1
			if tmpOutCounter[iter] == 0 {
				topStack.PushBack(iter)
			}
		}
	}

	if topCount != len(graph) {
		return false
	}

	for iter, node := range graph {
		if node.OutCounter == 0 {
			taskGraph.todo.Add(iter)
		}
	}

	return true
}

func (taskGraph *TaskGraph) GetTodoTasks() [] string {
	var todo []string
	for taskName := range taskGraph.todo.Iter() {
		todo = append(todo, taskName.(string))
	}
	return todo
}

func (taskGraph *TaskGraph) MarkTaskDone(taskName string) bool {
	if !taskGraph.todo.Contains(taskName) {
		return false
	}

	taskGraph.todo.Remove(taskName)

	node := taskGraph.graph[taskName]

	node.Done = true

	for k := range node.InEdge {
		from := taskGraph.graph[k]
		from.OutEdge[taskName] = true
		from.OutCounter -= 1
		if from.OutCounter == 0 { // 出度为0, 依赖完全, 进入待办
			taskGraph.todo.Add(k)
		}
	}

	for k := range node.OutEdge {
		dest := taskGraph.graph[k]
		dest.InEdge[taskName] = true
		dest.InCounter -= 1
	}

	return true
}

func (taskGraph *TaskGraph) PrintGraph() {
	fmt.Println("-----------------------------------")
	for k, node := range taskGraph.graph {
		fmt.Println("任务名：", k)
		if node.Done {
			fmt.Println("是否完成：", "YES")
		} else {
			fmt.Println("是否完成：", "NO")
		}
		fmt.Println("（当前）依赖这些任务：")

		for taskName, v := range node.OutEdge {
			if !v {
				fmt.Print(" ", taskName, " ")
			}
		}
		fmt.Println()

		fmt.Println("（当前）被这些任务依赖：")
		for taskName, v := range node.InEdge {
			if !v {
				fmt.Print(" ", taskName, " ")
			}
		}
		fmt.Println()
	}
	fmt.Println("-----------------------------------")
}
