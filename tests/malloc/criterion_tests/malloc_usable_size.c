/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests malloc_usable_size
*/

#include <malloc.h>
#include <criterion/criterion.h>

void osv_do_one_test(size_t size)
{
    void *ptr = malloc(size);
    cr_assert(malloc_usable_size(ptr) >= size);
    free(ptr);
}

Test(malloc_usable_size, osv)
{
    for (size_t i = 0; i < 1000; ++i)
        osv_do_one_test(i);
    osv_do_one_test(0x4010);
    osv_do_one_test(0x100000);
    osv_do_one_test(0x200000);
}

Test(malloc_usable_size, null)
{
    cr_assert_eq(malloc_usable_size(NULL), 0);
}
