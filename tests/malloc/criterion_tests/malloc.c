/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests malloc
*/

#include <malloc.h>
#include <criterion/criterion.h>
#include "successful_alloc.h"
#include <unistd.h>
#include <threads.h>
#include <time.h>
#include <stdint.h>

Test(malloc, very_simple)
{
    void *ptr = malloc(5);

    memset(ptr, 0, 5);
    free(ptr);
}

static void do_simple_oom_test(size_t size)
{
    cr_assert_eq(malloc(size), NULL);
}

Test(malloc, simple_oom)
{
    do_simple_oom_test(SIZE_MAX);

    if (sizeof(ptrdiff_t) < 8)
        return;
    do_simple_oom_test(PTRDIFF_MAX);
    do_simple_oom_test(PTRDIFF_MAX / 2);
    do_simple_oom_test((PTRDIFF_MAX / 2) - 1);
    do_simple_oom_test(PTRDIFF_MAX / 20);
}

Test(malloc, solaris_mtmalloc_alignment)
{
    char *ptrs[20];

    for (size_t i = 0; i < 20; ++i) {
        ptrs[i] = test_successful_malloc(1 << i);
        memset(ptrs[i], 0x5A, 1 << i);
        cr_assert_eq((uintptr_t)ptrs[i] & 7, 0);
    }
    for (size_t i = 0; i < 20; ++i)
        free(ptrs[i]);
}

static int solaris_mtmalloc_thread_health_check_thread_func(void *arg)
{
    (void)arg;
    enum {
        ALLOCATION_COUNT = 25000,
    };
    void *ptrs[ALLOCATION_COUNT];

    for (size_t i = 0; i < ALLOCATION_COUNT; ++i) {
        size_t sz = rand() % 64;
        ptrs[i] = test_successful_malloc(sz);
        memset(ptrs[i], 0x2E, sz);
    }
    for (size_t i = 0; i < ALLOCATION_COUNT; ++i)
        free(ptrs[i]);
    return 0;
}

Test(malloc, solaris_mtmalloc_thread_health_check)
{
    thrd_t threads[512];

    size_t thread_count = sysconf(_SC_NPROCESSORS_CONF);
    cr_assert(thread_count <= 512);
    srand(getpid() ^ time(NULL));

    for (size_t i = 0; i < thread_count; ++i)
        thrd_create(&threads[i], &solaris_mtmalloc_thread_health_check_thread_func, NULL);
    for (size_t i = 0; i < thread_count; ++i)
        thrd_join(threads[i], NULL);
}

Test(malloc, size_0)
{
    free(test_successful_malloc(0));
}

Test(malloc, openbsd_simple_stress)
{
    for (size_t i = 0; i < 2000; ++i) {
        volatile char *ptrs[1000];

        for (size_t j = 0; j < 1000; ++j) {
            size_t size = (random() & 0x7FFFF) + 1;
            ptrs[j] = test_successful_malloc(size);
            ptrs[j][0] = 0;
            ptrs[j][size - 1] = 0;
        }
        for (size_t j = 0; j < 1000; ++j)
            free((void *)ptrs[j]);
    }
}

Test(malloc, bionic_simple)
{
    free(test_successful_malloc(100));
}

Test(malloc, bionic_verify_alignment)
{
    static const size_t LOOP_CNT = 100;

    uint32_t **values_32 = test_successful_malloc(sizeof(uint32_t *) * LOOP_CNT);
    uint64_t **values_64 = test_successful_malloc(sizeof(uint64_t *) * LOOP_CNT);
    long double **values_ldbl = test_successful_malloc(sizeof(long double *) * LOOP_CNT);
    void **filler = test_successful_malloc(sizeof(void *) * LOOP_CNT);

    for (size_t i = 0; i < LOOP_CNT; ++i) {
        filler[i] = test_successful_malloc(i);
        values_32[i] = test_successful_malloc(sizeof(uint32_t));
        *values_32[i] = i;
        cr_assert_eq(*values_32[i], i);
        cr_assert_eq(0, (uintptr_t)values_32[i] & (_Alignof(uint32_t) - 1));
        free(filler[i]);
    }

    for (size_t i = 0; i < LOOP_CNT; ++i) {
        filler[i] = test_successful_malloc(i);
        values_64[i] = test_successful_malloc(sizeof(uint64_t));
        *values_64[i] = 0x1000 + i;
        cr_assert_eq(*values_64[i], 0x1000 + i);
        cr_assert_eq(0, (uintptr_t)values_64[i] & (_Alignof(uint64_t) - 1));
        free(filler[i]);
    }

    for (size_t i = 0; i < LOOP_CNT; ++i) {
        filler[i] = test_successful_malloc(i);
        values_ldbl[i] = test_successful_malloc(sizeof(long double));
        *values_ldbl[i] = 5.5 + i;
        cr_assert_eq(*values_ldbl[i], 5.5 + i);
        cr_assert_eq(0, (uintptr_t)values_ldbl[i] & (_Alignof(long double) - 1));
        free(filler[i]);
    }

    for (size_t i = 0; i < LOOP_CNT; ++i) {
        free(values_32[i]);
        free(values_64[i]);
        free(values_ldbl[i]);
    }

    free(filler);
    free(values_ldbl);
    free(values_64);
    free(values_32);
}
