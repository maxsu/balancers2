// Test Runner for exists_balancers

#include <iostream>

#include "../lib/exists_balancer.hpp"
#include "catch2/catch.hpp"

TEST_CASE("existsBalancer") {
  SECTION("Balancer 1-1-1 exists") { REQUIRE(existsBalancer(1, 1, 1)); }
  SECTION("Balancer 3-3-2 doesn't exist") { REQUIRE(!existsBalancer(3, 3, 2)); }
  SECTION("Balancer 4-4-4 exists") { REQUIRE(existsBalancer(4, 4, 4)); }
}