/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines memalign
*/

#include "my_malloc.h"
#include "assert.h"
#include <malloc.h>
#include <string.h>
#include <errno.h>

static void *do_align(char *malloc_result, size_t alignment, size_t size)
{
    char *aligned_pointer =
        (void *)(((uintptr_t)malloc_result + alignment) & ~(alignment - 1));
    union my_malloc_block *resulting_block =
        (union my_malloc_block *)aligned_pointer - 1;

    memmove(resulting_block, malloc_result - sizeof(union my_malloc_block),
        sizeof(union my_malloc_block));
    resulting_block->bucket_index = my_malloc_compute_used_bucket(size);
    MY_MALLOC_ASSERT(resulting_block->bucket_index != (uint8_t)(size_t)-1);
    return (aligned_pointer);
}

// Allocates just enough memory to guarantee alignment, then moves the malloc
// block just before the aligned memory so that everything works properly...
void *memalign(size_t alignment, size_t size)
{
    char *malloc_result;

    if (alignment == 0 || alignment & (alignment - 1)) {
        errno = EINVAL;
        return (NULL);
    }
    if (alignment < sizeof(union my_malloc_block) * 2)
        alignment = sizeof(union my_malloc_block) * 2;
    malloc_result = malloc(alignment + size);
    if (malloc_result == NULL)
        return (NULL);
    return (do_align(malloc_result, alignment, size));
}