#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

struct list_node_s {
    data_t data;
    struct list_node_s *next;
};

linkedlist_t
create_linkedlist()
{
    return NULL;
}

void
destroy_linkedlist(linkedlist_t list)
{
    clear_linkedlist(list);
}

linkedlist_t
clear_linkedlist(linkedlist_t list)
{
    list_node_ptr_t p = NULL;
    while (list != NULL) {
        p = list;
        list = p->next;
        free(p);
    }
    return list;
}

list_node_ptr_t
next_node(linkedlist_t list, list_node_ptr_t ptr)
{
    if (ptr != NULL)
        return ptr->next;
    return NULL;
}

list_node_ptr_t
prev_node(linkedlist_t list, list_node_ptr_t ptr)
{
    list_node_ptr_t p = list;
    if (ptr == NULL)
        return last_node(list);
    if (ptr == list)
        return NULL;
    while (p->next != NULL && p->next != ptr)
        p = p->next;
    if (p->next != NULL)
        return p;
    return NULL;
}

list_node_ptr_t
first_node(linkedlist_t list)
{
    return list;
}

list_node_ptr_t
last_node(linkedlist_t list)
{
    list_node_ptr_t p = list;
    if (list != NULL) {
        while (p->next != NULL) {
            p = p->next;
        }
        return p;
    }
    return NULL;
}

data_t
get_element(list_node_ptr_t ptr)
{
    return ptr->data;
}

linkedlist_t
insert_node(linkedlist_t list, list_node_ptr_t ptr, data_t data)
{
    list_node_ptr_t p = prev_node(list, ptr);
    list_node_ptr_t tmp = (list_node_ptr_t)malloc(sizeof(list_node_t));
    if (tmp == NULL) {
        perror("malloc");
        exit(0);
    }
    tmp->data = data;
    
    if (p == NULL) {
        tmp->next = list;
        list = tmp;
    }
    else {
        tmp->next = p->next;
        p->next = tmp;
    }
    return list;
}

linkedlist_t
delete_node(linkedlist_t list, list_node_ptr_t ptr)
{
    list_node_ptr_t p = prev_node(list, ptr);
    if (ptr == NULL)
        return;
    if (p == NULL)
        list = ptr->next;
    else
        p->next = ptr->next;
    
    free(ptr);
    return list;
}

int
is_empty(linkedlist_t list)
{
    return list == NULL;
}

list_node_ptr_t
find_node(list_node_ptr_t ptr, data_t data)
{
    list_node_ptr_t p = ptr;
    while (p != NULL && p->data != data)
        p = p->next;
    return p;
}
