#include "NCounter.hpp"
#include <catch2/catch.hpp>

SCENARIO("Test Initialize") {
  NCounter c = NCounter(10);

  REQUIRE(true);
}

SCENARIO("Test Reset") {
  NCounter c = NCounter(10);

  REQUIRE(c.value() == 0);

  REQUIRE(c.step() == 1);
  REQUIRE(c.step() == 2);

  REQUIRE(c.value() == 2);

  REQUIRE(c.step() == 3);
  REQUIRE(c.reset() == 0);

  REQUIRE(c.value() == 0);
  REQUIRE(c.step() == 1);
  REQUIRE(c.value() == 1);

  REQUIRE(c.reset() == 0);
}
