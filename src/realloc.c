/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines realloc
*/

#include "my_malloc.h"
#include "assert.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void *realloc_innards(void *old_ptr, size_t new_size)
{
    union my_malloc_block *old_block = (union my_malloc_block *)old_ptr - 1;
    void *result;

    MY_MALLOC_ASSERT(old_block->magic_number == MY_MALLOC_MAGIC_NUMBER);
    MY_MALLOC_ASSERT(old_block->is_used == true);
    if (old_block->size >= new_size)
        return (old_ptr);
    my_free_unlocked(old_ptr);
    result = my_malloc_unlocked(new_size);
    if (result != NULL && result != old_ptr)
        memcpy(result, old_ptr, old_block->size < new_size ? old_block->size :
            new_size);
    return (result);
}

void *realloc(void *old_ptr, size_t new_size)
{
    void *result;

    MY_MALLOC_DEBUG_PRINTF("Reallocating to %zu bytes\n", new_size);
    if (old_ptr == NULL)
        return (malloc(new_size));
    if (new_size == 0) {
        free(old_ptr);
        return (NULL);
    }
    pthread_mutex_lock(&g_my_malloc.mutex);
    result = realloc_innards(old_ptr, new_size);
    pthread_mutex_unlock(&g_my_malloc.mutex);
    MY_MALLOC_DEBUG_PRINTF("Realloc returning %p\n", result);
    return (result);
}
