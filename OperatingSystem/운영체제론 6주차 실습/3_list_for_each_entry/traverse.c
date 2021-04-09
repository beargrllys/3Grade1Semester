#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/slab.h>

/* ISO C90 경고는 변수 선언의 순서와 관계된 것이므로 본 코드에서는 무시해도 무방하다. */

struct my_struct {
  int data;
  struct list_head list;
};

LIST_HEAD(my_list);

struct my_struct *createNode(int _data) { 	
  struct my_struct *n; 	
  n = kmalloc(sizeof(struct my_struct), GFP_KERNEL); 	 
  n->data = _data;
  return n; 				
}

int example_init(void) {
  printk("INSTALL MODULE: traverse\n");
  struct my_struct *node_pointer = createNode(1);
  list_add_tail(&node_pointer->list, &my_list);		
  node_pointer = createNode(2); 			
  list_add_tail(&node_pointer->list, &my_list);		
  node_pointer = createNode(3);
  list_add_tail(&node_pointer->list, &my_list);
	
  /* 본 코드의 핵심 함수 */
  struct my_struct *cursor;
  list_for_each_entry(cursor, &my_list, list){		// loop를 사용하지 않았는데도 반복적으로 노드들을 방문한다.
    printk("NODE DATA: %d\n", cursor->data);
  }
  return 0;
}

void example_exit(void) {
  return;
}

module_init(example_init);
module_exit(example_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("traverse each node");
MODULE_AUTHOR("OS2019");
