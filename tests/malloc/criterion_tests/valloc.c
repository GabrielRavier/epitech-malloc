/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests valloc
*/

#include <malloc.h>
#include <criterion/criterion.h>
#include "successful_alloc.h"

#pragma GCC diagnostic ignored "-Walloc-size-larger-than=0"

Test(valloc, bionic_simple)
{
    void *ptr = test_successful_valloc(100);

    free(ptr);
}

Test(valloc, bionic_overflow)
{
    cr_assert_eq(valloc(SIZE_MAX), NULL);
}
