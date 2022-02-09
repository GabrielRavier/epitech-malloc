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
    union my_malloc_block *freed_block =
        (union my_malloc_block *)malloced_ptr - 1;

    MY_MALLOC_DEBUG_PRINTF("Freeing %zu bytes from %p\n", freed_block->size,
        malloced_ptr);
    MY_MALLOC_ASSERT(freed_block->magic_number == MY_MALLOC_MAGIC_NUMBER);
    MY_MALLOC_ASSERT(freed_block->is_used == true);
    freed_block->is_used = false;
    freed_block->next = g_my_malloc.list_head;
    g_my_malloc.list_head = freed_block;
}

void free(void *malloced_ptr)
{
    if (malloced_ptr == NULL)
        return;
    pthread_mutex_lock(&g_my_malloc.mutex);
    my_free_unlocked(malloced_ptr);
    pthread_mutex_unlock(&g_my_malloc.mutex);
}
