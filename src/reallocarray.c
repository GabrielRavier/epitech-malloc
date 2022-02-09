/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines reallocarray
*/

#include <stdlib.h>
#include <errno.h>

void *reallocarray(void *old_ptr, size_t num_elements, size_t element_size)
{
    size_t realloced_size;

    if (__builtin_mul_overflow(num_elements, element_size, &realloced_size)) {
        errno = ENOMEM;
        return NULL;
    }
    return realloc(old_ptr, realloced_size);
}
