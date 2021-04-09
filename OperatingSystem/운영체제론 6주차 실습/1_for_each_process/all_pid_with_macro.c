#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

int module_start(void) {
	struct task_struct *task;

	printk(KERN_INFO "Init Module....");
	for_each_process(task) {
		printk("%s[%d]\n", task->comm, task->pid);
	}
	return 0;
}

void module_end(void) {
	printk("Module removing...");
}

module_init(module_start);
module_exit(module_end);
