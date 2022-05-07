#include <algorithm>
#include <set>

#include "flow.hpp"

// Determines if a connection is part of a wiring
bool contains(const Wiring wiring, const int connection);

// Determines if a matrix is in a set of matrices
bool contains(const std::set<Matrix> m, const Matrix value);

// Generate an incrementing range of integers
std::vector<int> range(int size, int start = 0);