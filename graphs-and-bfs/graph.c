#include "graph.h"
#include "../hash-table/hashtable.h"

#include <stdlib.h>

typedef struct {
    void *node;
    void *label;
} EdgeInfo;

struct graph {
    int labelled;
    int directed;
    int num_nodes;
    int num_edges;

    HashTable *nodes;

    int (*compare)(const void *, const void *);
    unsigned long (*hash)(const void *);
};

static HashTable *get_neighbours_table(
    const Graph gr,
    const void *node
)
{
    return hash_table_get(gr->nodes, node);
}

Graph graph_create(
    int labelled,
    int directed,
    int (*compare)(const void *, const void *),
    unsigned long (*hash)(const void *)
)
{
    if (compare == NULL || hash == NULL) {
        return NULL;
    }

    Graph gr = malloc(sizeof(struct graph));

    if (gr == NULL) {
        return NULL;
    }

    gr->nodes = hash_table_create(compare, hash);

    if (gr->nodes == NULL) {
        free(gr);
        return NULL;
    }

    gr->labelled = labelled;
    gr->directed = directed;
    gr->num_nodes = 0;
    gr->num_edges = 0;
    gr->compare = compare;
    gr->hash = hash;

    return gr;
}

int graph_is_directed(const Graph gr)
{
    if (gr == NULL) {
        return 0;
    }

    return gr->directed;
}

int graph_is_labelled(const Graph gr)
{
    if (gr == NULL) {
        return 0;
    }

    return gr->labelled;
}

int graph_add_node(
    Graph gr,
    const void *node
)
{
    if (gr == NULL || node == NULL) {
        return 0;
    }

    if (graph_contains_node(gr, node)) {
        return 0;
    }

    HashTable *neighbours =
        hash_table_create(
            gr->compare,
            gr->hash
        );

    if (neighbours == NULL) {
        return 0;
    }

    hash_table_put(
        gr->nodes,
        node,
        neighbours
    );

    if (!hash_table_contains_key(gr->nodes, node)) {
        hash_table_free(neighbours);
        return 0;
    }

    gr->num_nodes++;

    return 1;
}

int graph_contains_node(
    const Graph gr,
    const void *node
)
{
    if (gr == NULL || node == NULL) {
        return 0;
    }

    return hash_table_contains_key(
        gr->nodes,
        node
    );
}

int graph_remove_node(
    Graph gr,
    const void *node
)
{
    if (gr == NULL || node == NULL) {
        return 0;
    }

    if (!graph_contains_node(gr, node)) {
        return 0;
    }

    void **nodes = graph_get_nodes(gr);

    if (nodes != NULL) {
        for (int i = 0; i < gr->num_nodes; i++) {

            if (gr->compare(nodes[i], node) == 0) {
                continue;
            }

            HashTable *neighbours =
                get_neighbours_table(
                    gr,
                    nodes[i]
                );

            if (hash_table_contains_key(
                    neighbours,
                    node)) {

                hash_table_remove(
                    neighbours,
                    node
                );

                if (gr->directed) {
                    gr->num_edges--;
                }
            }
        }

        free(nodes);
    }

    HashTable *neighbours =
        get_neighbours_table(gr, node);

    if (neighbours != NULL) {
        gr->num_edges -=
            hash_table_size(neighbours);

        void **neighbour_nodes =
            hash_table_keyset(neighbours);

        if (neighbour_nodes != NULL) {
            int size =
                hash_table_size(neighbours);

            for (int i = 0; i < size; i++) {
                EdgeInfo *info =
                    hash_table_get(
                        neighbours,
                        neighbour_nodes[i]
                    );

                free(info);
            }

            free(neighbour_nodes);
        }

        hash_table_free(neighbours);
    }

    hash_table_remove(
        gr->nodes,
        node
    );

    gr->num_nodes--;

    return 1;
}

int graph_num_nodes(const Graph gr)
{
    if (gr == NULL) {
        return 0;
    }

    return gr->num_nodes;
}

void **graph_get_nodes(const Graph gr)
{
    if (gr == NULL) {
        return NULL;
    }

    return hash_table_keyset(gr->nodes);
}

int graph_add_edge(
    Graph gr,
    const void *node1,
    const void *node2,
    const void *label
)
{
    if (gr == NULL ||
        node1 == NULL ||
        node2 == NULL) {
        return 0;
    }

    if (!graph_contains_node(gr, node1) ||
        !graph_contains_node(gr, node2)) {
        return 0;
    }

    if (graph_contains_edge(
            gr,
            node1,
            node2)) {
        return 0;
    }

    HashTable *neighbours1 =
        get_neighbours_table(
            gr,
            node1
        );

    EdgeInfo *edge1 =
        malloc(sizeof(EdgeInfo));

    if (edge1 == NULL) {
        return 0;
    }

    edge1->node = (void *)node2;
    edge1->label = (void *)label;

    hash_table_put(
        neighbours1,
        node2,
        edge1
    );

    if (!hash_table_contains_key(
            neighbours1,
            node2)) {

        free(edge1);
        return 0;
    }

    if (!gr->directed &&
        gr->compare(node1, node2) != 0) {

        HashTable *neighbours2 =
            get_neighbours_table(
                gr,
                node2
            );

        EdgeInfo *edge2 =
            malloc(sizeof(EdgeInfo));

        if (edge2 == NULL) {
            hash_table_remove(
                neighbours1,
                node2
            );

            free(edge1);

            return 0;
        }

        edge2->node = (void *)node1;
        edge2->label = (void *)label;

        hash_table_put(
            neighbours2,
            node1,
            edge2
        );

        if (!hash_table_contains_key(
                neighbours2,
                node1)) {

            free(edge2);

            hash_table_remove(
                neighbours1,
                node2
            );

            free(edge1);

            return 0;
        }
    }

    gr->num_edges++;

    return 1;
}

