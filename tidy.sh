#!/bin/bash
# Format all cpp and hpp files in project

# Use google spec
STYLE=Google

clang-format \
  -i \
  --style=$STYLE \
  ./**/*.{cpp,hpp}