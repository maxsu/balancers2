# Building balancers2

## Dependencies

To get project dependencies use

### Mac

    brew install make clang-format catch2

### Debian

    sudo apt install make clang-14 clang-format catch2

## Build project

To build the project, run

    make all

### Run tests

To run all tests use

    make test

For more testing information see [Testing](testing.md).

### Build profiling target

To build an example for profiling use

    make example

### Clean project

To remove all compiled files, run

    make clean

### Lint code

To format the source code, run

    make lint

To ensure code is formatted, use

    make linted
