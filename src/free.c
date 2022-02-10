/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines free
*/

#include "my_malloc.h"
#include "assert.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void my_free_unlocked(void *malloced_ptr)
{
    union my_malloc_block *freed_block = (union my_malloc_block *)malloced_ptr - 1;
    uint8_t bucket = freed_block->bucket_index;

    MY_MALLOC_DEBUG_PRINTF("Freeing %p from bucket %u\n", malloced_ptr, bucket);
    MY_MALLOC_ASSERT(freed_block->magic_number == MY_MALLOC_MAGIC_NUMBER);
    MY_MALLOC_ASSERT(bucket < MY_MALLOC_TOTAL_BUCKET_COUNT);
    freed_block->next_block = g_my_malloc.free_blocks[bucket];
    g_my_malloc.free_blocks[bucket] = freed_block;
}

void free(void *malloced_ptr)
{
    if (malloced_ptr == NULL)
        return;
    pthread_mutex_lock(&g_my_malloc.mutex);
    my_free_unlocked(malloced_ptr);
    pthread_mutex_unlock(&g_my_malloc.mutex);
}
