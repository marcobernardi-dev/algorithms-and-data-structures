#include "graph.h"
#include "breadth_first_visit.h"

#include <stdio.h>
#include <stdlib.h>

static int compare_ints(const void *a, const void *b)
{
    const int *ia = a;
    const int *ib = b;

    if (*ia < *ib) {
        return -1;
    }

    if (*ia > *ib) {
        return 1;
    }

    return 0;
}

static unsigned long hash_int(const void *element)
{
    const int *value = element;

    return (unsigned long)*value;
}

static int test_nodes(void)
{
    Graph gr = graph_create(
        0,
        0,
        compare_ints,
        hash_int
    );

    if (gr == NULL) {
        return 0;
    }

    int a = 1;
    int b = 2;
    int c = 3;

    int result =
        graph_add_node(gr, &a) &&
        graph_add_node(gr, &b) &&
        graph_add_node(gr, &c) &&
        graph_num_nodes(gr) == 3 &&
        graph_contains_node(gr, &a) &&
        graph_contains_node(gr, &b) &&
        graph_contains_node(gr, &c) &&
        !graph_add_node(gr, &a);

    graph_free(gr);

    return result;
}

static int test_undirected_edges(void)
{
    Graph gr = graph_create(
        1,
        0,
        compare_ints,
        hash_int
    );

    if (gr == NULL) {
        return 0;
    }

    int a = 1;
    int b = 2;
    int c = 3;
    int label = 42;

    graph_add_node(gr, &a);
    graph_add_node(gr, &b);
    graph_add_node(gr, &c);

    int result =
        graph_add_edge(gr, &a, &b, &label) &&
        graph_num_edges(gr) == 1 &&
        graph_contains_edge(gr, &a, &b) &&
        graph_contains_edge(gr, &b, &a) &&
        graph_get_label(gr, &a, &b) == &label &&
        graph_get_label(gr, &b, &a) == &label &&
        graph_num_neighbours(gr, &a) == 1 &&
        graph_num_neighbours(gr, &b) == 1 &&
        !graph_add_edge(gr, &a, &b, &label);

    graph_free(gr);

    return result;
}

static int test_directed_edges(void)
{
    Graph gr = graph_create(
        1,
        1,
        compare_ints,
        hash_int
    );

    if (gr == NULL) {
        return 0;
    }

    int a = 1;
    int b = 2;
    int label = 99;

    graph_add_node(gr, &a);
    graph_add_node(gr, &b);

    int result =
        graph_add_edge(gr, &a, &b, &label) &&
        graph_num_edges(gr) == 1 &&
        graph_contains_edge(gr, &a, &b) &&
        !graph_contains_edge(gr, &b, &a) &&
        graph_get_label(gr, &a, &b) == &label &&
        graph_num_neighbours(gr, &a) == 1 &&
        graph_num_neighbours(gr, &b) == 0;

    graph_free(gr);

    return result;
}

static int test_remove_edge(void)
{
    Graph gr = graph_create(
        0,
        0,
        compare_ints,
        hash_int
    );

    if (gr == NULL) {
        return 0;
    }

    int a = 1;
    int b = 2;

    graph_add_node(gr, &a);
    graph_add_node(gr, &b);
    graph_add_edge(gr, &a, &b, NULL);

    int result =
        graph_num_edges(gr) == 1 &&
        graph_remove_edge(gr, &a, &b) &&
        graph_num_edges(gr) == 0 &&
        !graph_contains_edge(gr, &a, &b) &&
        !graph_contains_edge(gr, &b, &a) &&
        !graph_remove_edge(gr, &a, &b);

    graph_free(gr);

    return result;
}

static int test_remove_node(void)
{
    Graph gr = graph_create(
        0,
        0,
        compare_ints,
        hash_int
    );

    if (gr == NULL) {
        return 0;
    }

    int a = 1;
    int b = 2;
    int c = 3;

    graph_add_node(gr, &a);
    graph_add_node(gr, &b);
    graph_add_node(gr, &c);

    graph_add_edge(gr, &a, &b, NULL);
    graph_add_edge(gr, &b, &c, NULL);

    int result =
        graph_num_nodes(gr) == 3 &&
        graph_num_edges(gr) == 2 &&
        graph_remove_node(gr, &b) &&
        graph_num_nodes(gr) == 2 &&
        graph_num_edges(gr) == 0 &&
        !graph_contains_node(gr, &b) &&
        !graph_contains_edge(gr, &a, &b) &&
        !graph_contains_edge(gr, &c, &b);

    graph_free(gr);

    return result;
}

