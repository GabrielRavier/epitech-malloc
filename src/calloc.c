/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines calloc
*/

#include <stdlib.h>
#include <string.h>
#include <errno.h>

void *calloc(size_t num_elements, size_t element_size)
{
    size_t malloced_size;
    void *result;

    if (__builtin_mul_overflow(num_elements, element_size, &malloced_size)) {
        errno = ENOMEM;
        return NULL;
    }
    result = malloc(malloced_size);
    if (result != NULL)
        memset(result, 0, malloced_size);
    return result;
}
