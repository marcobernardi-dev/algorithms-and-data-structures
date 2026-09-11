#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct HashTable HashTable;

HashTable* hash_table_create(
    int (*f1)(const void*, const void*),
    unsigned long (*f2)(const void*)
);

void hash_table_put(
    HashTable*,
    const void*,
    const void*
);

void* hash_table_get(
    const HashTable*,
    const void*
);

int hash_table_contains_key(
    const HashTable*,
    const void*
);

void hash_table_remove(
    HashTable*,
    const void*
);

int hash_table_size(
    const HashTable*
);

void** hash_table_keyset(
    const HashTable*
);

void hash_table_free(
    HashTable*
);

#endif