static int test_get_neighbours(void)
{
    Graph gr = graph_create(
        0,
        1,
        compare_ints,
        hash_int
    );

    if (gr == NULL) {
        return 0;
    }

    int a = 1;
    int b = 2;
    int c = 3;

    graph_add_node(gr, &a);
    graph_add_node(gr, &b);
    graph_add_node(gr, &c);

    graph_add_edge(gr, &a, &b, NULL);
    graph_add_edge(gr, &a, &c, NULL);

    void **neighbours =
        graph_get_neighbours(gr, &a);

    if (neighbours == NULL) {
        graph_free(gr);
        return 0;
    }

    int found_b = 0;
    int found_c = 0;

    for (int i = 0; i < 2; i++) {
        if (neighbours[i] == &b) {
            found_b = 1;
        }

        if (neighbours[i] == &c) {
            found_c = 1;
        }
    }

    int result =
        graph_num_neighbours(gr, &a) == 2 &&
        found_b &&
        found_c;

    free(neighbours);
    graph_free(gr);

    return result;
}

static int test_bfs_order(void)
{
    Graph gr = graph_create(
        0,
        0,
        compare_ints,
        hash_int
    );

    if (gr == NULL) {
        return 0;
    }

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;

    graph_add_node(gr, &a);
    graph_add_node(gr, &b);
    graph_add_node(gr, &c);
    graph_add_node(gr, &d);
    graph_add_node(gr, &e);

    graph_add_edge(gr, &a, &b, NULL);
    graph_add_edge(gr, &a, &c, NULL);
    graph_add_edge(gr, &b, &d, NULL);
    graph_add_edge(gr, &c, &e, NULL);

    void **result = breadth_first_visit(
        gr,
        &a,
        compare_ints,
        hash_int
    );

    if (result == NULL) {
        graph_free(gr);
        return 0;
    }

    int correct =
        result[0] == &a &&
        result[1] == &b &&
        result[2] == &c &&
        result[3] == &d &&
        result[4] == &e;

    free(result);
    graph_free(gr);

    return correct;
}

static int test_bfs_disconnected(void)
{
    Graph gr = graph_create(
        0,
        0,
        compare_ints,
        hash_int
    );

    if (gr == NULL) {
        return 0;
    }

    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;

    graph_add_node(gr, &a);
    graph_add_node(gr, &b);
    graph_add_node(gr, &c);
    graph_add_node(gr, &d);

    graph_add_edge(gr, &a, &b, NULL);
    graph_add_edge(gr, &c, &d, NULL);

    void **result = breadth_first_visit(
        gr,
        &a,
        compare_ints,
        hash_int
    );

    if (result == NULL) {
        graph_free(gr);
        return 0;
    }

    int correct =
        result[0] == &a &&
        result[1] == &b;

    free(result);
    graph_free(gr);

    return correct;
}

static int test_bfs_invalid_start(void)
{
    Graph gr = graph_create(
        0,
        0,
        compare_ints,
        hash_int
    );

    if (gr == NULL) {
        return 0;
    }

    int a = 1;
    int b = 2;
    int missing = 99;

    graph_add_node(gr, &a);
    graph_add_node(gr, &b);
    graph_add_edge(gr, &a, &b, NULL);

    void **result = breadth_first_visit(
        gr,
        &missing,
        compare_ints,
        hash_int
    );

    graph_free(gr);

    return result == NULL;
}

int main(void)
{
    int passed = 0;
    int total = 9;

    if (test_nodes()) {
        printf("PASS: nodi\n");
        passed++;
    } else {
        printf("FAIL: nodi\n");
    }

    if (test_undirected_edges()) {
        printf("PASS: archi non diretti\n");
        passed++;
    } else {
        printf("FAIL: archi non diretti\n");
    }

    if (test_directed_edges()) {
        printf("PASS: archi diretti\n");
        passed++;
    } else {
        printf("FAIL: archi diretti\n");
    }

    if (test_remove_edge()) {
        printf("PASS: rimozione arco\n");
        passed++;
    } else {
        printf("FAIL: rimozione arco\n");
    }

    if (test_remove_node()) {
        printf("PASS: rimozione nodo\n");
        passed++;
    } else {
        printf("FAIL: rimozione nodo\n");
    }

    if (test_get_neighbours()) {
        printf("PASS: vicini\n");
        passed++;
    } else {
        printf("FAIL: vicini\n");
    }

    if (test_bfs_order()) {
        printf("PASS: BFS ordine\n");
        passed++;
    } else {
        printf("FAIL: BFS ordine\n");
    }

    if (test_bfs_disconnected()) {
        printf("PASS: BFS grafo disconnesso\n");
        passed++;
    } else {
        printf("FAIL: BFS grafo disconnesso\n");
    }

    if (test_bfs_invalid_start()) {
        printf("PASS: BFS nodo iniziale non presente\n");
        passed++;
    } else {
        printf("FAIL: BFS nodo iniziale non presente\n");
    }

    printf("\nRisultato: %d/%d test superati.\n", passed, total);

    return passed == total
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}