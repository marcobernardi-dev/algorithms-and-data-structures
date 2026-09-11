#include "hybridsort.h"
#include <stdlib.h>
#include <string.h>

static void swap(void *a, void *b, size_t size) {
    
    void *temp = malloc(size);

    if (temp == NULL) {
        exit(EXIT_FAILURE);
    }

    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);

    free(temp);
}

static void selectionsort(
    void *base,
    size_t nitems,
    size_t size,
    int (*compar)(const void *, const void *)
){
    if (nitems < 2) {
        return;
    }

    for (size_t i = 0; i < nitems - 1; i++) {
        size_t min = i;

        for (size_t j = i + 1; j < nitems; j++) {
            if (compar((unsigned char *)base + j * size,
                (unsigned char *)base + min * size) < 0){
                min = j;
            }
        }
        swap((unsigned char *)base + i * size,
            (unsigned char *)base + min * size,
            size
        );
    }
}

static void partition(
    void *base,
    size_t low,
    size_t high,
    size_t size,
    int (*compar)(const void *, const void *),
    size_t *left_end,
    size_t *right_start
){
    size_t i = low;
    size_t j = low;
    size_t k = high;

    size_t pivot_index = low + (high - low) / 2;

    void *pivot = malloc(size);

    if (pivot == NULL) {
        exit(EXIT_FAILURE);
    }

    memcpy(
        pivot,
        (unsigned char *)base + pivot_index * size,
        size
    );

    while (j <= k) {
        int cmp = compar(
            (unsigned char *)base + j * size,
            pivot
        );

        if (cmp < 0) {
            swap(
                (unsigned char *)base + i * size,
                (unsigned char *)base + j * size,
                size
            );

            i++;
            j++;
        }
        else if (cmp > 0) {
            swap(
                (unsigned char *)base + j * size,
                (unsigned char *)base + k * size,
                size
            );

            if (k == 0) {
                break;
            }

            k--;
        }
        else {
            j++;
        }
    }

    free(pivot);

    *left_end = i;
    *right_start = j;
}

static void quicksort(
    void *base, 
    size_t low, 
    size_t high, 
    size_t size, 
    size_t k,
    int (*compar)(const void *, const void *)){

    if(low >= high){
        return;
    }

    size_t nitems = high - low + 1;

    if (nitems < k) {
        selectionsort(
            (unsigned char *)base + low * size,
            nitems,
            size,
            compar
        );
        return;
    }

    size_t left_end;
    size_t right_start;

    partition(
        base,
        low,
        high,
        size,
        compar,
        &left_end,
        &right_start
    );

    if (left_end > low) {
        quicksort(base, low, left_end - 1, size, k, compar);
    }

    if (right_start < high) {
        quicksort(base, right_start, high, size, k, compar);
    }
}

void hybridsort(
    void *base,
    size_t nitems,
    size_t size,
    size_t k,
    int (*compar)(const void *, const void *)
){

    if (nitems == 0){
        return;
    }
    if(nitems < k){
        selectionsort(base, nitems, size, compar);
        return;
    }

    quicksort(base, 0, nitems - 1, size, k, compar);
}