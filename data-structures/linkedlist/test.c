#include "linkedlist.h"
#include <stdio.h>
#include <assert.h>

int main()
{
    linkedlist_t list = create_linkedlist();
    list_node_ptr_t p = NULL;
    list = insert_node(list, NULL, 3);
    list = insert_node(list, NULL, 5);
    list = insert_node(list, NULL, 7);
    list = insert_node(list, NULL, 9);
    list = insert_node(list, NULL, 11);
    list = insert_node(list, NULL, 13);

    p = find_node(list, 9);
    list = delete_node(list, p);
    
    for (p = first_node(list); p != NULL; p = next_node(list, p))
        printf("%d ", get_element(p));
    printf("\n");
    list = clear_linkedlist(list);
    assert(is_empty(list));
    
    list = insert_node(list, NULL, 4);
    list = insert_node(list, NULL, 6);
    list = insert_node(list, list, 2);
    list = insert_node(list, NULL, 8);
    list = insert_node(list, NULL, 10);
    for (p = last_node(list); p != NULL; p = prev_node(list, p))
        printf("%d ", get_element(p));
    printf("\n");
    destroy_linkedlist(list);
}
