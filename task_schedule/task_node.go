package task_schedule

type TaskNode struct {
	OutEdge    map[string]bool // 本任务依赖的其他任务、完成状况
	InEdge     map[string]bool // 依赖了本任务的其他任务、完成状况
	OutCounter int             //剩余依赖任务
	InCounter  int             // 剩余被依赖任务
	Done       bool            // 任务是否完成
}

func NewNode(deps []string) *TaskNode {
	taskNode := new(TaskNode)
	taskNode.OutCounter = 0
	taskNode.InCounter = 0
	taskNode.Done = false
	taskNode.OutEdge = make(map[string]bool)
	taskNode.InEdge = make(map[string]bool)
	for _, dep := range deps {
		taskNode.OutEdge[dep] = false
		taskNode.OutCounter += 1
	}
	return taskNode
}
