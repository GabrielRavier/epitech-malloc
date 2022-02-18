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
// We do this mess with alignment stuff to ensure that all returned pointers are
// aligned to 16
void my_malloc_allocate_block(size_t which_bucket)
{
    ssize_t added_size_for_alignment = 16 - sizeof(union my_malloc_block);
    ssize_t wanted_size = align_to_16((1ull << (which_bucket + 4)) +
        added_size_for_alignment);
    union my_malloc_block *new_block;

    if (added_size_for_alignment < 0)
        added_size_for_alignment = 0;
    if (wanted_size <= 0)
        return;
    new_block = my_malloc_increase_break(wanted_size);
    if (new_block == NULL)
        return;
    new_block = (union my_malloc_block *)
        ((char *)new_block + added_size_for_alignment);
    new_block->next_block = g_my_malloc.free_blocks[which_bucket];
    g_my_malloc.free_blocks[which_bucket] = new_block;
}
