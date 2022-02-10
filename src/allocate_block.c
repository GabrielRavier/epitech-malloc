/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines allocate_block
*/

#include "my_malloc.h"
#include <unistd.h>

// This simply allocates a new block and puts it on the front of the
// corresponding bucket's free list
void my_malloc_allocate_block(size_t which_bucket)
{
    ssize_t wanted_size = (1 << (which_bucket + 4));
    union my_malloc_block *new_block;

    if (wanted_size <= 0)
        return;
    new_block = my_malloc_increase_break(wanted_size);
    if (new_block == (void *)-1)
        return;
    new_block->next_block = g_my_malloc.free_blocks[which_bucket];
    g_my_malloc.free_blocks[which_bucket] = new_block;
}
