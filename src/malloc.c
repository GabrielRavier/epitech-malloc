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

static void do_remove_from_list(union my_malloc_block *result,
    union my_malloc_block **prev_next_location)
{
    if (result != NULL) {
        result->is_used = true;
        MY_MALLOC_ASSERT(*prev_next_location == result);
        *prev_next_location = result->next;
    }
}

static union my_malloc_block *find_best_block(size_t size)
{
    union my_malloc_block *result = NULL;
    union my_malloc_block *prev = NULL;
    union my_malloc_block **prev_next_location = NULL;

    for (union my_malloc_block *it = g_my_malloc.list_head; it != NULL;
         it = it->next) {
        MY_MALLOC_ASSERT(it->magic_number == MY_MALLOC_MAGIC_NUMBER);
        MY_MALLOC_ASSERT(it->is_used == false);
        if (it->size > size &&
            (result == NULL || result->size <= it->size)) {
            prev_next_location = prev ? &prev->next : &g_my_malloc.list_head;
            MY_MALLOC_ASSERT(*prev_next_location == it);
            result = it;
        }
        prev = it;
    }
    do_remove_from_list(result, prev_next_location);
    return (result);
}

void *my_malloc_unlocked(size_t size)
{
    union my_malloc_block *result;

    size = (size + 16) & ~0xF;
    result = find_best_block(size);
    if (result == NULL) {
        result = my_malloc_increase_break(size +
            sizeof(union my_malloc_block));
        MY_MALLOC_ASSERT(((uintptr_t)result %
            16) == 0);
        if (result == NULL)
            return (do_oom_return());
        result->size = size;
        result->magic_number = MY_MALLOC_MAGIC_NUMBER;
        result->is_used = true;
    }
    return (&result[1]);
}

void *malloc(size_t size)
{
    void *result;

    MY_MALLOC_DEBUG_PRINTF("Malloc allocating %zu bytes\n", size);
    pthread_mutex_lock(&g_my_malloc.mutex);
    if (g_my_malloc.page_size == 0 && !my_malloc_initializer())
        return (do_oom_return());
    result = my_malloc_unlocked(size);
    MY_MALLOC_ASSERT(((uintptr_t)result % 16) == 0);
    pthread_mutex_unlock(&g_my_malloc.mutex);
    MY_MALLOC_DEBUG_PRINTF("Malloc returning %p\n", result);
    return (result);
}

