#!/bin/bash

# Usage:
#   ./build.sh : build project and run tests
#
#  ./build.sh <catch2 options> : pass arguments to catch2
#
# Examples
#
#   Test existsBalancer only
#     ./build.sh existsBalancer
#
#   Show verbose output for all tests
#     ./build.sh --success reporter console
#
# See [1] for more commandline options
#
# [1] https://github.com/catchorg/Catch2/blob/v2.x/docs/command-line.md

set -o errexit # Exit on errors

# Code config
INCLUDE="include/"
SRC="src/*.cpp"
TESTS="tests/*.cpp"

# Build config
BUILD_DIR="build/"

# Executables
EXAMPLE_BINARY="build/test_example"
TEST_BINARY="build/test"

# Testing config
DEFAULT_CATCH2_ARGS="--success --reporter compact"

# Compiler config
cpp_std='c++17'
cpp='clang++'
cpp_flags="$SRC $TESTS --include-directory $INCLUDE --debug --std $cpp_std"

process_args() {
  if [[ -z $1 ]]; then
    CATCH2_ARGS=$DEFAULT_CATCH2_ARGS
  else
    CATCH2_ARGS=$*
  fi
}

clean() {
  echo Linting project
  ./tools/tidy.sh

  echo Checking build directory
  mkdir -p $BUILD_DIR

  echo Cleaning old tests
  rm -rf $TEST_BINARY

  echo Cleaning old example
  rm -rf $EXAMPLE_BINARY
}

compile() {
  $cpp $cpp_flags $*
}

build_tests() {
  compile --output $TEST_BINARY
}

build_examples() {
  compile --output $EXAMPLE_BINARY --optimize=3
}

run_tests() {
  echo testing binary: $TEST_BINARY. Catch2 options: $CATCH2_ARGS
  $TEST_BINARY $CATCH2_ARGS
}

main() {
  echo Processing test options
  process_args "$1"

  echo Cleaning project
  clean

  echo Building tests
  build_tests

  echo Testing
  run_tests

  echo Building example
  build_examples
}

main "$@"
