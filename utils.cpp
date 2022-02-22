#include <iostream>
#include <limits>
#include <vector>

using namespace std;

// Log a string to console
void log(string message) { cout << message << "\n\n"; }

// Log a vector<double> to console with full precision
void log(vector<double> row) {
  cout.precision(numeric_limits<double>::max_digits10);
  for (double val : row) {
    cout << val << " ";
  }
  cout << "\n\n";
}