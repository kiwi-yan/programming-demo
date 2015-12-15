#ifndef _KIWI_LINKEDLIST_H
#define _KIWI_LINKEDLIST_H

/** 链表数据类型声明 **/

typedef int     data_t;

struct list_node_s;
typedef struct list_node_s  list_node_t;
typedef list_node_t *   list_node_ptr_t;
typedef list_node_t *   linkedlist_t;

/** 链表类型基本操作声明 **/
linkedlist_t create_linkedlist();           /* 创建链表 */
void destroy_linkedlist(linkedlist_t list); /* 删除链表 */
linkedlist_t clear_linkedlist(linkedlist_t list);   /* 清空链表 */
int is_empty(linkedlist_t list);            /* 判断链表是否为空 */

list_node_ptr_t next_node(linkedlist_t list, list_node_ptr_t ptr); /* 后继结点 */
list_node_ptr_t prev_node(linkedlist_t list, list_node_ptr_t ptr); /* 前驱结点 */
list_node_ptr_t first_node(linkedlist_t list);      /* 头结点 */
list_node_ptr_t last_node(linkedlist_t list);       /* 尾结点 */
data_t get_element(list_node_ptr_t ptr);    /* 获取结点元素 */

linkedlist_t insert_node(linkedlist_t list, list_node_ptr_t ptr, data_t data); /* 在指定结点处插入元素 */
linkedlist_t delete_node(linkedlist_t list, list_node_ptr_t ptr);   /* 删除指定结点 */
list_node_ptr_t find_node(list_node_ptr_t ptr, data_t data);        /* 从指定位置起查找结点 */

#endif /* _KIWI_LINKEDLIST_H */
