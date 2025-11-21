#include "BinaryCounter.hpp"
#include <catch2/catch.hpp>

SCENARIO("Test Initialize") {           // NOLINT
    BinaryCounter c = BinaryCounter(8); // NOLINT

    REQUIRE(true);
}

SCENARIO("Back And Forth") {            // NOLINT
    BinaryCounter c = BinaryCounter(4); // NOLINT
    REQUIRE(c.direction() == 1);
    REQUIRE(c.value() == 0);
    REQUIRE(c.toggleDirection() == Counter::Direction::REVERSE);
    REQUIRE(c.step() == 3);
    REQUIRE(c.toggleDirection() == Counter::Direction::FORWARD);
    REQUIRE(c.step() == 0);
    REQUIRE(c.value() == 0);
}

SCENARIO("Preset") {                     // NOLINT
    BinaryCounter c = BinaryCounter(16); // NOLINT
    REQUIRE(c.value() == 0);
    REQUIRE(c.set(6) == 6);
    REQUIRE(c.set(10) == 10);
    REQUIRE(c.set(-1) == 10);
    REQUIRE(c.step() == 11);
    REQUIRE(c.step() == 12);
    REQUIRE(c.step() == 13);
    REQUIRE(c.step() == 14);
    REQUIRE(c.step() == 15);
    REQUIRE(c.step() == 0);
}
