// tests/test_utils
// Tools to run and report tests

#include <string>

#include "../balancers2/output_ratios.h"
#include "../balancers2/utils.h"

void test_outputRatio_first_column(TestNet testnet) {
  Flow flow = outputRatios(testnet.network);
  Row ratios = getColumn(flow, 0);
  bool test_passed = ratios == testnet.ratios;

  string spacer(24 - testnet.name.size(), '.');

  if (test_passed) {
    log("✔️  " + testnet.name + spacer + ". Test passed! 😌");

  } else {
    log("❌  " + testnet.name + spacer + ". Test failed! 🥲 ");

    log("Expected:");
    log(testnet.ratios);

    log("Result:");
    log(ratios);
  }
}
