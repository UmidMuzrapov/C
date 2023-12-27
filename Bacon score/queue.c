#include "queue.h"

/**
 * Function add_last()
 *
 * Adds to the end of the queue
 *
 * @param head
 * @param tail
 * @param val_p
 */
void add_last(queue_node** head, queue_node** tail, void* val_p)
{
    if (*head==NULL)
    {
        queue_node* new_node = malloc(sizeof(queue_node));
        memory_check(new_node);
        new_node->val_p = val_p;
        new_node->next = NULL;
        new_node->previous = NULL;
        *head = new_node;
        *tail = new_node;
    }

    else
    {
        queue_node* new_node = malloc(sizeof(queue_node));
        memory_check(new_node);
        new_node->val_p = val_p;
        new_node->next = NULL;
        new_node->previous = *tail;
        (*tail)->next = new_node;
        (*tail) = new_node;
    }
}

/**
 * Function remove_first
 *
 * The function removed the first element
 * @param head
 * @param tail
 * @return
 */
queue_node* remove_first(queue_node** head, queue_node** tail)
{
    queue_node* temp = *head;
    *head = (*head)->next;
    if (*head != NULL)
    {
        (*head) ->previous = NULL;
    }

    return temp;
}