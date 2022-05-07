// Guard function to make vector functions safer

#pragma once

#include <vector>

// Throw exception if index is out of bounds of vector
template <class T>
void vectorGuard(std::vector<T> v, int index) {
  if (index < 0 || index > v.size()) {
    throw "Index out of bounds";
  }
}