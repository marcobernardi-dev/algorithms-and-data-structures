#include "graph.h"
#include "breadth_first_visit.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 1024

static int compare_strings(const void *a, const void *b)
{
    return strcmp(
        (const char *)a,
        (const char *)b
    );
}

static unsigned long hash_string(const void *element)
{
    const char *string = element;
    unsigned long hash = 5381;
    int c;

    while ((c = *string++) != '\0') {
        hash = ((hash << 5) + hash) + (unsigned long)c;
    }

    return hash;
}

static char *find_node(
    Graph graph,
    const char *name
)
{
    void **nodes = graph_get_nodes(graph);

    if (nodes == NULL) {
        return NULL;
    }

    int num_nodes = graph_num_nodes(graph);

    for (int i = 0; i < num_nodes; i++) {
        if (strcmp(
                (const char *)nodes[i],
                name
            ) == 0) {

            char *result = nodes[i];
            free(nodes);

            return result;
        }
    }

    free(nodes);

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(
            stderr,
            "Uso: %s <input.csv> <citta> <output.txt>\n",
            argv[0]
        );

        return EXIT_FAILURE;
    }

    FILE *input = fopen(argv[1], "r");

    if (input == NULL) {
        perror("Errore apertura file input");
        return EXIT_FAILURE;
    }

    FILE *output = fopen(argv[3], "w");

    if (output == NULL) {
        perror("Errore apertura file output");
        fclose(input);

        return EXIT_FAILURE;
    }

    Graph graph = graph_create(
        1,
        0,
        compare_strings,
        hash_string
    );

    if (graph == NULL) {
        fprintf(
            stderr,
            "Errore creazione grafo\n"
        );

        fclose(output);
        fclose(input);

        return EXIT_FAILURE;
    }

    char line[LINE_SIZE];

    while (fgets(line, sizeof(line), input) != NULL) {

        char *place1 = strtok(line, ",");
        char *place2 = strtok(NULL, ",");
        char *distance_string = strtok(NULL, "\r\n");

        if (place1 == NULL ||
            place2 == NULL ||
            distance_string == NULL) {
            continue;
        }

        char *node1 = malloc(
            strlen(place1) + 1
        );

        char *node2 = malloc(
            strlen(place2) + 1
        );

        if (node1 == NULL || node2 == NULL) {
            free(node1);
            free(node2);

            fprintf(
                stderr,
                "Errore allocazione memoria\n"
            );

            graph_free(graph);
            fclose(output);
            fclose(input);

            return EXIT_FAILURE;
        }

        strcpy(node1, place1);
        strcpy(node2, place2);

        if (!graph_contains_node(graph, node1)) {

            if (!graph_add_node(graph, node1)) {
                free(node1);
                free(node2);

                fprintf(
                    stderr,
                    "Errore aggiunta nodo\n"
                );

                graph_free(graph);
                fclose(output);
                fclose(input);

                return EXIT_FAILURE;
            }

        } else {
            free(node1);
        }

        if (!graph_contains_node(graph, node2)) {

            if (!graph_add_node(graph, node2)) {
                free(node2);

                fprintf(
                    stderr,
                    "Errore aggiunta nodo\n"
                );

                graph_free(graph);
                fclose(output);
                fclose(input);

                return EXIT_FAILURE;
            }

        } else {
            free(node2);
        }

        char *actual_node1 =
            find_node(graph, place1);

        char *actual_node2 =
            find_node(graph, place2);

        if (actual_node1 == NULL ||
            actual_node2 == NULL) {

            fprintf(
                stderr,
                "Errore recupero nodo\n"
            );

            graph_free(graph);
            fclose(output);
            fclose(input);

            return EXIT_FAILURE;
        }

        float *distance = malloc(sizeof(float));

        if (distance == NULL) {
            fprintf(
                stderr,
                "Errore allocazione memoria\n"
            );

            graph_free(graph);
            fclose(output);
            fclose(input);

            return EXIT_FAILURE;
        }

        *distance = strtof(
            distance_string,
            NULL
        );

        if (!graph_add_edge(
                graph,
                actual_node1,
                actual_node2,
                distance)) {

            free(distance);
        }
    }

    fclose(input);

    char *start =
        find_node(graph, argv[2]);

    if (start == NULL) {
        fprintf(
            stderr,
            "Errore: città '%s' non presente nel grafo\n",
            argv[2]
        );

        graph_free(graph);
        fclose(output);

        return EXIT_FAILURE;
    }

    void **visited = breadth_first_visit(
        graph,
        start,
        compare_strings,
        hash_string
    );

    if (visited == NULL) {
        fprintf(
            stderr,
            "Errore durante la visita BFS\n"
        );

        graph_free(graph);
        fclose(output);

        return EXIT_FAILURE;
    }

    int num_nodes = graph_num_nodes(graph);

    for (int i = 0; i < num_nodes; i++) {
        fprintf(
            output,
            "%s\n",
            (char *)visited[i]
        );
    }

    free(visited);
    graph_free(graph);
    fclose(output);

    return EXIT_SUCCESS;
}