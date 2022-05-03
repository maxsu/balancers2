#include "../lib/exists_balancer.hpp"
#include "catch2/catch.hpp"

TEST_CASE("profile_existsBalancer", "[!hide]") {
  REQUIRE(existsBalancer(4, 4, 4));
}

TEST_CASE("burn_existsBalancer", "[!hide]") {
  REQUIRE(existsBalancer(5, 5, 6));
}
