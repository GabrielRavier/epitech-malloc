/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests memalign
*/

#include <malloc.h>
#include <criterion/criterion.h>
#include "successful_alloc.h"
#include <unistd.h>
#include <stdint.h>

#pragma GCC diagnostic ignored "-Walloc-size-larger-than=0"

static uint8_t haiku_sum(uintptr_t address)
{
    return (address >> 24) | (address >> 16) | (address >> 8) | address;
}

static void haiku_write_test_pattern(uint8_t *address, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        address[i] = haiku_sum((uintptr_t)address + i);
}

static void haiku_verify_test_pattern(uint8_t *address, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        if (address[i] != haiku_sum((uintptr_t)address + i))
            cr_assert_fail("Invalid test pattern ! %u != %u !", address[i], haiku_sum((uintptr_t)address + i));
}

static void haiku_do_random_no_alignment(size_t count, size_t max_size)
{
    void **allocations = malloc(count * sizeof(void *));
    size_t *sizes = malloc(count * sizeof(size_t));

    if (max_size >= (1 << 14))
        count /= 16;
    for (size_t i = 0; i < count; ++i) {
        sizes[i] = random() % max_size;
        allocations[i] = test_successful_malloc(sizes[i]);
        cr_assert_eq((uintptr_t)allocations[i] % 8, 0);
        haiku_write_test_pattern(allocations[i], sizes[i]);
    }
    for (size_t i = count - 1; i != (size_t)-1; --i) {
        haiku_verify_test_pattern(allocations[i], sizes[i]);
        free(allocations[i]);
    }
    free(sizes);
    free(allocations);
}

static void haiku_do_random_fixed_alignment(size_t count, size_t max_size, size_t alignment)
{
    void **allocations = malloc(count * sizeof(void *));
    size_t *sizes = malloc(count * sizeof(size_t));

    if (max_size > (1 << 15))
        count /= 16;
    for (size_t i = 0; i < count; ++i) {
        sizes[i] = random() % max_size;
        allocations[i] = test_successful_memalign(alignment, sizes[i]);
        haiku_write_test_pattern(allocations[i], sizes[i]);
    }
    for (size_t i = count - 1; i != (size_t)-1; --i) {
        haiku_verify_test_pattern(allocations[i], sizes[i]);
        free(allocations[i]);
    }
    free(sizes);
    free(allocations);
}

static void haiku_do_random_random_alignment(size_t count, size_t max_size)
{
    for (size_t i = 0; i < (count / 128); ++i)
        haiku_do_random_fixed_alignment(128, max_size, 1 << (random() % 18));
}

Test(memalign, haiku)
{
    haiku_do_random_no_alignment(1024, getpagesize() * 128);
    haiku_do_random_random_alignment(1024, getpagesize() * 128);
}

Test(memalign, bionic_overflow)
{
    cr_assert_eq(memalign(4096, SIZE_MAX), NULL);
}

Test(memalign, bionic_realloc)
{
    for (size_t alignment = 1; alignment <= 4096; alignment <<= 1) {
        char *ptr = test_successful_memalign(alignment, 100);
        memset(ptr, 0x23, 100);

        ptr = test_successful_realloc(ptr, 200);
        for (size_t i = 0; i < 100; ++i)
            cr_assert_eq(ptr[i], 0x23);
        memset(ptr, 0x45, 200);

        ptr = test_successful_realloc(ptr, 300);
        for (size_t i = 0; i < 200; ++i)
            cr_assert_eq(ptr[i], 0x45);
        memset(ptr, 0x67, 300);

        ptr = test_successful_realloc(ptr, 250);
        for (size_t i = 0; i < 250; ++i)
            cr_assert_eq(ptr[i], 0x67);
        free(ptr);
    }
}
