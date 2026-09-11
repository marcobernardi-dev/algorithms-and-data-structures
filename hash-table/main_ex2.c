#include "hashtable.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_BUFFER_SIZE 1024

static int compare_strings(const void *a, const void *b)
{
    return strcmp(a, b);
}

static unsigned long hash_string(const void *key)
{
    const unsigned char *str = key;
    unsigned long hash = 5381;

    while (*str != '\0') {
        hash = ((hash << 5) + hash) + *str;
        str++;
    }

    return hash;
}

static void normalize_word(char *word)
{
    size_t start = 0;
    size_t length = strlen(word);

    while (start < length &&
           ispunct((unsigned char)word[start])) {
        start++;
    }

    while (length > start &&
           ispunct((unsigned char)word[length - 1])) {
        length--;
    }

    if (start > 0) {
        memmove(word, word + start, length - start);
    }

    word[length - start] = '\0';

    for (size_t i = 0; word[i] != '\0'; i++) {
        word[i] = (char)tolower((unsigned char)word[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(
            stderr,
            "Uso: %s <file> <lunghezza_minima>\n",
            argv[0]
        );
        return 1;
    }

    char *endptr = NULL;
    long minimum_length = strtol(argv[2], &endptr, 10);

    if (*argv[2] == '\0' ||
        *endptr != '\0' ||
        minimum_length < 0) {
        fprintf(stderr, "Lunghezza minima non valida\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        perror("Errore nell'apertura del file");
        return 1;
    }

    HashTable *table = hash_table_create(
        compare_strings,
        hash_string
    );

    if (table == NULL) {
        fprintf(stderr, "Errore nella creazione della hash table\n");
        fclose(file);
        return 1;
    }

    char word[WORD_BUFFER_SIZE];

    while (fscanf(file, "%1023s", word) == 1) {
        normalize_word(word);

        if (word[0] == '\0' ||
            strlen(word) < (size_t)minimum_length) {
            continue;
        }

        int *frequency = hash_table_get(table, word);

        if (frequency != NULL) {
            (*frequency)++;
        } else {
            char *key = malloc(strlen(word) + 1);
            int *value = malloc(sizeof(int));

            if (key == NULL || value == NULL) {
                free(key);
                free(value);
                hash_table_free(table);
                fclose(file);
                return 1;
            }

            strcpy(key, word);
            *value = 1;

            hash_table_put(table, key, value);
        }
    }

    fclose(file);

    int table_size = hash_table_size(table);

    if (table_size == 0) {
        printf("Nessuna parola trovata.\n");
        hash_table_free(table);
        return 0;
    }

    void **keys = hash_table_keyset(table);

    if (keys == NULL) {
        fprintf(stderr, "Errore nella creazione del keyset\n");
        hash_table_free(table);
        return 1;
    }

    char *most_frequent_word = NULL;
    int maximum_frequency = 0;

    for (int i = 0; i < table_size; i++) {
        char *key = keys[i];
        int *frequency = hash_table_get(table, key);

        if (frequency != NULL &&
            *frequency > maximum_frequency) {
            maximum_frequency = *frequency;
            most_frequent_word = key;
        }
    }

    printf(
        "%s %d\n",
        most_frequent_word,
        maximum_frequency
    );

    free(keys);

    //libera la memoria allocata per le chiavi e i valori nella hash table
    keys = hash_table_keyset(table);

    if (keys != NULL) {
        for (int i = 0; i < table_size; i++) {
            char *key = keys[i];
            int *value = hash_table_get(table, key);

            free(key);
            free(value);
        }

        free(keys);
    }

    hash_table_free(table);

    return 0;
}