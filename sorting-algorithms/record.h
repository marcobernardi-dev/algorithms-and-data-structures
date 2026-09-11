#ifndef RECORD_H
#define RECORD_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct __attribute__((packed)) {
    uint64_t id;
    float field1;
    int64_t field2;
    char field3[16];
} Record;

int read_record(FILE *file, Record *record);

int write_record(FILE *file, const Record *record);

int load_records_from_file(
    FILE *file,
    Record **records,
    size_t *count
);

int save_records_to_file(
    FILE *file,
    const Record *records,
    size_t count
);

int compare_id(const void *a, const void *b);
int compare_field1(const void *a, const void *b);
int compare_field2(const void *a, const void *b);
int compare_field3(const void *a, const void *b);

#endif