#ifndef GRAPH_H
#define GRAPH_H

typedef struct graph *Graph;

typedef struct edge {
    void *node1;
    void *node2;
    void *label;
} Edge;

Graph graph_create(
    int labelled,
    int directed,
    int (*compare)(const void *, const void *),
    unsigned long (*hash)(const void *)
);

int graph_is_directed(const Graph gr);

int graph_is_labelled(const Graph gr);

int graph_add_node(
    Graph gr,
    const void *node
);

int graph_contains_node(
    const Graph gr,
    const void *node
);

int graph_remove_node(
    Graph gr,
    const void *node
);

int graph_num_nodes(
    const Graph gr
);

void **graph_get_nodes(
    const Graph gr
);

int graph_add_edge(
    Graph gr,
    const void *node1,
    const void *node2,
    const void *label
);

int graph_contains_edge(
    const Graph gr,
    const void *node1,
    const void *node2
);

int graph_remove_edge(
    Graph gr,
    const void *node1,
    const void *node2
);

int graph_num_edges(
    const Graph gr
);

Edge **graph_get_edges(
    const Graph gr
);

void *graph_get_label(
    const Graph gr,
    const void *node1,
    const void *node2
);

void **graph_get_neighbours(
    const Graph gr,
    const void *node
);

int graph_num_neighbours(
    const Graph gr,
    const void *node
);

void graph_free(
    Graph gr
);

#endif