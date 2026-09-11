#include "record.h"

#include <stdlib.h>
#include <string.h>

int compare_id(const void *a, const void *b)
{
    const Record *record_a = a;
    const Record *record_b = b;

    if (record_a->id < record_b->id) {
        return -1;
    }

    if (record_a->id > record_b->id) {
        return 1;
    }

    return 0;
}

int compare_field1(const void *a, const void *b)
{
    const Record *record_a = a;
    const Record *record_b = b;

    if (record_a->field1 < record_b->field1) {
        return -1;
    }

    if (record_a->field1 > record_b->field1) {
        return 1;
    }

    return 0;
}

int compare_field2(const void *a, const void *b)
{
    const Record *record_a = a;
    const Record *record_b = b;

    if (record_a->field2 < record_b->field2) {
        return -1;
    }

    if (record_a->field2 > record_b->field2) {
        return 1;
    }

    return 0;
}

int compare_field3(const void *a, const void *b)
{
    const Record *record_a = a;
    const Record *record_b = b;

    return strcmp(record_a->field3, record_b->field3);
}

int read_record(FILE *file, Record *record)
{
    return fread(record, sizeof(Record), 1, file);
}

int write_record(FILE *file, const Record *record)
{
    return fwrite(record, sizeof(Record), 1, file) == 1 ? 0 : -1;
}

static size_t get_record_count(FILE *file)
{
    if (fseek(file, 0, SEEK_END) != 0) {
        return 0;
    }

    long file_size = ftell(file);

    if (file_size < 0) {
        return 0;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        return 0;
    }

    return (size_t)file_size / sizeof(Record);
}

int load_records_from_file(
    FILE *file,
    Record **records,
    size_t *count
)
{
    *count = get_record_count(file);

    *records = malloc(*count * sizeof(Record));

    if (*records == NULL && *count != 0) {
        return -1;
    }

    for (size_t i = 0; i < *count; i++) {
        if (read_record(file, &(*records)[i]) != 1) {
            free(*records);
            *records = NULL;
            *count = 0;
            return -1;
        }
    }

    return 0;
}

int save_records_to_file(
    FILE *file,
    const Record *records,
    size_t count
)
{
    for (size_t i = 0; i < count; i++) {
        if (write_record(file, &records[i]) != 0) {
            return -1;
        }
    }

    return 0;
}