#include "priority_queue.h"
#include "../hash-table/hashtable.h"

#include <stdint.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 16

struct PriorityQueue {
    void **data;
    unsigned long size;
    unsigned long capacity;

    int (*compare)(const void *, const void *);
    unsigned long (*hash)(const void *);

    HashTable *positions;
};

static int compare_elements(const void *a, const void *b) {
    const void *pa = *(const void * const *)a;
    const void *pb = *(const void * const *)b;

    if (pa < pb) {
        return -1;
    }

    if (pa > pb) {
        return 1;
    }

    return 0;
}

static unsigned long hash_element(const void *element) {
    return (unsigned long)(uintptr_t)element;
}

static int priority_queue_resize(
    PriorityQueue *pq,
    unsigned long new_capacity
) {
    void **new_data = realloc(
        pq->data,
        new_capacity * sizeof(void *)
    );

    if (new_data == NULL) {
        return -1;
    }

    pq->data = new_data;
    pq->capacity = new_capacity;

    return 0;
}

static unsigned long parent_index(unsigned long index) {
    return (index - 1) / 3;
}

static unsigned long child_index(
    unsigned long index,
    unsigned long child
) {
    return 3 * index + child;
}

static void swap_elements(
    PriorityQueue *pq,
    unsigned long first,
    unsigned long second
) {
    void *tmp = pq->data[first];
    pq->data[first] = pq->data[second];
    pq->data[second] = tmp;

    hash_table_put(
        pq->positions,
        pq->data[first],
        (void *)(uintptr_t)(first + 1)
    );

    hash_table_put(
        pq->positions,
        pq->data[second],
        (void *)(uintptr_t)(second + 1)
    );
}

static void sift_up(
    PriorityQueue *pq,
    unsigned long index
) {
    while (index > 0) {
        unsigned long parent = parent_index(index);

        if (pq->compare(
                pq->data[index],
                pq->data[parent]
            ) <= 0) {
            break;
        }

        swap_elements(pq, index, parent);
        index = parent;
    }
}

static void sift_down(
    PriorityQueue *pq,
    unsigned long index
) {
    while (1) {
        unsigned long best = index;

        for (unsigned long child = 1; child <= 3; child++) {
            unsigned long child_pos =
                child_index(index, child);

            if (child_pos < pq->size &&
                pq->compare(
                    pq->data[child_pos],
                    pq->data[best]
                ) > 0) {
                best = child_pos;
            }
        }

        if (best == index) {
            break;
        }

        swap_elements(pq, index, best);
        index = best;
    }
}

PriorityQueue* priority_queue_create(
    int (*f1)(const void *, const void *),
    unsigned long (*f2)(const void *)
) {
    if (f1 == NULL || f2 == NULL) {
        return NULL;
    }

    PriorityQueue *pq = malloc(sizeof(PriorityQueue));

    if (pq == NULL) {
        return NULL;
    }

    pq->data = malloc(
        INITIAL_CAPACITY * sizeof(void *)
    );

    if (pq->data == NULL) {
        free(pq);
        return NULL;
    }

    pq->positions = hash_table_create(
        compare_elements,
        hash_element
    );

    if (pq->positions == NULL) {
        free(pq->data);
        free(pq);
        return NULL;
    }

    pq->size = 0;
    pq->capacity = INITIAL_CAPACITY;
    pq->compare = f1;
    pq->hash = f2;

    return pq;
}

int priority_queue_push(
    PriorityQueue *pq,
    void *element
) {
    if (pq == NULL || element == NULL) {
        return -1;
    }

    if (hash_table_contains_key(
            pq->positions,
            element
        )) {
        return -1;
    }

    if (pq->size == pq->capacity) {
        if (priority_queue_resize(
                pq,
                pq->capacity * 2
            ) != 0) {
            return -1;
        }
    }

    pq->data[pq->size] = element;

    hash_table_put(
        pq->positions,
        element,
        (void *)(uintptr_t)(pq->size + 1)
    );

    pq->size++;

    sift_up(pq, pq->size - 1);

    return 0;
}

int priority_queue_contains(
    const PriorityQueue *pq,
    const void *element
) {
    if (pq == NULL || element == NULL) {
        return 0;
    }

    return hash_table_contains_key(
        pq->positions,
        element
    );
}

void* priority_queue_top(
    const PriorityQueue *pq
) {
    if (pq == NULL || pq->size == 0) {
        return NULL;
    }

    return pq->data[0];
}

void priority_queue_pop(
    PriorityQueue *pq
) {
    if (pq == NULL || pq->size == 0) {
        return;
    }

    void *removed = pq->data[0];

    hash_table_remove(
        pq->positions,
        removed
    );

    pq->size--;

    if (pq->size == 0) {
        return;
    }

    pq->data[0] = pq->data[pq->size];

    hash_table_put(
        pq->positions,
        pq->data[0],
        (void *)(uintptr_t)1
    );

    sift_down(pq, 0);
}

int priority_queue_remove(
    PriorityQueue *pq,
    const void *element
) {
    if (pq == NULL || element == NULL) {
        return 0;
    }

    if (!hash_table_contains_key(
            pq->positions,
            element
        )) {
        return 0;
    }

    void *value = hash_table_get(
        pq->positions,
        element
    );

    unsigned long index =
        (unsigned long)(uintptr_t)value - 1;

    hash_table_remove(
        pq->positions,
        element
    );

    pq->size--;

    if (index == pq->size) {
        return 1;
    }

    pq->data[index] = pq->data[pq->size];

    hash_table_put(
        pq->positions,
        pq->data[index],
        (void *)(uintptr_t)(index + 1)
    );

    if (index > 0) {
        unsigned long parent = parent_index(index);

        if (pq->compare(
                pq->data[index],
                pq->data[parent]
            ) > 0) {
            sift_up(pq, index);
            return 1;
        }
    }

    sift_down(pq, index);

    return 1;
}

int priority_queue_size(
    const PriorityQueue *pq
) {
    if (pq == NULL) {
        return 0;
    }

    return (int)pq->size;
}

void priority_queue_free(
    PriorityQueue *pq
) {
    if (pq == NULL) {
        return;
    }

    hash_table_free(pq->positions);
    free(pq->data);
    free(pq);
}