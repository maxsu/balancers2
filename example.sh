#!/bin/bash

# Usage:
#   ./example.sh : build optimized debug target

set -o errexit # Exit on errors

INCLUDE="lib/"
SRC="lib/*.cpp"
TESTS="tests/*.cpp"
EXAMPLE_BINARY="bin/test_example"

OPTIM='-g -O3'

clean() {
  echo Cleaning old example
  rm -rf $EXAMPLE_BINARY
}

build() {
  echo Building example
  clang++ $OPTIM -std=c++17 \
    $SRC $TESTS -I $INCLUDE \
    -o $EXAMPLE_BINARY
  echo Done!
}

main() {
  clean && build
}

main
