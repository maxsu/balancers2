# Testing

## Running tests

To run main tests, use:

    make tests

To run a specific function's test:

    make test outputRatio

To pass arguments to the test suite, use

    make test <catch2 options>

## Examples

  Test existsBalancer only
  
    make test existsBalancer
  
  Show verbose output for all tests
  
    make test --success reporter console

## Other Commandline Options

See the [Catch 2 Documentation](https://github.com/catchorg/Catch2/blob/v2.x/docs/command-line.md) for more commandline options.