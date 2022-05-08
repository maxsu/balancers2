// Utilities in the system domain (like logging)

#pragma once

#include <iostream>
#include <limits>
#include <string>
#include <vector>

// Log a string to console
void log(std::string message) { std::cout << message << "\n"; }

// Log a vector<double> to console
void log(std::vector<double> row) {
  // log numbers with full precision, usually 17 digits
  std::cout.precision(std::numeric_limits<double>::max_digits10);

  std::cout << "{";
  for (double val : row) {
    std::cout << val << ", ";
  }
  std::cout << "}"
            << "\n";
}

// Throw exception if index is out of bounds of vector
template <class T>
void vectorGuard(std::vector<T> v, int index) {
  if (index < 0 || index > v.size()) {
    throw "Index out of bounds";
  }
}