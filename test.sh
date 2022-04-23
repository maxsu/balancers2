#!/bin/bash

# Usage:
#   ./test.sh : build and run all tests
# 
#  ./test.sh <catch2 options> : pass arguments to catch2
#   
# Examples
#   
#   Test existsBalancer only
#     ./test.sh existsBalancer
#
#   Show verbose output for all tests 
#     ./test.sh --success reporter console 
#
# See [1] for more commandline options
# 
# [1] https://github.com/catchorg/Catch2/blob/v2.x/docs/command-line.md

set -o errexit # Exit on errors

DEFAULT_TEST_OPTIONS="--success --reporter compact" # Default catch2 args
TEST_BINARY="bin/test"                              # Test runner binary
LIBS="lib/*.cpp tests/*.cpp"                        # Library sources

# Compile tests
echo Cleaning old tests
rm -f $TEST_BINARY

echo Building tests
clang++ -std=c++17 $LIBS -o $TEST_BINARY
echo Done!

# Run tests
echo Running $TEST_BINARY

if [[ -z $@ ]]; then
  # Run default tests
  $TEST_BINARY $DEFAULT_TEST_OPTIONS

else
  # Run custom tests
  $TEST_BINARY $@

fi