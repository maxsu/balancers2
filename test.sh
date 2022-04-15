#!/bin/bash
# Exit on errors
set -o errexit

# Library sources
LIBS="lib/* tests/*" 

function test {
  # Compile and run a test file
  test_file=$1
  binary=bin/$(basename $test_file .cpp)

  # Clean output
  echo Cleaning $binary
  rm -f $binary

  # Build test
  echo Building $test_file
  clang++ -std=c++17 $test_file $LIBS
  mv a.out $binary
  echo Done!

  echo Running $binary
  if [ $# -gt 1 ]
  then
    test_to_run=$2
    $binary $test_to_run
  else
    $binary
  fi
}

if [ $# -gt 0 ]
  then
  test run_tests.cpp $1
else
  test run_tests.cpp
fi

# Set environment variable TEST_ONE or TEST_TWO to run just one test
#if [[ -n $TEST_ONE ]]; then
#  test test_output_ratios.cpp
#  exit
#fi

#if [[ -n $TEST_TWO ]]; then
#  test test_exists_balancer.cpp
#  exit
#fi

# Otherwise run all tests
#test test_output_ratios.cpp
#test test_exists_balancer.cpp
