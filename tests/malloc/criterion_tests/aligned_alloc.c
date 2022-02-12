/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests aligned_alloc
*/

#include <malloc.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>

Test(aligned_alloc, illumos)
{
    errno = 0;
    cr_assert_eq(aligned_alloc(sizeof(void *) - 1, 16), NULL);
    cr_assert_eq(errno, EINVAL);

    errno = 0;
    cr_assert_eq(aligned_alloc(sizeof(void *) + 1, 16), NULL);
    cr_assert_eq(errno, EINVAL);

    errno = 0;
    cr_assert_eq(aligned_alloc(23, 16), NULL);
    cr_assert_eq(errno, EINVAL);

    void *buf = aligned_alloc(sizeof(void *), 16);
    cr_assert_neq(buf, NULL);
    free(buf);
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
            void *p = aligned_alloc(alignments[j], sizes[i]);

            if (p == NULL) {
                if (alignments[j] == 0 || ((alignments[j] - 1) & alignments[j]) != 0 ||
                    sizes[i] % alignments[j] != 0)
                    cr_assert_eq(errno, EINVAL);
                else
                    cr_assert_eq(errno, ENOMEM);
            } else {
                cr_assert_neq(alignments[j], 0);
                cr_assert_eq((alignments[j] - 1) & alignments[j], 0);
                cr_assert_eq((uintptr_t)p & (alignments[j] - 1), 0);
                memset(p, 0x2B, sizes[i]);
                free(p);
            }
        }
    };
}

Test(aligned_alloc, jemalloc_alignment_errors)
{
    errno = 0;
    cr_assert_eq(aligned_alloc(0, 1), NULL);
    cr_assert_eq(errno, EINVAL);

    for (size_t alignment = sizeof(size_t); alignment < ((size_t)1 << (sizeof(size_t) * CHAR_BIT - 1)); alignment <<= 1) {
        errno = 0;
        cr_assert_eq(aligned_alloc(alignment + 1, 1), NULL);
        cr_assert_eq(errno, EINVAL);
    }
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
        for (size_t size = 1; size < 3 * alignment && size < (1u << 31); size += (alignment >> (sizeof(void *) - 1)) - 1) {
            for (int i = 0; i < PTR_COUNT; ++i) {
                ptrs[i] = aligned_alloc(alignment, size);
                cr_assert_neq(ptrs[i], NULL);
                total += malloc_usable_size(ptrs[i]);
                if (total >= ((size_t)1 << 22))
                    break;
            }
            for (int i = 0; i < PTR_COUNT; ++i) {
                free(ptrs[i]);
                ptrs[i] = NULL;
            }
        }
    }
}
