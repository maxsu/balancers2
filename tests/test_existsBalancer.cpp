// Test Runner for exists_balancers

#include <iostream>

#include "../lib/exists_balancer.hpp"
#include "catch2/catch.hpp"

unsigned int Factorial2(unsigned int number) {
  return number <= 1 ? number : Factorial2(number - 1) * number;
}

TEST_CASE("existsBalancer") {
  SECTION("Balancer 1-1-1 exists") { REQUIRE(existsBalancer(1, 1, 1)); }
  SECTION("Balancer 3-3-2 doesn't exist") { REQUIRE(!existsBalancer(3, 3, 2)); }
  SECTION("Balancer 4-4-4 exists") { REQUIRE(existsBalancer(4, 4, 4)); }
}

// vector<vector<double>> my_network = {{0.1, 0.2, 0.3, 0.4}, {0.5, 0.6, 0.7,
// 0.8}};

// vector<vector<double>> new_network = addSplitter(my_network, {0}, {1, -1});

// bool balancerExists = existsBalancer(5, 5, 6);
