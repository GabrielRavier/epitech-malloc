/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests pvalloc
*/

#include <malloc.h>
#include <criterion/criterion.h>
#include "successful_alloc.h"

#pragma GCC diagnostic ignored "-Walloc-size-larger-than=0"

Test(pvalloc, bionic_simple)
{
    void *ptr = test_successful_pvalloc(100);

    cr_assert(malloc_usable_size(ptr) >= (size_t)getpagesize());
    free(ptr);
}

Test(pvalloc, bionic_overflow)
{
    cr_assert_eq(pvalloc(SIZE_MAX), NULL);
}
