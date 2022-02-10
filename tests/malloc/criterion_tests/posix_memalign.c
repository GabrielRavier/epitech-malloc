/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Tests posix_memalign
*/

#include <stdlib.h>
#include <criterion/criterion.h>
#include <errno.h>
#include <stdint.h>

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

    cr_assert_eq(posix_memalign(&buf, sizeof(void *), 16), 0);
    cr_assert_neq(buf, sentinel);
    cr_assert_neq(buf, NULL);
    cr_assert_eq(((uintptr_t)buf % sizeof(void *)), 0);
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
