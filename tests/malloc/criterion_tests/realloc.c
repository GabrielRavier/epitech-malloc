/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests realloc
*/

#include <stdlib.h>
#include <criterion/criterion.h>
#include <malloc.h>

static void osv_do_one_realloc_with_alignment(size_t original_size, size_t new_size, size_t alignment)
{
    static const char data[10] = "0123456789";

    char *original_buffer = memalign(alignment, original_size);
    cr_assert_neq(original_buffer, NULL);

    for (size_t i = 0; i < original_size; i += 10) {
        size_t memcpy_len = ((original_size - i) < 10) ? (original_size - i) : 10;
        memcpy(&original_buffer[i], data, memcpy_len);
    }

    char *new_buffer = realloc(original_buffer, new_size);
    cr_assert_neq(new_buffer, NULL);

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

    void *buffer = realloc(NULL, 0);
    cr_assert_neq(buffer, NULL);
    free(buffer);

    buffer = malloc(16);
    cr_assert_eq(realloc(buffer, 0), NULL);
}
