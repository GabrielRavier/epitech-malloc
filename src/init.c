/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines init
*/

#include "my_malloc.h"
#include "assert.h"
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

struct g_my_malloc_type g_my_malloc = {
    .page_size = 0,
    .mutex = PTHREAD_MUTEX_INITIALIZER
};

// This is called the first time malloc is called. It sets up the page size and
// all other needed global data, along with aligning the break such that all
// data is page aligned
// Actually, the subject made me lie on the comment from before saying that the
// page size is getpagesize(). It actually has to be getpagesize() * 2 because
// the break must always be aligned on a multiple of two pages...
bool my_malloc_initializer()
{
    g_my_malloc.page_size = getpagesize() * 2;
    g_my_malloc.system_break = sbrk(0);
    g_my_malloc.virtual_system_break =
        (char *)(((uintptr_t)g_my_malloc.system_break + 16) & ~0xF);
    return (true);
}
