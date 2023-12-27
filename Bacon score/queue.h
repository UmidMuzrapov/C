/**
 * Author: Umid Muzrapov
 *
 * Purpose: This is the header file for queue.h
 */
#ifndef BACON_QUEUE_H
#define BACON_QUEUE_H

#include <stdlib.h>
#include "utils.h"

typedef struct queue_node queue_node;

struct queue_node
{
    void *val_p;
    queue_node *next;
    queue_node *previous;
};

void add_last(queue_node **head, queue_node **tail, void *val_p);

queue_node *remove_first(queue_node **head, queue_node **tail);

#endif //BACON_QUEUE_H
