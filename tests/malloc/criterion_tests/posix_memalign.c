/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests posix_memalign
*/

#include <malloc.h>
#include <criterion/criterion.h>
#include "successful_alloc.h"
#include <errno.h>
#include <stdint.h>
#include <limits.h>

Test(posix_memalign, illumos)
{
    void *sentinel = (void *)0xBAD00000;
    void *buf = sentinel;

    cr_assert_eq(posix_memalign(&buf, sizeof(void *) - 1, 16), EINVAL);
    cr_assert_eq(buf, sentinel);

    cr_assert_eq(posix_memalign(&buf, sizeof(void *) + 1, 16), EINVAL);
    cr_assert_eq(buf, sentinel);

    cr_assert_eq(posix_memalign(&buf, 23, 16), EINVAL);
    cr_assert_eq(buf, sentinel);

    test_successful_posix_memalign(&buf, sizeof(void *), 16);
    free(buf);
}

Test(posix_memalign, netbsd_basic)
{
    static const size_t sizes[] = {
        1, 2, 3, 4, 10, 100, 16384, 32768, 65536, 10000, 0
    };
    static const size_t alignments[] = {
        512, 1024, 16, 32, 64, 4, 2048, 16, 2, 0
    };

    for (size_t i = 0; i < (sizeof(sizes) / sizeof(sizes[0])); ++i) {
        for (size_t j = 0; j < (sizeof(alignments) / sizeof(alignments[0])); ++j) {
            void *p = (void *)1;
            int memalign_result;
            memalign_result = posix_memalign(&p, alignments[j], sizes[i]);

            if (alignments[j] < sizeof(void *)) {
                cr_assert_eq(p, (void *)1);
                cr_assert_eq(memalign_result, EINVAL);
            } else {
                cr_assert_eq(memalign_result, 0);
                cr_assert_eq((uintptr_t)p & (alignments[j] - 1), 0);
                memset(p, 0x2B, sizes[i]);
                free(p);
            }
        }
    };
}

Test(posix_memalign, jemalloc_alignment_errors)
{
    for (size_t alignment = 0; alignment < sizeof(void *); ++alignment) {
        void *result;
        cr_assert_eq(posix_memalign(&result, alignment, 1), EINVAL);
    }
    for (size_t alignment = sizeof(void *); alignment < ((uintptr_t)1 << (sizeof(void *) * CHAR_BIT - 2)); alignment <<= 1) {
        void *result;
        cr_assert_eq(posix_memalign(&result, alignment + 1, 1), EINVAL);
    }
}

static void do_jemalloc_oom_error_test(size_t alignment, size_t size)
{
    void *result;

    cr_assert_eq(posix_memalign(&result, alignment, size), ENOMEM);
}

Test(posix_memalign, jemalloc_oom_errors)
{
    do_jemalloc_oom_error_test(0x8000000000000000, 0x8000000000000000);
    do_jemalloc_oom_error_test(0x4000000000000000, 0xc000000000000001);
    do_jemalloc_oom_error_test(0x10LU, 0xfffffffffffffff0);
}

Test(posix_memalign, jemalloc_alignment_and_size)
{
    enum {
        PTR_COUNT = 4
    };
    void *ptrs[PTR_COUNT] = {};

    for (size_t alignment = 8; alignment <= ((size_t)1 << 23); alignment <<= 1) {
        size_t total = 0;
        for (size_t size = 1; size < 3 * alignment && size < (1u << 31); size += ((size == 0) ? 1 : ((alignment >> 2) - 1))) {
            for (int i = 0; i < PTR_COUNT; ++i) {
                test_successful_posix_memalign(&ptrs[i], alignment, size);
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
