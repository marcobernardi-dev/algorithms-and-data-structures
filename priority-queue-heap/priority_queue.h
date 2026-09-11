#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

typedef struct PriorityQueue PriorityQueue;

PriorityQueue* priority_queue_create(
    int (*f1)(const void*, const void*),
    unsigned long (*f2)(const void*)
);

int priority_queue_push(PriorityQueue*, void*);
int priority_queue_contains(const PriorityQueue*, const void*);
void* priority_queue_top(const PriorityQueue*);
void priority_queue_pop(PriorityQueue*);
int priority_queue_remove(PriorityQueue*, const void*);
int priority_queue_size(const PriorityQueue*);
void priority_queue_free(PriorityQueue*);

#endif