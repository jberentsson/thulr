#include "Counter.hpp"
#include <catch2/catch.hpp>

SCENARIO("Test Initialize") {
  Counter c = Counter(10); // NOLINT

  REQUIRE(true);
}

SCENARIO("Test Back Step") {
  Counter c = Counter(10); // NOLINT

  for (int i = 0; i < 10; i++) {
    c.back();
  }

  REQUIRE(c.value() == 0);
}

SCENARIO("Test Forward Step") {
  Counter c = Counter(10); // NOLINT

  c.direction();

  for (int i = 0; i < 10; i++) { // NOLINT
    c.forward();
  }

  REQUIRE(c.value() == 0);
}

SCENARIO("Test Reset") {
  Counter c = Counter(10); // NOLINT

  REQUIRE(c.value() == 0);

  REQUIRE(c.forward() == 1);
  REQUIRE(c.forward() == 2);

  REQUIRE(c.value() == 2);

  REQUIRE(c.forward() == 3);
  REQUIRE(c.reset() == 0);

  REQUIRE(c.value() == 0);
  REQUIRE(c.forward() == 1);
  REQUIRE(c.value() == 1);

  REQUIRE(c.reset() == 0);
}
