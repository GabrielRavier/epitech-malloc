##
## EPITECH PROJECT, 2022
## malloc
## File description:
## Makefile for malloc
##

# Makefiles are prettier like this
ifeq ($(origin .RECIPEPREFIX), undefined)
  $(error This Make does not support .RECIPEPREFIX. \
	Please use GNU Make 3.82 or later)
endif
.RECIPEPREFIX = >

# Use bash as the shell
SHELL := bash

# ...And use strict flags with it to make sure things fail if a step in there
# fails
.SHELLFLAGS := -eu -o pipefail -c

# Delete the target file of a Make rule if it fails - this guards against
# broken files
.DELETE_ON_ERROR:

# --warn-undefined-variables: Referencing undefined variables is probably
# wrong...
# --no-builtin-rules: I'd rather make my own rules myself, make, thanks :)
MAKEFLAGS += --warn-undefined-variables --no-builtin-rules

# We use `override` to enable setting part of CFLAGS on the command line

# This makes the compiler generate dependency files, which will solve any
# header-related dependency problems we could have had
override CFLAGS += -MMD -MP -MF $@.d

# We need to make our code position-independent since it's for a shared library
# Also notify GCC that we're making a shared library
# Also notify GCC that we need pthread support, since we use it
override CFLAGS += -fPIC -shared -pthread

# LDFLAGS should contain CFLAGS (seperate so command-line can add to it, and
# to correspond to usual practice)
override LDFLAGS += $(CFLAGS)

.PHONY: all clean fclean re make_tests_binary

.PREVIOUS: obj/%.o

BINARY_NAME := libmy_malloc.so

all: $(BINARY_NAME)

# Program sources files
SOURCE_FILES := calloc reallocarray init malloc realloc free increase_break
SOURCE_FILES += assert_fail allocate_block posix_memalign memalign
SOURCE_FILES += malloc_usable_size pvalloc valloc aligned_alloc malloc_trim

OBJECT_FILES := $(addprefix obj/src/, $(addsuffix .o, $(SOURCE_FILES)))

$(BINARY_NAME): $(OBJECT_FILES)
> $(CC) $(LDFLAGS) -o $@ $(OBJECT_FILES)

obj/src/%.o: src/%.c
> @mkdir --parents obj/src/
> $(CC) -c $< -o $@ $(CFLAGS)

# Include dependencies for the object files
include $(shell [ -d obj ] && find obj/ -type f -name '*.d')

# Remove all object files
clean:
> rm --recursive --force obj
> $(MAKE) --directory=tests/malloc clean

# Remove all object, binary and other produced files
fclean: clean
> rm --recursive --force $(BINARY_NAME)
> $(MAKE) --directory=tests/malloc fclean

# "Remakes" the project.
re:
> $(MAKE) clean
> $(MAKE) all

make_tests_binary:
> $(MAKE) --directory=tests/malloc

# Runs tests
tests_run: make_tests_binary
> $(MAKE) clean
> $(MAKE) CFLAGS="$(CFLAGS) --coverage"
> ./tests/malloc/script_test.sh & \
    LD_PRELOAD=./libmy_malloc.so timeout 5 ./tests/malloc/tests_binary & wait
