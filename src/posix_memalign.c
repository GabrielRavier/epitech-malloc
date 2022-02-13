/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines posix_memalign
*/

#include <malloc.h>
#include <errno.h>

int posix_memalign(void **memptr, size_t alignment, size_t size)
{
    int old_errno = errno;
    int result;
    void *memalign_result;

    errno = 0;
    if (alignment % sizeof(void *) != 0 ||
        ((alignment - 1) & (alignment)) != 0 || alignment == 0)
        return (EINVAL);
    memalign_result = memalign(alignment, size);
    if (memalign_result != NULL)
        *memptr = memalign_result;
    result = errno;
    errno = old_errno;
    return (result);
}
