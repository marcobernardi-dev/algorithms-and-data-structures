#include "hashtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int compare_strings(const void *a, const void *b)
{
    return strcmp(a, b);
}

static unsigned long hash_string(const void *key)
{
    const char *str = key;
    unsigned long hash = 5381;

    while (*str != '\0') {
        hash = ((hash << 5) + hash) + (unsigned char)*str;
        str++;
    }

    return hash;
}

/*
 * Hash function deliberately creates collisions.
 */
static unsigned long hash_collision(const void *key)
{
    (void)key;
    return 0;
}

static int test_basic_operations(void)
{
    HashTable *table = hash_table_create(
        compare_strings,
        hash_string
    );

    if (table == NULL) {
        return 0;
    }

    int value1 = 10;
    int value2 = 20;

    hash_table_put(table, "one", &value1);
    hash_table_put(table, "two", &value2);

    if (hash_table_size(table) != 2) {
        hash_table_free(table);
        return 0;
    }

    if (!hash_table_contains_key(table, "one") ||
        !hash_table_contains_key(table, "two")) {
        hash_table_free(table);
        return 0;
    }

    int *result = hash_table_get(table, "one");

    if (result == NULL || *result != 10) {
        hash_table_free(table);
        return 0;
    }

    if (hash_table_get(table, "missing") != NULL) {
        hash_table_free(table);
        return 0;
    }

    hash_table_free(table);

    return 1;
}

static int test_update(void)
{
    HashTable *table = hash_table_create(
        compare_strings,
        hash_string
    );

    if (table == NULL) {
        return 0;
    }

    int value1 = 10;
    int value2 = 99;

    hash_table_put(table, "key", &value1);
    hash_table_put(table, "key", &value2);

    if (hash_table_size(table) != 1) {
        hash_table_free(table);
        return 0;
    }

    int *result = hash_table_get(table, "key");

    if (result == NULL || *result != 99) {
        hash_table_free(table);
        return 0;
    }

    hash_table_free(table);

    return 1;
}

static int test_collisions(void)
{
    HashTable *table = hash_table_create(
        compare_strings,
        hash_collision
    );

    if (table == NULL) {
        return 0;
    }

    int value1 = 1;
    int value2 = 2;
    int value3 = 3;

    hash_table_put(table, "first", &value1);
    hash_table_put(table, "second", &value2);
    hash_table_put(table, "third", &value3);

    if (hash_table_size(table) != 3) {
        hash_table_free(table);
        return 0;
    }

    int *result1 = hash_table_get(table, "first");
    int *result2 = hash_table_get(table, "second");
    int *result3 = hash_table_get(table, "third");

    if (result1 == NULL || *result1 != 1 ||
        result2 == NULL || *result2 != 2 ||
        result3 == NULL || *result3 != 3) {
        hash_table_free(table);
        return 0;
    }

    hash_table_remove(table, "second");

    if (hash_table_size(table) != 2 ||
        hash_table_contains_key(table, "second")) {
        hash_table_free(table);
        return 0;
    }

    result1 = hash_table_get(table, "first");
    result3 = hash_table_get(table, "third");

    if (result1 == NULL || *result1 != 1 ||
        result3 == NULL || *result3 != 3) {
        hash_table_free(table);
        return 0;
    }

    hash_table_free(table);

    return 1;
}

static int test_remove(void)
{
    HashTable *table = hash_table_create(
        compare_strings,
        hash_collision
    );

    if (table == NULL) {
        return 0;
    }

    int value1 = 1;
    int value2 = 2;
    int value3 = 3;

    hash_table_put(table, "first", &value1);
    hash_table_put(table, "second", &value2);
    hash_table_put(table, "third", &value3);

    hash_table_remove(table, "third");

    if (hash_table_size(table) != 2 ||
        hash_table_contains_key(table, "third")) {
        hash_table_free(table);
        return 0;
    }

    hash_table_remove(table, "first");

    if (hash_table_size(table) != 1 ||
        hash_table_contains_key(table, "first")) {
        hash_table_free(table);
        return 0;
    }

    hash_table_remove(table, "does-not-exist");

    if (hash_table_size(table) != 1) {
        hash_table_free(table);
        return 0;
    }

    hash_table_free(table);

    return 1;
}

static int test_keyset(void)
{
    HashTable *table = hash_table_create(
        compare_strings,
        hash_string
    );

    if (table == NULL) {
        return 0;
    }

    int value1 = 1;
    int value2 = 2;
    int value3 = 3;

    hash_table_put(table, "alpha", &value1);
    hash_table_put(table, "beta", &value2);
    hash_table_put(table, "gamma", &value3);

    void **keys = hash_table_keyset(table);

    if (keys == NULL) {
        hash_table_free(table);
        return 0;
    }

    int found_alpha = 0;
    int found_beta = 0;
    int found_gamma = 0;

    for (int i = 0; i < hash_table_size(table); i++) {
        if (strcmp(keys[i], "alpha") == 0) {
            found_alpha = 1;
        } else if (strcmp(keys[i], "beta") == 0) {
            found_beta = 1;
        } else if (strcmp(keys[i], "gamma") == 0) {
            found_gamma = 1;
        }
    }

    free(keys);
    hash_table_free(table);

    return found_alpha && found_beta && found_gamma;
}

static int test_empty_table(void)
{
    HashTable *table = hash_table_create(
        compare_strings,
        hash_string
    );

    if (table == NULL) {
        return 0;
    }

    if (hash_table_size(table) != 0 ||
        hash_table_contains_key(table, "missing") ||
        hash_table_get(table, "missing") != NULL ||
        hash_table_keyset(table) != NULL) {
        hash_table_free(table);
        return 0;
    }

    hash_table_remove(table, "missing");

    hash_table_free(table);

    return 1;
}

int main(void)
{
    int passed = 0;
    int total = 6;

    if (test_basic_operations()) {
        printf("PASS: operazioni di base\n");
        passed++;
    } else {
        printf("FAIL: operazioni di base\n");
    }

    if (test_update()) {
        printf("PASS: aggiornamento valore\n");
        passed++;
    } else {
        printf("FAIL: aggiornamento valore\n");
    }

    if (test_collisions()) {
        printf("PASS: collisioni\n");
        passed++;
    } else {
        printf("FAIL: collisioni\n");
    }

    if (test_remove()) {
        printf("PASS: remove\n");
        passed++;
    } else {
        printf("FAIL: remove\n");
    }

    if (test_keyset()) {
        printf("PASS: keyset\n");
        passed++;
    } else {
        printf("FAIL: keyset\n");
    }

    if (test_empty_table()) {
        printf("PASS: tabella vuota\n");
        passed++;
    } else {
        printf("FAIL: tabella vuota\n");
    }

    printf("\nRisultato: %d/%d test superati.\n", passed, total);

    return passed == total ? 0 : 1;
}