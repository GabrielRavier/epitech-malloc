/*
** EPITECH PROJECT, 2022
** malloc
** File description:
** Declares internal my_malloc identifiers
*/

#pragma once

#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// This is a data structure placed before every malloced block
union my_malloc_block {
    struct {
        union my_malloc_block *next;
        size_t size;
        bool is_used;
        uint16_t magic_number;
    };
    char unused_for_alignment[32];
};

_Static_assert(sizeof(union my_malloc_block) == 32);

// The magic number is a value that every allocated block contains
enum {
    MY_MALLOC_MAGIC_NUMBER = 0xADF8,
};

// This struct contains all the global data that my_malloc uses
// page_size is the value of getpagesize() * 2
// mutex is a mutex locked whenever malloc is invoked
// system_break is the break as the kernel sees it
// virtual_system_break is the break as actually used by our malloc
extern struct g_my_malloc_type {
    size_t page_size;
    pthread_mutex_t mutex;
    char *system_break;
    char *virtual_system_break;
    union my_malloc_block *list_head;
} g_my_malloc;

bool my_malloc_initializer();
void *my_malloc_increase_break(size_t size);
void *my_malloc_unlocked(size_t size);
void my_free_unlocked(void *freed_block);

#if 1
    #define MY_MALLOC_DEBUG_PRINTF(...) fprintf(stderr, __VA_ARGS__)
#else
    #define MY_MALLOC_DEBUG_PRINTF(...)
#endif
