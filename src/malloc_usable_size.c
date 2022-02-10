#include "my_malloc.h"
#include <malloc.h>

size_t malloc_usable_size(void *ptr)
{
    union my_malloc_block *block;
    size_t result;

    if (ptr == NULL)
        return (0);
    block = (union my_malloc_block *)ptr - 1;
    result = 1 << (block->bucket_index + 3);
    if (result < g_my_malloc.page_size)
        result -= sizeof(union my_malloc_block);
    else
        result += g_my_malloc.page_size - sizeof(union my_malloc_block);
    return (result);
}
