#include <algorithm>
#include <set>

#include "wiring.hpp"

bool contains(const Wiring wiring, const int connection) {
  auto it = std::find(wiring.begin(), wiring.end(), connection);
  return it != wiring.end();
}

bool contains(const std::set<Matrix> m, const Matrix value) {
  return m.find(value) != m.end();
}

std::vector<int> range(const int size, const int start = 0) {
  std::vector<int> result;
  for (int number = start; number < start + size; ++number) {
    result.push_back(number);
  }
  return result;
}