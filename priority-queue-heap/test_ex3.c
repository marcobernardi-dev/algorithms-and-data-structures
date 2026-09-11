#include "priority_queue.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value;
    int priority;
} Item;

static int compare_items(const void *a, const void *b) {
    const Item *item_a = a;
    const Item *item_b = b;

    if (item_a->priority > item_b->priority) {
        return 1;
    }

    if (item_a->priority < item_b->priority) {
        return -1;
    }

    return 0;
}

static unsigned long hash_item(const void *element) {
    const Item *item = element;

    return (unsigned long)item->value;
}

static int test_push_and_top(void) {
    Item a = {1, 10};
    Item b = {2, 30};
    Item c = {3, 20};

    PriorityQueue *pq =
        priority_queue_create(compare_items, hash_item);

    if (pq == NULL) {
        return 0;
    }

    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);

    Item *top = priority_queue_top(pq);

    int result =
        top == &b &&
        priority_queue_size(pq) == 3;

    priority_queue_free(pq);

    return result;
}

static int test_pop(void) {
    Item a = {1, 10};
    Item b = {2, 30};
    Item c = {3, 20};

    PriorityQueue *pq =
        priority_queue_create(compare_items, hash_item);

    if (pq == NULL) {
        return 0;
    }

    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);

    priority_queue_pop(pq);

    Item *top = priority_queue_top(pq);

    int result =
        top == &c &&
        priority_queue_size(pq) == 2 &&
        !priority_queue_contains(pq, &b);

    priority_queue_free(pq);

    return result;
}

static int test_contains(void) {
    Item a = {1, 10};
    Item b = {2, 20};

    PriorityQueue *pq =
        priority_queue_create(compare_items, hash_item);

    if (pq == NULL) {
        return 0;
    }

    priority_queue_push(pq, &a);

    int result =
        priority_queue_contains(pq, &a) &&
        !priority_queue_contains(pq, &b);

    priority_queue_free(pq);

    return result;
}

static int test_remove(void) {
    Item a = {1, 10};
    Item b = {2, 50};
    Item c = {3, 30};
    Item d = {4, 20};

    PriorityQueue *pq =
        priority_queue_create(compare_items, hash_item);

    if (pq == NULL) {
        return 0;
    }

    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);
    priority_queue_push(pq, &d);

    int removed = priority_queue_remove(pq, &c);

    Item *top = priority_queue_top(pq);

    int result =
        removed &&
        !priority_queue_contains(pq, &c) &&
        priority_queue_size(pq) == 3 &&
        top == &b;

    priority_queue_free(pq);

    return result;
}

static int test_resize(void) {
    Item items[32];

    PriorityQueue *pq =
        priority_queue_create(compare_items, hash_item);

    if (pq == NULL) {
        return 0;
    }

    for (int i = 0; i < 32; i++) {
        items[i].value = i + 1;
        items[i].priority = i;

        if (priority_queue_push(pq, &items[i]) != 0) {
            priority_queue_free(pq);
            return 0;
        }
    }

    int result =
        priority_queue_size(pq) == 32 &&
        priority_queue_top(pq) == &items[31];

    priority_queue_free(pq);

    return result;
}

static int test_invalid_arguments(void) {
    Item a = {1, 10};

    PriorityQueue *pq =
        priority_queue_create(compare_items, hash_item);

    if (pq == NULL) {
        return 0;
    }

    int result =
        priority_queue_push(NULL, &a) == -1 &&
        priority_queue_push(pq, NULL) == -1 &&
        priority_queue_contains(NULL, &a) == 0 &&
        priority_queue_contains(pq, NULL) == 0 &&
        priority_queue_top(NULL) == NULL &&
        priority_queue_top(pq) == NULL &&
        priority_queue_remove(NULL, &a) == 0 &&
        priority_queue_remove(pq, NULL) == 0 &&
        priority_queue_size(NULL) == 0;

    priority_queue_free(pq);

    return result;
}

int main(void) {
    int passed = 0;
    int total = 6;

    if (test_push_and_top()) {
        printf("PASS: push e top\n");
        passed++;
    } else {
        printf("FAIL: push e top\n");
    }

    if (test_pop()) {
        printf("PASS: pop\n");
        passed++;
    } else {
        printf("FAIL: pop\n");
    }

    if (test_contains()) {
        printf("PASS: contains\n");
        passed++;
    } else {
        printf("FAIL: contains\n");
    }

    if (test_remove()) {
        printf("PASS: remove\n");
        passed++;
    } else {
        printf("FAIL: remove\n");
    }

    if (test_resize()) {
        printf("PASS: resize\n");
        passed++;
    } else {
        printf("FAIL: resize\n");
    }

    if (test_invalid_arguments()) {
        printf("PASS: argomenti invalidi\n");
        passed++;
    } else {
        printf("FAIL: argomenti invalidi\n");
    }

    printf("\nRisultato: %d/%d test superati.\n", passed, total);

    return passed == total ? 0 : 1;
}