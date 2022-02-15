/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines increase_break
*/

#include "my_malloc.h"
#include "assert.h"
#include <unistd.h>

static bool do_system_increase(ssize_t size)
{
    void *new_break;

    size += g_my_malloc.page_size - (size % g_my_malloc.page_size);
    MY_MALLOC_ASSERT(sbrk(0) == g_my_malloc.system_break);
    MY_MALLOC_DEBUG_PRINTF("Increasing system break by %zd bytes\n", size);
    new_break = sbrk(size);
    MY_MALLOC_DEBUG_PRINTF("New break value: %p\n", new_break);
    if (new_break == (void *)-1)
        return (false);
    g_my_malloc.system_break = (char *)new_break + size;
    return (true);
}

void *my_malloc_increase_break(size_t size)
{
    void *result = g_my_malloc.virtual_break;

    MY_MALLOC_ASSERT((size % 16) == 0);
    MY_MALLOC_DEBUG_PRINTF("Increasing virtual break by %zu bytes\n", size);
    if (g_my_malloc.virtual_break + size < g_my_malloc.virtual_break)
        return (NULL);
    if (g_my_malloc.virtual_break + size > g_my_malloc.system_break)
        if (!do_system_increase(size))
            return (NULL);
    g_my_malloc.virtual_break += size;
    return (result);
}
