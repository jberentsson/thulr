#include <catch2/catch.hpp>
#include "BinaryCounter.hpp"

SCENARIO("Test Initialize") {
	BinaryCounter c = BinaryCounter(8);

	REQUIRE(true);
}

SCENARIO("Back And Forth") {
	BinaryCounter c = BinaryCounter(4);
	REQUIRE(c.direction() == 1);
	REQUIRE(c.value() == 0);
	REQUIRE(c.toggle_direction() == 0);
	REQUIRE(c.step() == 15);
	REQUIRE(c.toggle_direction() == 1);
	REQUIRE(c.step() == 0);
	REQUIRE(c.value() == 0);
}

SCENARIO("Preset") {
	BinaryCounter c = BinaryCounter(4);
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
