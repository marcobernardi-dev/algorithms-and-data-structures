#include "breadth_first_visit.h"
#include "../hash-table/hashtable.h"

#include <stdlib.h>

void **breadth_first_visit(
    Graph gr,
    void *start,
    int (*compare)(const void *, const void *),
    unsigned long (*hash)(const void *)
)
{
    if (gr == NULL ||
        start == NULL ||
        compare == NULL ||
        hash == NULL) {
        return NULL;
    }

    if (!graph_contains_node(gr, start)) {
        return NULL;
    }

    int num_nodes = graph_num_nodes(gr);

    if (num_nodes <= 0) {
        return NULL;
    }

    void **result = malloc(
        num_nodes * sizeof(void *)
    );

    if (result == NULL) {
        return NULL;
    }

    void **queue = malloc(
        num_nodes * sizeof(void *)
    );

    if (queue == NULL) {
        free(result);
        return NULL;
    }

    HashTable *visited =
        hash_table_create(
            compare,
            hash
        );

    if (visited == NULL) {
        free(queue);
        free(result);
        return NULL;
    }

    int result_size = 0;
    int queue_head = 0;
    int queue_tail = 0;

    hash_table_put(
        visited,
        start,
        start
    );

    queue[queue_tail] = start;
    queue_tail++;

    while (queue_head < queue_tail) {

        void *current =
            queue[queue_head];

        queue_head++;

        result[result_size] = current;
        result_size++;

        void **neighbours =
            graph_get_neighbours(
                gr,
                current
            );

        int num_neighbours =
            graph_num_neighbours(
                gr,
                current
            );

        if (neighbours == NULL &&
            num_neighbours > 0) {

            hash_table_free(visited);
            free(queue);
            free(result);

            return NULL;
        }

        for (int i = 0; i < num_neighbours; i++) {

            void *neighbour =
                neighbours[i];

            if (!hash_table_contains_key(
                    visited,
                    neighbour)) {

                hash_table_put(
                    visited,
                    neighbour,
                    neighbour
                );

                queue[queue_tail] =
                    neighbour;

                queue_tail++;
            }
        }

        free(neighbours);
    }

    hash_table_free(visited);
    free(queue);

    return result;
}