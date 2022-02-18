/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Declares assert
*/

#pragma once

#include <stdbool.h>

_Noreturn void my_malloc_assert_fail(const char *expression,
    const char *filename, int line_number, const char *function_name);

#define MY_MALLOC_ASSERT(expression)                                    \
    ((bool)(expression) ? (void)0 :                                     \
    my_malloc_assert_fail(#expression, __FILE__,                        \
        __LINE__, __extension__ __PRETTY_FUNCTION__))
