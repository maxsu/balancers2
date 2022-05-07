// Utilities in the system domain (like logging)

#include <iostream>
#include <limits>
#include <string>
#include <vector>

const int MAX_PRECISION = std::numeric_limits<double>::max_digits10;

void log(std::string message) { std::cout << message << "\n"; }

void log(std::vector<double> row) {
  std::cout.precision(MAX_PRECISION);
  std::cout << "{";
  for (double val : row) {
    std::cout << val << ", ";
  }
  std::cout << "}"
            << "\n";
}