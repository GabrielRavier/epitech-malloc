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

static void free_innards(union my_malloc_block *freed_block)
{
    uint8_t bucket = freed_block->bucket_index;

    MY_MALLOC_ASSERT(freed_block->magic_number == MY_MALLOC_MAGIC_NUMBER);
    MY_MALLOC_ASSERT(bucket < MY_MALLOC_TOTAL_BUCKET_COUNT);
    freed_block->next_block = g_my_malloc.free_blocks[bucket];
    g_my_malloc.free_blocks[bucket] = freed_block;
}

void free(void *malloced_ptr)
{
    if (malloced_ptr == NULL)
        return;
    free_innards((union my_malloc_block *)malloced_ptr - 1);
}
