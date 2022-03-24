#!/bin/bash

function test {
  in=$1
  out=bin/$(basename $in .cpp)

  echo Cleaning $out
  rm -f $out

  echo Building $in
  clang++ \
    -g $in \
    -o $out
  echo Done!

  echo Running $out
  $out
}

test test_output_ratios.cpp
test test_exists_balancer.cpp
