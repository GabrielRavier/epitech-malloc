/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines checked_malloc
*/

#pragma once

#include <malloc.h>
#include <criterion/criterion.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>

static size_t succesful_alloc_internal_next_pow2(size_t x)
{
    _Static_assert(sizeof(size_t) <= sizeof(unsigned long long),
        "__builtin_clzll doesn't work for size_t !");
    return (x == 1 ? 1 :
        1 << ((sizeof(size_t) * CHAR_BIT) - __builtin_clzll(x - 1)));
}

static inline void *successful_alloc_internal_check_properties(void *ptr, size_t size)
{
    cr_assert_neq(ptr, NULL);
    cr_assert_eq(errno, 0);
    cr_assert(malloc_usable_size(ptr) >= size);
    return (ptr);
}

static inline void *test_successful_malloc(size_t size)
{
    errno = 0;
    return (successful_alloc_internal_check_properties(malloc(size), size));
}

static inline void *test_successful_calloc(size_t num_elements, size_t element_size)
{
    errno = 0;
    return (successful_alloc_internal_check_properties(calloc(num_elements, element_size), num_elements * element_size));
}

static inline void *test_successful_realloc(void *old_ptr, size_t new_size)
{
    errno = 0;
    return (successful_alloc_internal_check_properties(realloc(old_ptr, new_size), new_size));
}

static inline void *successful_alloc_internal_do_memalign_like(size_t alignment, size_t size, void *(*memalign_like_func)(size_t alignment, size_t size))
{
    errno = 0;
    void *result = successful_alloc_internal_check_properties(memalign_like_func(alignment, size), size);

    if (alignment != 0)
        cr_assert_eq((uintptr_t)result % succesful_alloc_internal_next_pow2(alignment), 0);
    return (result);
}

static inline void *test_successful_aligned_alloc(size_t alignment, size_t size)
{
    return (successful_alloc_internal_do_memalign_like(alignment, size, &aligned_alloc));
}

static inline void *test_successful_memalign(size_t alignment, size_t size)
{
    return (successful_alloc_internal_do_memalign_like(alignment, size, &memalign));
}

static inline void test_successful_posix_memalign(void **memptr, size_t alignment, size_t size)
{
    void *sentinel = (void *)0xBAD00000;
    *memptr = sentinel;

    errno = 0;
    int result = posix_memalign(memptr, alignment, size);
    cr_assert_eq(result, 0);

    successful_alloc_internal_check_properties(*memptr, size);
    cr_assert_neq(*memptr, sentinel);
}

static inline void *test_successful_pvalloc(size_t size)
{
    errno = 0;
    void *result = successful_alloc_internal_check_properties(pvalloc(size), size);

    cr_assert_eq((uintptr_t)result & (getpagesize() - 1), 0);
    return (result);
}

static inline void *test_successful_valloc(size_t size)
{
    errno = 0;
    void *result = successful_alloc_internal_check_properties(valloc(size), size);

    cr_assert_eq((uintptr_t)result & (getpagesize() - 1), 0);
    return (result);
}
