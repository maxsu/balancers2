// Test runner for output_ratios

#include "../lib/network_tools.hpp"
#include "../lib/output_ratios.hpp"
#include "../lib/types.hpp"
#include "catch2/catch.hpp"

// Declare Testnets
TestNet trivialLink();
TestNet splitter2_1();
TestNet splitter1_2();
TestNet splitter2_2();
TestNet balancer3_3();
TestNet testnetA();
TestNet testnetB();

// Run and verify a test net
bool test_outputRatio_first_column(TestNet testnet) {
  Matrix flow = outputRatios(testnet.network);
  Row ratios = getColumn(flow, 0);
  return ratios == testnet.ratios;
}

TEST_CASE("outputRatio") {
  SECTION("Trivial Link") {
    REQUIRE(test_outputRatio_first_column(trivialLink()));
  }
  SECTION("Splitter 2-1") {
    REQUIRE(test_outputRatio_first_column(splitter2_1()));
  }
  SECTION("Splitter 1-2") {
    REQUIRE(test_outputRatio_first_column(splitter1_2()));
  }
  SECTION("Splitter 2-2") {
    REQUIRE(test_outputRatio_first_column(splitter2_2()));
  }
  SECTION("Balancer 3-3") {
    REQUIRE(test_outputRatio_first_column(balancer3_3()));
  }
  SECTION("8 node test case") {
    REQUIRE(test_outputRatio_first_column(testnetA()));
  }
  SECTION("17 node test case") {
    REQUIRE(test_outputRatio_first_column(testnetB()));
  }
}

// Define Testnets
TestNet trivialLink() {
  Network nodes = emptyNetwork(2);

  link(nodes, 0, 1);
  Row ratios = {1, 1};

  TestNet B = {"Trivial Link", nodes, ratios};
  return B;
}

TestNet splitter1_2() {
  Network nodes = emptyNetwork(4);

  link(nodes, 0, 1);
  link(nodes, 1, 2);
  link(nodes, 1, 3);

  Row ratios = {1, 0.5, 0.5, 0.5};

  TestNet B = {"Splitter 1 -> 2", nodes, ratios};
  return B;
}

TestNet splitter2_1() {
  Network nodes = emptyNetwork(4);

  link(nodes, 0, 2);
  link(nodes, 1, 2);
  link(nodes, 2, 3);

  Row ratios = {1, 0, 1, 1};

  TestNet B = {"Splitter 2 -> 1", nodes, ratios};
  return B;
}

TestNet splitter2_2() {
  Network nodes = emptyNetwork(5);

  link(nodes, 0, 2);
  link(nodes, 1, 2);
  link(nodes, 2, 3);
  link(nodes, 2, 4);

  Row ratios = {1, 0, 0.5, 0.5, 0.5};

  TestNet B = {"Splitter 2 -> 2", nodes, ratios};
  return B;
}

TestNet balancer3_3() {
  Network nodes = emptyNetwork(10);

  link(nodes, 0, 3);
  link(nodes, 1, 3);
  link(nodes, 2, 4);
  link(nodes, 3, 5);
  link(nodes, 3, 6);
  link(nodes, 4, 5);
  link(nodes, 4, 6);
  link(nodes, 5, 7);
  link(nodes, 5, 8);
  link(nodes, 6, 4);
  link(nodes, 6, 9);

  Row ratios = {1,       0,       0,       1.0 / 2, 1.0 / 6,
                1.0 / 3, 1.0 / 3, 1.0 / 3, 1.0 / 3, 1.0 / 3};

  TestNet B = {"Balancer 3 -> 3", nodes, ratios};
  return B;
}

TestNet testnetA() {
  Network nodes = emptyNetwork(8);

  link(nodes, 0, 2);
  link(nodes, 1, 2);
  link(nodes, 2, 4);
  link(nodes, 2, 5);
  link(nodes, 3, 4);
  link(nodes, 3, 5);
  link(nodes, 4, 3);
  link(nodes, 4, 6);
  link(nodes, 5, 7);

  Row ratios = {1, 0, 0.5, 1. / 6, 1. / 3, 2. / 3, 1. / 3, 2. / 3};

  TestNet testnet = {"Test Net A", nodes, ratios};
  return testnet;
}

TestNet testnetB() {
  Network nodes = emptyNetwork(17);

  link(nodes, 0, 1);
  link(nodes, 2, 1);
  link(nodes, 1, 2);
  link(nodes, 5, 2);
  link(nodes, 1, 3);
  link(nodes, 9, 3);
  link(nodes, 2, 4);
  link(nodes, 8, 5);
  link(nodes, 11, 5);
  link(nodes, 4, 6);
  link(nodes, 3, 7);
  link(nodes, 3, 8);
  link(nodes, 6, 8);
  link(nodes, 4, 9);
  link(nodes, 7, 9);
  link(nodes, 7, 10);
  link(nodes, 10, 11);
  link(nodes, 8, 11);
  link(nodes, 9, 12);
  link(nodes, 10, 12);
  link(nodes, 12, 13);
  link(nodes, 11, 14);
  link(nodes, 6, 15);
  link(nodes, 5, 16);

  Row ratios = {
      1,
      0.75641025641025639,
      0.51282051282051277,
      0.50549450549450547,
      0.25641025641025639,
      0.26923076923076927,
      0.12820512820512819,
      0.25274725274725274,
      0.31684981684981689,
      0.25457875457875456,
      0.12637362637362637,
      0.22161172161172166,
      0.38095238095238093,
      0.38095238095238093,
      0.22161172161172166,
      0.12820512820512819,
      0.26923076923076927,
  };

  TestNet testnet = {"Test Net B", nodes, ratios};
  return testnet;
}
