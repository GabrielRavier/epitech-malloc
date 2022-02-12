/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines aligned_alloc
*/

#include <stdlib.h>
#include <malloc.h>
#include <errno.h>

void *aligned_alloc(size_t alignment, size_t size)
{
    return (memalign(alignment, size));
}
