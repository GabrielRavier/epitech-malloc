/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines pvalloc
*/

#include <unistd.h>
#include <malloc.h>
#include <errno.h>

void *pvalloc(size_t size)
{
    size_t page_size = sysconf(_SC_PAGESIZE);
    size_t rounded_size;

    if (__builtin_add_overflow(size, page_size - 1, &rounded_size)) {
        errno = ENOMEM;
        return (NULL);
    }
    rounded_size = rounded_size & -(page_size - 1);
    return (memalign(page_size, rounded_size));
}
