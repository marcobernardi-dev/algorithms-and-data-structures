#ifndef BREADTH_FIRST_VISIT_H
#define BREADTH_FIRST_VISIT_H

#include "graph.h"

void **breadth_first_visit(
    Graph gr,
    void *start,
    int (*compare)(const void *, const void *),
    unsigned long (*hash)(const void *)
);

#endif