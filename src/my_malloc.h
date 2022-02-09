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
// If the block is currently free, it contains a pointer to the next block
// If the block is currently allocated, it contains a bucket index and a magic
// number (for sanity checking)
// Note that this means that every usage of malloc incurs an overhead equal to
// the size of a pointer
union my_malloc_block {
    union my_malloc_block *next_block;
    struct {
        uint8_t bucket_index;
        uint16_t magic_number;
    };
};

// The total bucket count is slightly fudged, just to be reaaaaally safe
// The magic number is a value that every allocated bucket contains
enum {
    MY_MALLOC_TOTAL_BUCKET_COUNT = 65,
    MY_MALLOC_MAGIC_NUMBER = 0xADF8,
};

// This struct contains all the global data that my_malloc uses
// page_size is the value of getpagesize()
// page_size_bucket indicates which bucket we use for page-sized allocations
// (or larger)
// free_blocks is an array of pointers, each pointing to the first free block of
// a certain bucket
extern struct g_my_malloc_type {
    size_t page_size;
    size_t above_page_size_bucket;
    union my_malloc_block *free_blocks[MY_MALLOC_TOTAL_BUCKET_COUNT];
    pthread_mutex_t mutex;
} g_my_malloc;

bool my_malloc_initializer();
void *my_malloc_increase_break(size_t which_bucket);
void *my_malloc_unlocked(size_t size);
void my_free_unlocked(union my_malloc_block *freed_block);

#if 0
    #define MY_MALLOC_DEBUG_PRINTF(...) fprintf(stderr, __VA_ARGS__)
#else
    #define MY_MALLOC_DEBUG_PRINTF(...)
#endif
