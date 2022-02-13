/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines increase_break
*/

#include "my_malloc.h"
#include "assert.h"
#include <unistd.h>

static char *align_to_page_size(char *ptr)
{
    return ptr +
        (((((~(uintptr_t) ptr) + 1) % g_my_malloc.page_size) +
            g_my_malloc.page_size));
}

static bool do_system_increase(size_t size)
{
    char *wanted_new_break =
        align_to_page_size(g_my_malloc.virtual_break + size);

    MY_MALLOC_ASSERT(sbrk(0) == g_my_malloc.system_break);
    MY_MALLOC_ASSERT(
        ((uintptr_t)wanted_new_break % g_my_malloc.page_size) == 0);
    MY_MALLOC_ASSERT(wanted_new_break - g_my_malloc.system_break > 0);
    MY_MALLOC_DEBUG_PRINTF("Increasing system break by %td bytes\n",
        wanted_new_break - g_my_malloc.system_break);
    if (brk(wanted_new_break) == -1)
        return (false);
    MY_MALLOC_ASSERT(g_my_malloc.system_break < wanted_new_break);
    g_my_malloc.system_break = wanted_new_break;
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
