/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines init
*/

#include "my_malloc.h"
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

struct g_my_malloc_type g_my_malloc = {
    .page_size = 0,
    .above_page_size_bucket = 0,
    .free_blocks = {}
};

// This is called the first time malloc is called. It sets up the page size and
// all other needed global data, along with aligning the break such that all
// data is page aligned
// Actually, the subject made me lie on the comment from before saying that the
// page size is getpagesize(). It actually has to be getpagesize() * 2 because
// the break must always be aligned on a multiple of two pages...
bool my_malloc_internal_initializer()
{
    union my_malloc_block *first_block = sbrk(0);
    ssize_t sbrked_size;
    size_t current_page_size_bucket_size = 8;

    g_my_malloc.page_size = getpagesize() * 2;
    sbrked_size = g_my_malloc.page_size - sizeof(union my_malloc_block) -
        ((intptr_t)first_block & (g_my_malloc.page_size - 1));
    if (sbrked_size < 0)
        sbrked_size += g_my_malloc.page_size;
    if (sbrked_size != 0)
        if (sbrk(sbrked_size) == (void *)-1) {
            g_my_malloc.page_size = 0;
            return (false);
        }
    while (g_my_malloc.page_size > current_page_size_bucket_size) {
        current_page_size_bucket_size <<= 1;
        ++g_my_malloc.above_page_size_bucket;
    }
    return (true);
}
