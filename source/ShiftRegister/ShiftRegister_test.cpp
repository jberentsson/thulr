#include "ShiftRegister.cpp"
#include <catch2/catch.hpp>
#include <iostream>

SCENARIO("create a new instance") { // NOLINT
  ShiftRegister sr = ShiftRegister();
  REQUIRE(true);
}

SCENARIO("fresh instance") { // NOLINT
  ShiftRegister sr = ShiftRegister();

  REQUIRE(true);

  WHEN("all bits are unset") {
    // Check if all the outputs are all zero.
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.get(i) == 0);
    }
  }

  WHEN("push data to register") {
    // Write all ones to the inactive register.
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.dataInput(1) == 1);
      REQUIRE(sr.step() == i);
    }

    // Data should not be visible yet (no activate called).
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.get(i) == 0);
    }

    // Manually activate to swap registers.
    REQUIRE(sr.activate() == 1);

    // Now data should be visible.
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.get(i) == 1);
    }

    // Write zeros to the new inactive register.
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.dataInput(0) == 0);
      REQUIRE(sr.step() == i);
    }

    // Data should still show ones.
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.get(i) == 1);
    }

    // Activate again to swap.
    REQUIRE(sr.activate() == 0);

    // Now should see zeros.
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.get(i) == 0);
    }
  }
}

SCENARIO("with a custom size") { // NOLINT
  ShiftRegister sr = ShiftRegister(5);

  REQUIRE(true);

  WHEN("all bits are unset") {
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.get(i) == 0);
    }
  }

  WHEN("push data to register") {
    // Write data to inactive register.
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.dataInput(1) == 1);
      REQUIRE(sr.step() == i);
    }

    // Data not visible yet.
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.get(i) == 0);
    }

    // Activate to swap.
    REQUIRE(sr.activate() == 1);

    // Now data visible.
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.get(i) == 1);
    }
  }
}

SCENARIO("test manual activate function") { // NOLINT
  ShiftRegister sr = ShiftRegister();

  WHEN("using activate to manually swap registers") {
    // Write partial data.
    for (int i = 0; i < 4; i++) {
      sr.dataInput(1);
      sr.step();
    }

    // Data not visible.
    for (int i = 0; i < sr.size(); i++) {
      REQUIRE(sr.get(i) == 0);
    }

    // Manually activate.
    REQUIRE(sr.activate() == 1);

    // Now partial data should be visible.
    for (int i = 0; i < 4; i++) {
      REQUIRE(sr.get(i) == 1);
    }
    // Remaining positions should be zero.
    for (int i = 4; i < sr.size(); i++) {
      REQUIRE(sr.get(i) == 0);
    }
  }
}

SCENARIO("test the data through functionality") { // NOLINT
  ShiftRegister sr = ShiftRegister();

  WHEN("add data to the register and check dataThrough") {
    // Fill the inactive register first
    for (int i = 0; i < sr.size(); i++) {
      // 1,2,3,4,5,6,7,8
      sr.dataInput(i + 1);
      sr.step();
    }

    // Activate to swap.
    sr.activate();

    // Fill the NEW inactive register to trigger dataThrough.
    for (int i = 0; i < sr.size(); i++) {
      // 9,10,11,12,13,14,15,16
      sr.dataInput(9 + i);
      sr.step();
    }

    // Write one more to trigger dataThrough.
    sr.dataInput(17);
    sr.step();

    // dataThrough should show the oldest value from active register.
    REQUIRE(sr.dataThrough() == 9);

    // Write another to see the next oldest value.
    sr.dataInput(18);
    sr.step();
    REQUIRE(sr.dataThrough() == 10);
  }
}

SCENARIO("test the bias function") { // NOLINT
  ShiftRegister sr = ShiftRegister();

  WHEN("we add more data than the register can handle") {
    // Fill inactive register
    for (int i = 1; i <= 8; i++) {
      // 10,20,30,...,80
      sr.dataInput(i * 10);
      sr.step();
    }

    sr.activate();

    // Fill to trigger dataThrough
    for (int i = 1; i <= 8; i++) {
      sr.dataInput(80 + i * 10);
      sr.step();
    }

    REQUIRE(sr.dataThrough() == 0);
    sr.dataInput(160);
    sr.step();

    REQUIRE(sr.dataThrough() == 90);

    REQUIRE(sr.get(0) == 10);
    REQUIRE(sr.get(1) == 20);
    REQUIRE(sr.get(2) == 30);
    REQUIRE(sr.get(3) == 40);
    REQUIRE(sr.get(4) == 50);
    REQUIRE(sr.get(5) == 60);
    REQUIRE(sr.get(6) == 70);
    REQUIRE(sr.get(7) == 80);
  }
}

SCENARIO("test continuous data flow") { // NOLINT
  ShiftRegister sr = ShiftRegister(4);

  WHEN("continuous data stream") {
    // Simple test: fill, activate, verify.
    for (int i = 1; i <= 4; i++) {
      sr.dataInput(i);
      sr.step();
    }

    // Activate to make data visible.
    sr.activate();

    // Verify we can read the data we wrote.
    REQUIRE(sr.get(0) == 1);
    REQUIRE(sr.get(1) == 2);
    REQUIRE(sr.get(2) == 3);
    REQUIRE(sr.get(3) == 4);

    // Write new data but don't activate yet.
    for (int i = 5; i <= 8; i++) {
      sr.dataInput(i);
      sr.step();
    }

    // Should still see old data.
    REQUIRE(sr.get(0) == 1);
    REQUIRE(sr.get(1) == 2);
    REQUIRE(sr.get(2) == 3);
    REQUIRE(sr.get(3) == 4);

    // Now activate to see new data.
    sr.activate();
    REQUIRE(sr.get(0) == 5);
    REQUIRE(sr.get(1) == 6);
    REQUIRE(sr.get(2) == 7);
    REQUIRE(sr.get(3) == 8);
  }
}
