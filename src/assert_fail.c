/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines assert_fail
*/

#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

// It is rather awkward to handle an assertion failure in a malloc... Pretty
// much the thing with the highest chance of succeeding is to use stderr, since
// that's unbuffered (i.e. it shouldn't try to malloc a buffer), but if it was
// to malloc somehow it could easily result in
// an infinite loop. Oh well, I guess if you got there you fucked up anyway,
// so it's technically ok to stack overflow from here lmfao
_Noreturn void my_malloc_assert_fail(const char *expression,
    const char *filename, int line_number, const char *function_name)
{
    fprintf(stderr, "%s:%d: %s: malloc: Assertion '%s' failed.\n", filename,
        line_number, function_name, expression);
    abort();
}
