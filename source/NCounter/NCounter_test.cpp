#include "NCounter.hpp"
#include <catch2/catch.hpp>

SCENARIO("Test Initialize") {
  NCounter counter = NCounter(10); // NOLINT

  REQUIRE(true);
}

SCENARIO("Test Reset") {
  NCounter counter = NCounter(10); // NOLINT

  REQUIRE(counter.value() == 0);

  REQUIRE(counter.step() == 1);
  REQUIRE(counter.step() == 2);

  REQUIRE(counter.value() == 2);

  REQUIRE(counter.step() == 3);
  REQUIRE(counter.reset() == 0);

  REQUIRE(counter.value() == 0);
  REQUIRE(counter.step() == 1);
  REQUIRE(counter.value() == 1);

  REQUIRE(counter.reset() == 0);
}
