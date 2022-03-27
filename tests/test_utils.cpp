// Tools to run and report tests

#include <string>

#include "../lib/network_tools.hpp"
#include "../lib/output_ratios.hpp"
#include "../lib/utils.hpp"
#include "test_utils.hpp"

void test_outputRatio_first_column(TestNet testnet) {
  Matrix flow = outputRatios(testnet.network);
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
