/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines allocate_block
*/

#include "my_malloc.h"
#include <unistd.h>

static size_t align_to_16(size_t size)
{
    size += 16;
    return (size - (size % 16));
}

// This simply allocates a new block and puts it on the front of the
// corresponding bucket's free list
// We do this mess with alignment stuff to ensure that all returned pointers are aligned to 16
void my_malloc_allocate_block(size_t which_bucket)
{
    size_t added_size_for_alignment = 16 - (sizeof(void *));
    ssize_t wanted_size = align_to_16((1ull << (which_bucket + 4)) +
        added_size_for_alignment);
    union my_malloc_block *new_block;

    if (wanted_size <= 0)
        return;
    new_block = (union my_malloc_block *)((char *)my_malloc_increase_break(
        wanted_size) + added_size_for_alignment);
    if (new_block == (void *)-1)
        return;
    new_block->next_block = g_my_malloc.free_blocks[which_bucket];
    g_my_malloc.free_blocks[which_bucket] = new_block;
}
