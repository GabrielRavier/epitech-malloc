/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests aligned_alloc
*/

#include <stdlib.h>
#include <criterion/criterion.h>
#include <errno.h>
#include <stdint.h>

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
