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
    return ptr + (((((~(uintptr_t)ptr) + 1) % g_my_malloc.page_size) +
        g_my_malloc.page_size));
}

void *my_malloc_increase_break(size_t size)
{
    char *wanted_new_break;
    void *result = g_my_malloc.virtual_system_break;

    MY_MALLOC_DEBUG_PRINTF("Increasing virtual break by %zu bytes\n", size);
    if (g_my_malloc.virtual_system_break + size > g_my_malloc.system_break) {
        wanted_new_break =
            align_to_page_size(g_my_malloc.virtual_system_break + size);
        MY_MALLOC_ASSERT(((uintptr_t)wanted_new_break %
            g_my_malloc.page_size) == 0);
        MY_MALLOC_DEBUG_PRINTF("Increasing system break by %td bytes\n",
            wanted_new_break - g_my_malloc.system_break);
        if (brk(wanted_new_break) == -1)
            return (NULL);
        MY_MALLOC_ASSERT(g_my_malloc.system_break < wanted_new_break);
        g_my_malloc.system_break = wanted_new_break;
    }
    g_my_malloc.virtual_system_break += size;
    return (result);
}
