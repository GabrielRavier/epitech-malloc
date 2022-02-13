/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests realloc
*/

#include <malloc.h>
#include <criterion/criterion.h>
#include "successful_alloc.h"

#pragma GCC diagnostic ignored "-Walloc-size-larger-than=0"

static void osv_do_one_realloc_with_alignment(size_t original_size, size_t new_size, size_t alignment)
{
    static const char data[10] = "0123456789";

    char *original_buffer = test_successful_memalign(alignment, original_size);

    for (size_t i = 0; i < original_size; i += 10) {
        size_t memcpy_len = ((original_size - i) < 10) ? (original_size - i) : 10;
        memcpy(&original_buffer[i], data, memcpy_len);
    }

    char *new_buffer = test_successful_realloc(original_buffer, new_size);
    size_t len_to_check = original_size < new_size ? original_size : new_size;
    for (size_t i = 0; i < len_to_check; i += 10) {
        size_t memcmp_len = len_to_check - i < 10 ? len_to_check - i : 10;
        cr_assert_eq(memcmp(&new_buffer[i], data, memcmp_len), 0);
    }
    free(new_buffer);
}

static void osv_do_one_realloc(size_t original_size, size_t new_size)
{
    osv_do_one_realloc_with_alignment(original_size, new_size, sizeof(void *));
    osv_do_one_realloc_with_alignment(original_size, new_size, sizeof(void *) * 2);
    osv_do_one_realloc_with_alignment(original_size, new_size, sizeof(void *) * 4);
    osv_do_one_realloc_with_alignment(original_size, new_size, sizeof(void *) * 8);
}

Test(realloc, osv)
{
    osv_do_one_realloc(1, 2);
    osv_do_one_realloc(2, 1);
    osv_do_one_realloc(4, 7);
    osv_do_one_realloc(7, 4);
    osv_do_one_realloc(63, 128);
    osv_do_one_realloc(128, 63);
    osv_do_one_realloc(4000, 5000);
    osv_do_one_realloc(5000, 4000);
    osv_do_one_realloc(4096, 4096);
    osv_do_one_realloc(0x100000, 0x100000);
    osv_do_one_realloc(0x100000, 0x100900);
    osv_do_one_realloc(0x100900, 0x100000);
    osv_do_one_realloc(0x200000, 0x200000);
    osv_do_one_realloc(0x200000, 0x300900);
    osv_do_one_realloc(0x300900, 0x200000);
    osv_do_one_realloc(0x600900, 0x600000);
    osv_do_one_realloc(0x400000, 0x600000);
    osv_do_one_realloc(0x600000, 0x400900);

    for (size_t i = 0; i < 10; ++i)
        osv_do_one_realloc((rand() % 100000) + 1, (rand() % 100000) + 1);

    void *buffer = test_successful_realloc(NULL, 0);
    free(buffer);

    buffer = test_successful_malloc(16);
    cr_assert_eq(realloc(buffer, 0), NULL);
}

Test(realloc, bionic_larger)
{
    char *ptr = (char *)test_successful_malloc(100);
    memset(ptr, 67, 100);

    ptr = (char *)test_successful_realloc(ptr, 200);
    for (size_t i = 0; i < 100; ++i)
        cr_assert_eq(ptr[i], 67);
    free(ptr);
}

Test(realloc, bionic_smaller)
{
    char *ptr = (char *)test_successful_malloc(200);
    memset(ptr, 67, 200);

    ptr = (char *)test_successful_realloc(ptr, 100);
    for (size_t i = 0; i < 100; ++i)
        cr_assert_eq(ptr[i], 67);
    free(ptr);
}

Test(realloc, bionic_multiple)
{
    char *ptr = (char *)test_successful_malloc(200);
    memset(ptr, 0x23, 200);

    ptr = (char *)test_successful_realloc(ptr, 100);
    for (size_t i = 0; i < 100; ++i)
        cr_assert_eq(0x23, ptr[i]);

    ptr = (char *)test_successful_realloc(ptr, 50);
    for (size_t i = 0; i < 50; ++i)
        cr_assert_eq(0x23, ptr[i]);

    ptr = (char *)test_successful_realloc(ptr, 150);
    for (size_t i = 0; i < 50; ++i)
        cr_assert_eq(0x23, ptr[i]);
    memset(ptr + 50, 0x23, 100);

    ptr = (char *)test_successful_realloc(ptr, 425);
    for (size_t i = 0; i < 150; ++i)
        cr_assert_eq(0x23, ptr[i]);
    free(ptr);
}

Test(realloc, bionic_calloc_larger)
{
    char *ptr = (char *)test_successful_calloc(1, 100);

    ptr = (char *)test_successful_realloc(ptr, 200);
    for (size_t i = 0; i < 100; ++i)
        cr_assert_eq(ptr[i], 0);
    free(ptr);
}

Test(realloc, bionic_calloc_smaller)
{
    char *ptr = (char *)test_successful_calloc(1, 200);

    ptr = (char *)test_successful_realloc(ptr, 100);
    for (size_t i = 0; i < 100; ++i)
        cr_assert_eq(ptr[i], 0);
    free(ptr);
}

Test(realloc, bionic_calloc_multiple)
{
    char *ptr = (char *)test_successful_calloc(1, 200);

    ptr = (char *)test_successful_realloc(ptr, 100);
    for (size_t i = 0; i < 100; ++i)
        cr_assert_eq(0, ptr[i]);

    ptr = (char *)test_successful_realloc(ptr, 50);
    for (size_t i = 0; i < 50; ++i)
        cr_assert_eq(0, ptr[i]);

    ptr = (char *)test_successful_realloc(ptr, 150);
    for (size_t i = 0; i < 50; ++i)
        cr_assert_eq(0, ptr[i]);
    memset(ptr + 50, 0x23, 100);

    ptr = (char *)test_successful_realloc(ptr, 425);
    for (size_t i = 0; i < 150; ++i)
        cr_assert_eq(i < 50 ? 0 : 0x23, ptr[i]);
    free(ptr);
}

Test(realloc, bionic_overflow)
{
    errno = 0;
    cr_assert_eq(realloc(NULL, SIZE_MAX), NULL);
    cr_assert_eq(errno, ENOMEM);

    void *ptr = test_successful_malloc(100);
    errno = 0;
    cr_assert_eq(realloc(ptr, SIZE_MAX), NULL);
    cr_assert_eq(errno, ENOMEM);
    free(ptr);
}

Test(realloc, bionic_null_0)
{
    free(test_successful_realloc(NULL, 0));
}

Test(realloc, bionic_0)
{
    void *p = test_successful_malloc(1024);

    cr_assert_eq(realloc(p, 0), NULL);
}
