/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines calloc
*/

#include <malloc.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include "successful_alloc.h"
#include <errno.h>
#include <stdint.h>

#pragma GCC diagnostic ignored "-Walloc-size-larger-than=0"

static void osv_do_test_part(size_t a, size_t b, const char *zero_buffer)
{
    void *buffer = test_successful_calloc(a, b);
    cr_assert_eq(memcmp(buffer, zero_buffer, a * b), 0);
    memset(buffer, 'X', a * b);
    free(buffer);
}

static void osv_do_test(size_t a, size_t b, const char *zero_buffer)
{
    osv_do_test_part(a, b, zero_buffer);
    osv_do_test_part(b, a, zero_buffer);
}

Test(calloc, osv)
{
    const char bunch_of_0s[10] = "\0";

    osv_do_test(1, 10, bunch_of_0s);
    osv_do_test(2, 5, bunch_of_0s);
    osv_do_test(1, 0, bunch_of_0s);
}

Test(calloc, overflow)
{
    cr_assert_eq(calloc(SIZE_MAX, SIZE_MAX), NULL);
    cr_assert_eq(errno, ENOMEM);
}

Test(calloc, bionic_simple)
{
    static const size_t ALLOC_LEN = 100;

    char *ptr = test_successful_calloc(1, ALLOC_LEN);
    for (size_t i = 0; i < ALLOC_LEN; ++i)
        cr_assert_eq(ptr[i], 0);
    free(ptr);
}

Test(calloc, bionic_illegal)
{
    errno = 0;
    cr_assert_eq(calloc(-1, 100), NULL);
    cr_assert_eq(errno, ENOMEM);
}

static void bionic_overflow_do_test(size_t a, size_t b)
{
    errno = 0;
    cr_assert_eq(calloc(a, b), NULL);
    cr_assert_eq(errno, ENOMEM);
}

Test(calloc, bionic_overflow)
{
    bionic_overflow_do_test(1, SIZE_MAX);
    bionic_overflow_do_test(SIZE_MAX, SIZE_MAX);
    bionic_overflow_do_test(2, SIZE_MAX);
    bionic_overflow_do_test(SIZE_MAX, 2);
}

Test(calloc, bionic_usable_size)
{
    for (size_t size = 1; size <= 2048; ++size) {
        void *pointer = test_successful_malloc(size);
        memset(pointer, 0xEB, malloc_usable_size(pointer));
        free(pointer);

        uint8_t *zero_mem = (uint8_t *)test_successful_calloc(1, size);
        size_t usable_size = malloc_usable_size(zero_mem);
        for (size_t i = 0; i < usable_size; ++i)
            cr_assert_eq(zero_mem[i], 0);
        free(zero_mem);
    }
}

Test(calloc, bionic_0_sizes)
{
    free(test_successful_calloc(0, 0));
    free(test_successful_calloc(0, 1));
    free(test_successful_calloc(1, 0));
}