int graph_contains_edge(
    const Graph gr,
    const void *node1,
    const void *node2
)
{
    if (gr == NULL ||
        node1 == NULL ||
        node2 == NULL) {
        return 0;
    }

    if (!graph_contains_node(gr, node1)) {
        return 0;
    }

    HashTable *neighbours =
        get_neighbours_table(
            gr,
            node1
        );

    return hash_table_contains_key(
        neighbours,
        node2
    );
}

int graph_remove_edge(
    Graph gr,
    const void *node1,
    const void *node2
)
{
    if (gr == NULL ||
        node1 == NULL ||
        node2 == NULL) {
        return 0;
    }

    if (!graph_contains_edge(
            gr,
            node1,
            node2)) {
        return 0;
    }

    HashTable *neighbours1 =
        get_neighbours_table(
            gr,
            node1
        );

    EdgeInfo *edge1 =
        hash_table_get(
            neighbours1,
            node2
        );

    hash_table_remove(
        neighbours1,
        node2
    );

    free(edge1);

    if (!gr->directed &&
        gr->compare(node1, node2) != 0) {

        HashTable *neighbours2 =
            get_neighbours_table(
                gr,
                node2
            );

        EdgeInfo *edge2 =
            hash_table_get(
                neighbours2,
                node1
            );

        hash_table_remove(
            neighbours2,
            node1
        );

        free(edge2);
    }

    gr->num_edges--;

    return 1;
}

int graph_num_edges(const Graph gr)
{
    if (gr == NULL) {
        return 0;
    }

    return gr->num_edges;
}

Edge **graph_get_edges(const Graph gr)
{
    if (gr == NULL || gr->num_edges == 0) {
        return NULL;
    }

    Edge **edges = malloc(
        gr->num_edges * sizeof(Edge *)
    );

    if (edges == NULL) {
        return NULL;
    }

    void **nodes = graph_get_nodes(gr);

    if (nodes == NULL) {
        free(edges);
        return NULL;
    }

    int position = 0;

    for (int i = 0; i < gr->num_nodes; i++) {

        HashTable *neighbours =
            get_neighbours_table(
                gr,
                nodes[i]
            );

        void **neighbour_nodes =
            hash_table_keyset(neighbours);

        int num_neighbours =
            hash_table_size(neighbours);

        if (neighbour_nodes == NULL &&
            num_neighbours > 0) {

            for (int k = 0; k < position; k++) {
                free(edges[k]);
            }

            free(nodes);
            free(edges);

            return NULL;
        }

        for (int j = 0; j < num_neighbours; j++) {

            void *neighbour =
                neighbour_nodes[j];

            if (!gr->directed &&
                gr->compare(
                    nodes[i],
                    neighbour
                ) > 0) {
                continue;
            }

            EdgeInfo *info =
                hash_table_get(
                    neighbours,
                    neighbour
                );

            Edge *edge =
                malloc(sizeof(Edge));

            if (edge == NULL) {
                for (int k = 0; k < position; k++) {
                    free(edges[k]);
                }

                free(neighbour_nodes);
                free(nodes);
                free(edges);

                return NULL;
            }

            edge->node1 = nodes[i];
            edge->node2 = neighbour;
            edge->label = info->label;

            edges[position] = edge;
            position++;
        }

        free(neighbour_nodes);
    }

    free(nodes);

    return edges;
}

void *graph_get_label(
    const Graph gr,
    const void *node1,
    const void *node2
)
{
    if (gr == NULL ||
        node1 == NULL ||
        node2 == NULL) {
        return NULL;
    }

    if (!graph_contains_edge(
            gr,
            node1,
            node2)) {
        return NULL;
    }

    HashTable *neighbours =
        get_neighbours_table(
            gr,
            node1
        );

    EdgeInfo *info =
        hash_table_get(
            neighbours,
            node2
        );

    if (info == NULL) {
        return NULL;
    }

    return info->label;
}

void **graph_get_neighbours(
    const Graph gr,
    const void *node
)
{
    if (gr == NULL || node == NULL) {
        return NULL;
    }

    HashTable *neighbours =
        get_neighbours_table(
            gr,
            node
        );

    if (neighbours == NULL) {
        return NULL;
    }

    return hash_table_keyset(neighbours);
}

int graph_num_neighbours(
    const Graph gr,
    const void *node
)
{
    if (gr == NULL || node == NULL) {
        return 0;
    }

    HashTable *neighbours =
        get_neighbours_table(
            gr,
            node
        );

    if (neighbours == NULL) {
        return 0;
    }

    return hash_table_size(neighbours);
}

void graph_free(Graph gr)
{
    if (gr == NULL) {
        return;
    }

    void **nodes = graph_get_nodes(gr);

    if (nodes != NULL) {

        for (int i = 0; i < gr->num_nodes; i++) {

            HashTable *neighbours =
                get_neighbours_table(
                    gr,
                    nodes[i]
                );

            if (neighbours != NULL) {

                void **neighbour_nodes =
                    hash_table_keyset(
                        neighbours
                    );

                if (neighbour_nodes != NULL) {

                    int size =
                        hash_table_size(
                            neighbours
                        );

                    for (int j = 0; j < size; j++) {

                        EdgeInfo *info =
                            hash_table_get(
                                neighbours,
                                neighbour_nodes[j]
                            );

                        free(info);
                    }

                    free(neighbour_nodes);
                }

                hash_table_free(neighbours);
            }
        }

        free(nodes);
    }

    hash_table_free(gr->nodes);
    free(gr);
}