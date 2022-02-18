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

static void compute_sizes(size_t bucket, size_t *lower_size,
    size_t *higher_size)
{
    *lower_size = 1 << (bucket + 3);
    if (*lower_size < g_my_malloc.page_size)
        *lower_size -= sizeof(union my_malloc_block);
    else
        *lower_size += g_my_malloc.page_size - sizeof(union my_malloc_block);
    if (bucket) {
        *higher_size = 1 << (bucket + 2);
        if (*higher_size < g_my_malloc.page_size)
            *higher_size -= sizeof(union my_malloc_block);
        else
            *higher_size +=
                g_my_malloc.page_size - sizeof(union my_malloc_block);
    }
}

// This makes sure to avoid the copy when the block size would be the same
// anyway
// We make that check for (new_size > higher_size) neutralized to obey the best
// fit demands of the subject
static void *realloc_innards(void *old_ptr, size_t new_size)
{
    union my_malloc_block *old_block = (union my_malloc_block *)old_ptr - 1;
    size_t bucket = old_block->bucket_index;
    size_t lower_size;
    size_t higher_size = 0;
    void *result;

    MY_MALLOC_ASSERT(old_block->magic_number == MY_MALLOC_MAGIC_NUMBER);
    compute_sizes(bucket, &lower_size, &higher_size);
    if ((new_size <= lower_size) && (new_size > higher_size || true))
        return (old_ptr);
    result = my_malloc_unlocked(new_size);
    if (result != NULL) {
        memcpy(result, old_ptr,
            (lower_size < new_size) ? lower_size : new_size);
        my_free_unlocked(old_ptr);
    }
    return (result);
}

// Yes, I know new_size == 0 to new_size = 1 is stupid... but it's
// mandated by Marvin
void *realloc(void *old_ptr, size_t new_size)
{
    void *result;

    MY_MALLOC_DEBUG_PRINTF("Reallocating to %zu bytes\n", new_size);
    if (old_ptr == NULL)
        return (malloc(new_size));
    if (new_size == 0)
        new_size = 1;
    pthread_mutex_lock(&g_my_malloc.mutex);
    result = realloc_innards(old_ptr, new_size);
    pthread_mutex_unlock(&g_my_malloc.mutex);
    MY_MALLOC_DEBUG_PRINTF("Realloc returning %p\n", result);
    return (result);
}
