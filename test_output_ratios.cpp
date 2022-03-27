// Test runner for output_ratios

#include "lib/utils.hpp"
#include "lib/output_ratios.hpp"
#include "tests/test_cases.hpp"
#include "tests/test_utils.hpp"

// Load test networks, compute ratios, and report results
int main() {
  log("Running test cases:");

  test_outputRatio_first_column(trivialLink());
  test_outputRatio_first_column(splitter1_2());
  test_outputRatio_first_column(splitter2_1());
  test_outputRatio_first_column(splitter2_2());
  test_outputRatio_first_column(balancer3_3());
  test_outputRatio_first_column(testnetA());  // 8 node test case
  test_outputRatio_first_column(testnetB());  // 17 node test case
}
