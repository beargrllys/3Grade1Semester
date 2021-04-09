#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

int module_start(void) {
	struct task_struct *task;
	struct task_struct *child;
	struct list_head *list;

	printk("INSTALL: parent_child");
	for_each_process(task) {								//1) init process로 시작해서 모든 프로세스를 탐색한다.
		printk("\n %4d task %s\n children: ", task->pid, task->comm);			//2) 해당 프로세스의 task 구조체 정보를 출력한다.
		list_for_each(list, &task->children) {						// 3) 해당 task의 자식 head를 가져온다.
			child = list_entry(list, struct task_struct, sibling);			// 4) 자식 head를 기준으로 형제 관계(sibling)에 있는 모든 자식들을 불러온다.
			printk(" %4d %s", child->pid, child->comm);				// 5) 자식의 정보를 출력한다.
											// 결과적으로 task의 모든 자식들이 출력된다.
		}
	}
	return 0;
}

void module_end(void) {
	printk("REMOVE: parent_child");
}

module_init(module_start);
module_exit(module_end);
