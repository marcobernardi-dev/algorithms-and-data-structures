#include <stdio.h>
#include <stdlib.h>

#include "hybridsort.h"
#include "record.h"

void sort_records(
    FILE *infile,
    FILE *outfile,
    size_t field,
    size_t k
)
{
    Record *records = NULL;
    size_t count = 0;

    if (load_records_from_file(infile, &records, &count) != 0) {
        fprintf(stderr, "Errore nella lettura dei record\n");
        return;
    }

    int (*compar)(const void *, const void *) = NULL;

    switch (field) {
        case 1:
            compar = compare_id;
            break;

        case 2:
            compar = compare_field1;
            break;

        case 3:
            compar = compare_field2;
            break;

        case 4:
            compar = compare_field3;
            break;

        default:
            fprintf(stderr, "Campo non valido\n");
            free(records);
            return;
    }

    hybridsort(
        records,
        count,
        sizeof(Record),
        k,
        compar
    );

    if (save_records_to_file(outfile, records, count) != 0) {
        fprintf(stderr, "Errore nella scrittura dei record\n");
    }

    free(records);
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        fprintf(
            stderr,
            "Uso: %s <input_file> <output_file> <field> <k>\n",
            argv[0]
        );
        return 1;
    }

    size_t field = (size_t)strtoul(argv[3], NULL, 10);
    size_t k = (size_t)strtoul(argv[4], NULL, 10);

    FILE *infile = fopen(argv[1], "rb");

    if (infile == NULL) {
        perror("Errore nell'apertura del file di input");
        return 1;
    }

    FILE *outfile = fopen(argv[2], "wb");

    if (outfile == NULL) {
        perror("Errore nell'apertura del file di output");
        fclose(infile);
        return 1;
    }

    sort_records(infile, outfile, field, k);

    fclose(outfile);
    fclose(infile);

    return 0;
}