#include "hashtable.h"

#include <stdlib.h>

#define INITIAL_CAPACITY 1024
#define MAX_LOAD_FACTOR 0.75

typedef struct HashNode {
    void *key;
    void *value;
    struct HashNode *next;
} HashNode;

struct HashTable {
    HashNode **buckets;
    size_t capacity;
    size_t size;
    int (*compare)(const void *, const void *);
    unsigned long (*hash)(const void *);
};

static size_t get_index(
    const HashTable *table,
    const void *key
)
{
    return table->hash(key) % table->capacity;
}

static HashNode *find_node(
    const HashTable *table,
    const void *key
)
{
    size_t index = get_index(table, key);
    HashNode *current = table->buckets[index];

    while (current != NULL) {
        if (table->compare(current->key, key) == 0) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

static int resize_table(
    HashTable *table,
    size_t new_capacity
)
{
    HashNode **new_buckets = calloc(
        new_capacity,
        sizeof(HashNode *)
    );

    if (new_buckets == NULL) {
        return 0;
    }

    for (size_t i = 0; i < table->capacity; i++) {
        HashNode *current = table->buckets[i];

        while (current != NULL) {
            HashNode *next = current->next;

            size_t index =
                table->hash(current->key) % new_capacity;

            current->next = new_buckets[index];
            new_buckets[index] = current;

            current = next;
        }
    }

    free(table->buckets);

    table->buckets = new_buckets;
    table->capacity = new_capacity;

    return 1;
}

HashTable *hash_table_create(
    int (*f1)(const void *, const void *),
    unsigned long (*f2)(const void *)
)
{
    if (f1 == NULL || f2 == NULL) {
        return NULL;
    }

    HashTable *table = malloc(sizeof(HashTable));

    if (table == NULL) {
        return NULL;
    }

    table->buckets = calloc(
        INITIAL_CAPACITY,
        sizeof(HashNode *)
    );

    if (table->buckets == NULL) {
        free(table);
        return NULL;
    }

    table->capacity = INITIAL_CAPACITY;
    table->size = 0;
    table->compare = f1;
    table->hash = f2;

    return table;
}

void hash_table_put(
    HashTable *table,
    const void *key,
    const void *value
)
{
    if (table == NULL || key == NULL) {
        return;
    }

    HashNode *existing = find_node(table, key);

    if (existing != NULL) {
        existing->value = (void *)value;
        return;
    }

    if ((double)(table->size + 1) / table->capacity >
        MAX_LOAD_FACTOR) {

        if (!resize_table(table, table->capacity * 2)) {
            return;
        }
    }

    size_t index = get_index(table, key);

    HashNode *node = malloc(sizeof(HashNode));

    if (node == NULL) {
        return;
    }

    node->key = (void *)key;
    node->value = (void *)value;
    node->next = table->buckets[index];

    table->buckets[index] = node;
    table->size++;
}

void *hash_table_get(
    const HashTable *table,
    const void *key
)
{
    if (table == NULL || key == NULL) {
        return NULL;
    }

    HashNode *node = find_node(table, key);

    if (node == NULL) {
        return NULL;
    }

    return node->value;
}

int hash_table_contains_key(
    const HashTable *table,
    const void *key
)
{
    if (table == NULL || key == NULL) {
        return 0;
    }

    return find_node(table, key) != NULL;
}

void hash_table_remove(
    HashTable *table,
    const void *key
)
{
    if (table == NULL || key == NULL) {
        return;
    }

    size_t index = get_index(table, key);

    HashNode *current = table->buckets[index];
    HashNode *previous = NULL;

    while (current != NULL) {
        if (table->compare(current->key, key) == 0) {

            if (previous == NULL) {
                table->buckets[index] = current->next;
            } else {
                previous->next = current->next;
            }

            free(current);
            table->size--;

            return;
        }

        previous = current;
        current = current->next;
    }
}

int hash_table_size(const HashTable *table)
{
    if (table == NULL) {
        return 0;
    }

    return (int)table->size;
}

void **hash_table_keyset(const HashTable *table)
{
    if (table == NULL || table->size == 0) {
        return NULL;
    }

    void **keys = malloc(
        table->size * sizeof(void *)
    );

    if (keys == NULL) {
        return NULL;
    }

    size_t position = 0;

    for (size_t i = 0; i < table->capacity; i++) {
        HashNode *current = table->buckets[i];

        while (current != NULL) {
            keys[position] = current->key;
            position++;

            current = current->next;
        }
    }

    return keys;
}

void hash_table_free(HashTable *table)
{
    if (table == NULL) {
        return;
    }

    for (size_t i = 0; i < table->capacity; i++) {
        HashNode *current = table->buckets[i];

        while (current != NULL) {
            HashNode *next = current->next;

            free(current);
            current = next;
        }
    }

    free(table->buckets);
    free(table);
}