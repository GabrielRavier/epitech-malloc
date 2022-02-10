/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Defines valloc
*/

#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>

void *valloc(size_t size)
{
    return (memalign(sysconf(_SC_PAGESIZE), size));
}
