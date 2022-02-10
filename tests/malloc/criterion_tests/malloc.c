/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests malloc
*/

#include <stdlib.h>
#include <criterion/criterion.h>
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

Test(malloc, very_simple_oom)
{
    if (sizeof(ptrdiff_t) < 8)
        return;
    void *ptr = malloc(PTRDIFF_MAX);

    cr_assert_eq(ptr, NULL);
    free(ptr);
}

Test(malloc, very_simple_oom_2)
{
    if (sizeof(ptrdiff_t) < 8)
        return;
    void *ptr = malloc(PTRDIFF_MAX / 2);

    cr_assert_eq(ptr, NULL);
    free(ptr);
}

Test(malloc, solaris_mtmalloc_alignment)
{
    char *ptrs[20];

    for (size_t i = 0; i < 20; ++i) {
        ptrs[i] = malloc(1 << i);
        memset(ptrs[i], 0x5A, 1 << i);
        cr_assert_eq((uintptr_t)ptrs[i] & 7, 0);
    }
    for (size_t i = 0; i < 20; ++i) {
        free(ptrs[i]);
    }
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
        ptrs[i] = malloc(sz);
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
