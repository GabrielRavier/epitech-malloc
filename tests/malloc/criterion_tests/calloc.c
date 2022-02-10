/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines calloc
*/

#include <stdlib.h>
#include <criterion/criterion.h>

static void osv_do_test_part(size_t a, size_t b, const char *zero_buffer)
{
    void *buffer = calloc(a, b);
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
