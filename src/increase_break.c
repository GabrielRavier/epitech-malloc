/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines increase_break
*/

#include "my_malloc.h"
#include <unistd.h>

static void set_new_blocks(size_t which_bucket,
    union my_malloc_block *new_blocks, size_t allocated_blocks,
    size_t block_size)
{
    g_my_malloc.free_blocks[which_bucket] = new_blocks;
    while (--allocated_blocks > 0) {
        new_blocks->next_block =
            (union my_malloc_block *)((uintptr_t)new_blocks + block_size);
        new_blocks = new_blocks->next_block;
    }
}

void my_malloc_increase_break(size_t which_bucket)
{
    ssize_t wanted_size = 1 << (which_bucket + 3);
    size_t allocated_size;
    size_t allocated_blocks;
    union my_malloc_block *new_blocks;

    if (wanted_size <= 0)
        return;
    if ((size_t)wanted_size < g_my_malloc.page_size) {
        allocated_size = g_my_malloc.page_size;
        allocated_blocks = allocated_size / wanted_size;
    } else {
        allocated_size = g_my_malloc.page_size + wanted_size;
        allocated_blocks = 1;
    }
    MY_MALLOC_DEBUG_PRINTF("Increasing break by %zu for bucket %zu\n",
        allocated_size, which_bucket);
    new_blocks = sbrk(allocated_size);
    if (new_blocks == (void *)-1)
        return;
    set_new_blocks(which_bucket, new_blocks, allocated_blocks, wanted_size);
}
