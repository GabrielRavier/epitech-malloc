/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines calloc
*/

#include "my_malloc.h"
#include <malloc.h>
#include <string.h>
#include <errno.h>

void *calloc(size_t num_elements, size_t element_size)
{
    void *result;

    MY_MALLOC_DEBUG_PRINTF("calloc allocating %zu\n",
        num_elements * element_size);
    result = reallocarray(NULL, num_elements, element_size);
    if (result != NULL)
        memset(result, 0, malloc_usable_size(result));
    return result;
}
