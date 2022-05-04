#include "catch2/catch.hpp"
#include "exists_balancer.hpp"

TEST_CASE("profile_existsBalancer", "[!hide]") {
  REQUIRE(existsBalancer(4, 4, 4));
}

TEST_CASE("burn_existsBalancer", "[!hide]") {
  REQUIRE(existsBalancer(5, 5, 6));
}
