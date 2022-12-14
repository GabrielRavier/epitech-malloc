#!/bin/sh
set -euo pipefail

# Execute tests from the directory that which contains the script
cd "$(dirname "$0")"

LIBMY_MALLOC=$(realpath ../../libmy_malloc.so)

test_command()
{
    diff -u <(LD_PRELOAD="$LIBMY_MALLOC" $1) <($1)
}

test_command 'ls' &
test_command 'ls -l' &
test_command 'ls -R /usr/share/man/man1' &
test_command 'ls -Rl /usr/share/man/man1' &
test_command 'ls -R /usr/share/man' &
test_command 'ls -Rl /usr/share/man' &

# Wait for all tests to be over before exiting
wait
