/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests aligned_alloc
*/

#include <malloc.h>
#include <criterion/criterion.h>
#include "successful_alloc.h"
#include <errno.h>
#include <stdint.h>
#include <limits.h>

#pragma GCC diagnostic ignored "-Walloc-size-larger-than=0"

Test(aligned_alloc, illumos)
{
    free(test_successful_aligned_alloc(sizeof(void *) - 1, 16));
    free(test_successful_aligned_alloc(sizeof(void *) + 1, 16));
    free(test_successful_aligned_alloc(23, 16));
    free(test_successful_aligned_alloc(sizeof(void *), 16));
}

Test(aligned_alloc, netbsd_basic)
{
    static const size_t sizes[] = {
        1, 2, 3, 4, 10, 100, 16384, 32768, 65536, 10000, 0
    };
    static const size_t alignments[] = {
        512, 1024, 16, 32, 64, 4, 2048, 16, 2, 0
    };

    for (size_t i = 0; i < (sizeof(sizes) / sizeof(sizes[0])); ++i) {
        for (size_t j = 0; j < (sizeof(alignments) / sizeof(alignments[0])); ++j) {
            void *p = test_successful_aligned_alloc(alignments[j], sizes[i]);

            cr_assert_eq((alignments[j] - 1) & alignments[j], 0);
            memset(p, 0x2B, sizes[i]);
            free(p);
        }
    };
}

static void do_jemalloc_oom_errors_test(size_t alignment, size_t size)
{
    errno = 0;
    cr_assert_eq(aligned_alloc(alignment, size), NULL);
    cr_assert_eq(errno, ENOMEM);
}

Test(aligned_alloc, jemalloc_oom_errors)
{
    do_jemalloc_oom_errors_test(0x8000000000000000, 0x8000000000000000);
    do_jemalloc_oom_errors_test(0x4000000000000000, 0xc000000000000000);
    do_jemalloc_oom_errors_test(0x10LU, 0xfffffffffffffff0);
}

Test(aligned_alloc, jemalloc_alignment_and_size)
{
    enum {
        PTR_COUNT = 4
    };
    void *ptrs[PTR_COUNT] = {};

    for (size_t alignment = 8; alignment <= ((size_t)1 << 23); alignment <<= 1) {
        size_t total = 0;
        for (size_t size = 1; size < 3 * alignment && size < (1u << 31); size += (alignment >> 2) - 1) {
            for (int i = 0; i < PTR_COUNT; ++i) {
                ptrs[i] = test_successful_aligned_alloc(alignment, size);
                total += malloc_usable_size(ptrs[i]);
                if (total >= ((size_t)1 << 24))
                    break;
            }
            for (int i = 0; i < PTR_COUNT; ++i) {
                free(ptrs[i]);
                ptrs[i] = NULL;
            }
        }
    }
}
