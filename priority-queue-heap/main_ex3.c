#include "priority_queue.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned long id;
    long start;
    long length;
    int priority;
} Task;

static int compare_tasks(const void *a, const void *b) {
    const Task *task_a = a;
    const Task *task_b = b;

    if (task_a->priority > task_b->priority) {
        return 1;
    }

    if (task_a->priority < task_b->priority) {
        return -1;
    }

    return 0;
}

static unsigned long hash_task(const void *element) {
    const Task *task = element;

    return task->id;
}

static int read_task(FILE *file, Task *task) {
    return fscanf(
        file,
        "%lu,%ld,%ld,%d",
        &task->id,
        &task->start,
        &task->length,
        &task->priority
    ) == 4;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <input.csv> <output.csv>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("Errore apertura file");
        return EXIT_FAILURE;
    }

    FILE *output_file = fopen(argv[2], "w");

    if (output_file == NULL) {
        perror("Errore apertura file output");
        fclose(file);
        return EXIT_FAILURE;
    }
    
    PriorityQueue *pq =
        priority_queue_create(compare_tasks, hash_task);

    if (pq == NULL) {
        fclose(output_file);
        fclose(file);
        fprintf(stderr, "Errore creazione priority queue\n");
        return EXIT_FAILURE;
    }

    Task next_task;
    int has_next_task = read_task(file, &next_task);

    long current_time = 0;

    if (has_next_task) {
        current_time = next_task.start;
    }

    while (has_next_task || priority_queue_size(pq) > 0) {

        //Inserisce nella coda tutti i task già arrivati.
        while (has_next_task &&
               next_task.start <= current_time) {

            Task *task = malloc(sizeof(Task));

            if (task == NULL) {
                fprintf(stderr, "Errore allocazione memoria\n");
                priority_queue_free(pq);
                fclose(file);
                return EXIT_FAILURE;
            }

            *task = next_task;

            if (priority_queue_push(pq, task) != 0) {
                free(task);
                priority_queue_free(pq);
                fclose(file);
                return EXIT_FAILURE;
            }

            has_next_task = read_task(file, &next_task);
        }

        /*Se non ci sono task pronti, il processore rimane inattivo
        fino all'arrivo del prossimo task. */

        if (priority_queue_size(pq) == 0) {
            if (has_next_task) {
                current_time = next_task.start;
            }

            continue;
        }

        Task *task = priority_queue_top(pq);

        priority_queue_pop(pq);

        long start_time = current_time;
        long end_time = current_time + task->length;

        fprintf(
            output_file,
            "%lu,%ld,%ld\n",
            task->id,
            start_time,
            end_time
        );

        current_time = end_time;

        free(task);
    }

    priority_queue_free(pq);
    fclose(output_file);
    fclose(file);

    return EXIT_SUCCESS;
}