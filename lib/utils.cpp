// balancers2/utils.cpp
// Utilities in the system or miscellaneous domain (like logging)
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

void log(string message) { cout << message << "\n"; }

void log(vector<double> row) {
  // log numbers with full precision, usually 17 digits
  cout.precision(numeric_limits<double>::max_digits10);

  cout << "{";
  for (double val : row) {
    cout << val << ", ";
  }
  cout << "}"
       << "\n";
}

template <class T>
void vectorGuard(vector<T> v, int index) {
  if (index < 0 || index > v.size()) {
    throw "Index out of bounds";
  }
}