SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c 
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
.DELETE_ON_ERROR:
.RECIPEPREFIX := >

build             := build
include           := include
headers           := $(wildcard include/*.hpp)
sources           := $(wildcard src/*.cpp) \
                     $(wildcard tests/*.cpp)
objects           := $(sources:.cpp=.o)
optimized_objects := $(sources:.cpp=.opt)
test_bin          := $(build)/test
example_bin       := $(build)/example

cc     := clang++
std    := c++17
lflags := -g -std=$(std)             # -g: enable debugging mode 
cflags := $(lflags) -c -I $(include) # -c: generate ".o" object files
oflags := --optimize=3

# Main recipes
all: $(objects)
> $(cc) $(lflags) $(objects) -o $(test_bin)

example: $(optimized_objects)
> $(cc) $(lflags) $(optimized_objects) -o $(example_bin) $(oflags)

# Build object files
%.o: %.cpp
> $(cc) $(cflags) $< -o $@

%.opt: %.cpp
> $(cc) $(cflags) $(oflags) $< -o $@

# Utilities
test: all
> $(test_bin) --success --reporter compact

clean:
> mkdir -p $(build)
> rm -f $(objects) $(optimized_objects) $(test_bin) $(example_bin)

lint:
> clang-format -i -style=file $(sources) $(headers)

linted:
> clang-format --dry-run --Werror -style=file $(sources) $(headers)

.PHONY: example test lint linted 