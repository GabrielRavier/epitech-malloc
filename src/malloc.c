/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines malloc
*/

#include "my_malloc.h"
#include "assert.h"
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

static void *do_oom_return(void)
{
    errno = ENOMEM;
    return (NULL);
}

static size_t best_fit(size_t starting_bucket)
{
    for (size_t i = starting_bucket; i != MY_MALLOC_TOTAL_BUCKET_COUNT; ++i)
        if (g_my_malloc.free_blocks[i] != NULL)
            return (i);
    return (starting_bucket);
}

static void *alloc_from_bucket(size_t used_bucket,
    __attribute__((unused)) size_t size)
{
    union my_malloc_block *result_block;

    used_bucket = best_fit(used_bucket);
    MY_MALLOC_DEBUG_PRINTF("Allocating %zu bytes from bucket %zu\n", size,
        used_bucket);
    result_block = g_my_malloc.free_blocks[used_bucket];
    if (result_block == NULL) {
        my_malloc_allocate_block(used_bucket);
        result_block = g_my_malloc.free_blocks[used_bucket];
        if (result_block == NULL)
            return (do_oom_return());
    }
    g_my_malloc.free_blocks[used_bucket] = result_block->next_block;
    result_block->magic_number = MY_MALLOC_MAGIC_NUMBER;
    result_block->bucket_index = used_bucket;
    return &result_block[1];
}

void *my_malloc_unlocked(size_t size)
{
    ssize_t full_page_allocation_size = g_my_malloc.page_size -
        sizeof(union my_malloc_block);
    size_t used_bucket;
    size_t bucket_size;

    if (size <= (size_t)full_page_allocation_size) {
        used_bucket = 0;
        bucket_size = 8;
        full_page_allocation_size = -(sizeof(union my_malloc_block));
    } else {
        used_bucket = g_my_malloc.above_page_size_bucket;
        bucket_size = g_my_malloc.page_size;
    }
    while (size > bucket_size + full_page_allocation_size) {
        bucket_size <<= 1;
        if (bucket_size == 0)
            return (do_oom_return());
        ++used_bucket;
    }
    return (alloc_from_bucket(used_bucket, size));
}

void *malloc(size_t size)
{
    void *result;

    MY_MALLOC_DEBUG_PRINTF("Malloc allocating %zu bytes\n", size);
    pthread_mutex_lock(&g_my_malloc.mutex);
    if (g_my_malloc.page_size == 0 && !my_malloc_initializer())
        return (do_oom_return());
    result = my_malloc_unlocked(size);
    MY_MALLOC_ASSERT(((uintptr_t)result % 8) == 0);
    pthread_mutex_unlock(&g_my_malloc.mutex);
    MY_MALLOC_DEBUG_PRINTF("Malloc returning %p\n", result);
    return (result);
}

