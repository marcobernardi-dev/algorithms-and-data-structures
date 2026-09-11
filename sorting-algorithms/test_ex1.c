#include "hybridsort.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

static int compare_int(const void *a, const void *b)
{
    const int *ia = a;
    const int *ib = b;

    if (*ia < *ib) {
        return -1;
    }

    if (*ia > *ib) {
        return 1;
    }

    return 0;
}

static int compare_double(const void *a, const void *b)
{
    const double *da = a;
    const double *db = b;

    if (*da < *db) {
        return -1;
    }

    if (*da > *db) {
        return 1;
    }

    return 0;
}

static int compare_uint64(const void *a, const void *b)
{
    const uint64_t *ua = a;
    const uint64_t *ub = b;

    if (*ua < *ub) {
        return -1;
    }

    if (*ua > *ub) {
        return 1;
    }

    return 0;
}

static int compare_float(const void *a, const void *b)
{
    const float *fa = a;
    const float *fb = b;

    if (*fa < *fb) {
        return -1;
    }

    if (*fa > *fb) {
        return 1;
    }

    return 0;
}

static int compare_int64(const void *a, const void *b)
{
    const int64_t *ia = a;
    const int64_t *ib = b;

    if (*ia < *ib) {
        return -1;
    }

    if (*ia > *ib) {
        return 1;
    }

    return 0;
}

static int compare_string(const void *a, const void *b)
{
    const char *sa = a;
    const char *sb = b;

    return strcmp(sa, sb);
}

int main(void)
{
    int array[] = {5, 2, 8, 1, 4};
    hybridsort(
        array,
        5,
        sizeof(int),
        2,
        compare_int
    );

    assert(array[0] == 1);
    assert(array[1] == 2);
    assert(array[2] == 4);
    assert(array[3] == 5);
    assert(array[4] == 8);


    int sorted[] = {1, 2, 3, 4, 5};
    hybridsort(
        sorted,
        5,
        sizeof(int),
        2,
        compare_int
    );
    assert(sorted[0] == 1);
    assert(sorted[1] == 2);
    assert(sorted[2] == 3);
    assert(sorted[3] == 4);
    assert(sorted[4] == 5);


    int reversed[] = {5, 4, 3, 2, 1};
    hybridsort(
        reversed,
        5,
        sizeof(int),
        2,
        compare_int
    );
    assert(reversed[0] == 1);
    assert(reversed[1] == 2);
    assert(reversed[2] == 3);
    assert(reversed[3] == 4);
    assert(reversed[4] == 5);


    int duplicates[] = {4, 2, 4, 1, 2, 4, 3};
    hybridsort(
        duplicates,
        7,
        sizeof(int),
        2,
        compare_int
    );
    assert(duplicates[0] == 1);
    assert(duplicates[1] == 2);
    assert(duplicates[2] == 2);
    assert(duplicates[3] == 3);
    assert(duplicates[4] == 4);
    assert(duplicates[5] == 4);
    assert(duplicates[6] == 4);

    int many_duplicates[] = {
        5, 5, 5, 5, 5,
        3, 8, 5, 2, 5,
        7, 5, 1, 5, 6,
        5, 5, 4, 5, 9
    };
    hybridsort(
        many_duplicates,
        20,
        sizeof(int),
        2,
        compare_int
    );
    int expected[] = {
        1, 2, 3, 4,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        6, 7, 8, 9
    };
    for (size_t i = 0; i < 20; i++) {
        assert(many_duplicates[i] == expected[i]);
    }


    int single[] = {42};
    hybridsort(
        single,
        1,
        sizeof(int),
        2,
        compare_int
    );
    assert(single[0] == 42);


    //empty array   
    hybridsort(
        NULL,
        0,
        sizeof(int),
        2,
        compare_int
    );


    int small[] = {5, 2, 8, 1, 4};
    hybridsort(
        small,
        5,
        sizeof(int),
        10,
        compare_int
    );  
    assert(small[0] == 1);
    assert(small[1] == 2);
    assert(small[2] == 4);
    assert(small[3] == 5);
    assert(small[4] == 8);


    int k_one[] = {7, 3, 9, 1, 5};

    hybridsort(
        k_one,
        5,
        sizeof(int),
        1,
        compare_int
    );
    assert(k_one[0] == 1);
    assert(k_one[1] == 3);
    assert(k_one[2] == 5);
    assert(k_one[3] == 7);
    assert(k_one[4] == 9);


    double doubles[] = {3.14, 1.5, 9.2, 2.7, 1.5};
    hybridsort(
        doubles,
        5,
        sizeof(double),
        2,
        compare_double
    );
    assert(doubles[0] == 1.5);
    assert(doubles[1] == 1.5);
    assert(doubles[2] == 2.7);
    assert(doubles[3] == 3.14);
    assert(doubles[4] == 9.2);


    uint64_t ids[] = {
        4000000000ULL,
        12ULL,
        9999999999ULL,
        42ULL,
        12ULL
    };
    hybridsort(
        ids,
        5,
        sizeof(uint64_t),
        2,
        compare_uint64
    );
    assert(ids[0] == 12ULL);
    assert(ids[1] == 12ULL);
    assert(ids[2] == 42ULL);
    assert(ids[3] == 4000000000ULL);
    assert(ids[4] == 9999999999ULL);


    float values[] = {3.14f, 1.5f, 9.2f, 2.7f, 1.5f};
    hybridsort(
        values,
        5,
        sizeof(float),
        2,
        compare_float
    );
    assert(values[0] == 1.5f);
    assert(values[1] == 1.5f);
    assert(values[2] == 2.7f);
    assert(values[3] == 3.14f);
    assert(values[4] == 9.2f);


    int64_t values64[] = {
        3000000000LL,
        -42LL,
        17LL,
        -10000000000LL,
        17LL
    };
    hybridsort(
        values64,
        5,
        sizeof(int64_t),
        2,
        compare_int64
    );
    assert(values64[0] == -10000000000LL);
    assert(values64[1] == -42LL);
    assert(values64[2] == 17LL);
    assert(values64[3] == 17LL);
    assert(values64[4] == 3000000000LL);


    char words[][20] = {
        "banana",
        "apple",
        "pear",
        "cane",
        "casa"
    };
    hybridsort(
        words,
        5,
        sizeof(words[0]),
        2,
        compare_string
    );
    assert(strcmp(words[0], "apple") == 0);
    assert(strcmp(words[1], "banana") == 0);
    assert(strcmp(words[2], "cane") == 0);
    assert(strcmp(words[3], "casa") == 0);
    assert(strcmp(words[4], "pear") == 0);
}