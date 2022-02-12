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

// Since we've allocated alignment + size, we always have enough space for the
// alignment to take place. We then move the old malloc data right below the
// aligned memory (and re-compute which bucket it should go into as it the
// memory block might now be much smaller than malloc thought and thus
// belonging in a different bucket
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
// We force alignment to sizeof(union my_malloc_block) * 2 juuust to be safe
void *memalign(size_t alignment, size_t size)
{
    char *malloc_result;
    size_t malloced_size;

    if (alignment == 0 || alignment & (alignment - 1)) {
        errno = EINVAL;
        return (NULL);
    }
    if (alignment < sizeof(union my_malloc_block) * 2)
        alignment = sizeof(union my_malloc_block) * 2;
    if (__builtin_add_overflow(alignment, size, &malloced_size)) {
        errno = ENOMEM;
        return (NULL);
    }
    malloc_result = malloc(malloced_size);
    if (malloc_result == NULL)
        return (NULL);
    return (do_align(malloc_result, alignment, size));
}
