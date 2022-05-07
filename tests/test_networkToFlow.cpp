// Test runner for output_ratios
#include "catch2/catch.hpp"
#include "flow.hpp"

// Generate a network and verify its first column of flow against a given row
void networkToFlowFirstColumnExpects(int size, Links links,
                                     Row expected_ratios) {
  Network network = makeNetwork(size, links);
  Matrix flow = networkToFlow(network);
  REQUIRE(expected_ratios == getColumn(flow, 0));
}

TEST_CASE("networkToFlow") {
  SECTION("Trivial Link") {
    networkToFlowFirstColumnExpects(2, {{0, 1}}, {1, 1});
  }

  SECTION("Splitter 2-1") {
    networkToFlowFirstColumnExpects(4, {{0, 2}, {1, 2}, {2, 3}}, {1, 0, 1, 1});
  }

  SECTION("Splitter 1-2") {
    networkToFlowFirstColumnExpects(4, {{0, 1}, {1, 2}, {1, 3}},
                                    {1, 0.5, 0.5, 0.5});
  }

  SECTION("Splitter 2-2") {
    networkToFlowFirstColumnExpects(5, {{0, 2}, {1, 2}, {2, 3}, {2, 4}},
                                    {1, 0, 0.5, 0.5, 0.5});
  }

  SECTION("Balancer 3-3") {
    networkToFlowFirstColumnExpects(10,
                                    {{0, 3},
                                     {1, 3},
                                     {2, 4},
                                     {3, 5},
                                     {3, 6},
                                     {4, 5},
                                     {4, 6},
                                     {5, 7},
                                     {5, 8},
                                     {6, 4},
                                     {6, 9}},
                                    {1, 0, 0, 1.0 / 2, 1.0 / 6, 1.0 / 3,
                                     1.0 / 3, 1.0 / 3, 1.0 / 3, 1.0 / 3});
  }

  SECTION("8 node test case") {
    networkToFlowFirstColumnExpects(
        8,
        {{0, 2},
         {1, 2},
         {2, 4},
         {2, 5},
         {3, 4},
         {3, 5},
         {4, 3},
         {4, 6},
         {5, 7}},
        {1, 0, 0.5, 1. / 6, 1. / 3, 2. / 3, 1. / 3, 2. / 3});
  }

  SECTION("17 node test case") {
    networkToFlowFirstColumnExpects(
        17, {{0, 1},  {2, 1},   {1, 2},   {5, 2},   {1, 3},   {9, 3},
             {2, 4},  {8, 5},   {11, 5},  {4, 6},   {3, 7},   {3, 8},
             {6, 8},  {4, 9},   {7, 9},   {7, 10},  {10, 11}, {8, 11},
             {9, 12}, {10, 12}, {12, 13}, {11, 14}, {6, 15},  {5, 16}},
        {
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
        });
  }
}
