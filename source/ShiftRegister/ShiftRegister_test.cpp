#include <catch2/catch.hpp>
#include <iostream>
#include "ShiftRegister.hpp"
#include "ShiftRegister.cpp" // NOLINT

SCENARIO("create a new instance") { // NOLINT
  ShiftRegister shiftRegister = ShiftRegister();
  REQUIRE(true);
}

SCENARIO("fresh instance") { // NOLINT
  ShiftRegister shiftRegister = ShiftRegister();

  REQUIRE(true);

  WHEN("all bits are unset") {
    // Check if all the outputs are all zero.
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.get(i) == 0);
    }
  }

  WHEN("push data to register") {
    // Write all ones to the inactive register.
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.dataInput(1) == 1);
      REQUIRE(shiftRegister.step() == i);
    }

    // Data should not be visible yet (no activate called).
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.get(i) == 0);
    }

    // Manually activate to swap registers.
    REQUIRE(shiftRegister.activate() == 1);

    // Now data should be visible.
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.get(i) == 1);
    }

    // Write zeros to the new inactive register.
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.dataInput(0) == 0);
      REQUIRE(shiftRegister.step() == i);
    }

    // Data should still show ones.
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.get(i) == 1);
    }

    // Activate again to swap.
    REQUIRE(shiftRegister.activate() == 0);

    // Now should see zeros.
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.get(i) == 0);
    }
  }
}

SCENARIO("with a custom size") { // NOLINT
  ShiftRegister shiftRegister = ShiftRegister(5);

  REQUIRE(true);

  WHEN("all bits are unset") {
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.get(i) == 0);
    }
  }

  WHEN("push data to register") {
    // Write data to inactive register.
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.dataInput(1) == 1);
      REQUIRE(shiftRegister.step() == i);
    }

    // Data not visible yet.
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.get(i) == 0);
    }

    // Activate to swap.
    REQUIRE(shiftRegister.activate() == 1);

    // Now data visible.
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.get(i) == 1);
    }
  }
}

SCENARIO("test manual activate function") { // NOLINT
  ShiftRegister shiftRegister = ShiftRegister();

  WHEN("using activate to manually swap registers") {
    // Write partial data.
    for (int i = 0; i < 4; i++) {
      shiftRegister.dataInput(1);
      shiftRegister.step();
    }

    // Data not visible.
    for (int i = 0; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.get(i) == 0);
    }

    // Manually activate.
    REQUIRE(shiftRegister.activate() == 1);

    // Now partial data should be visible.
    for (int i = 0; i < 4; i++) {
      REQUIRE(shiftRegister.get(i) == 1);
    }
    // Remaining positions should be zero.
    for (int i = 4; i < shiftRegister.size(); i++) {
      REQUIRE(shiftRegister.get(i) == 0);
    }
  }
}

SCENARIO("test the data through functionality") { // NOLINT
  ShiftRegister shiftRegister = ShiftRegister();

  WHEN("add data to the register and check dataThrough") {
    // Fill the inactive register first
    for (int i = 0; i < shiftRegister.size(); i++) {
      // 1,2,3,4,5,6,7,8
      shiftRegister.dataInput(i + 1);
      shiftRegister.step();
    }

    // Activate to swap.
    shiftRegister.activate();

    // Fill the NEW inactive register to trigger dataThrough.
    for (int i = 0; i < shiftRegister.size(); i++) {
      // 9,10,11,12,13,14,15,16
      shiftRegister.dataInput(9 + i);
      shiftRegister.step();
    }

    // Write one more to trigger dataThrough.
    shiftRegister.dataInput(17);
    shiftRegister.step();

    // dataThrough should show the oldest value from active register.
    REQUIRE(shiftRegister.dataThrough() == 9);

    // Write another to see the next oldest value.
    shiftRegister.dataInput(18);
    shiftRegister.step();
    REQUIRE(shiftRegister.dataThrough() == 10);
  }
}

SCENARIO("test the bias function") { // NOLINT
  ShiftRegister shiftRegister = ShiftRegister();

  WHEN("we add more data than the register can handle") {
    // Fill inactive register
    for (int i = 1; i <= 8; i++) {
      // 10,20,30,...,80
      shiftRegister.dataInput(i * 10);
      shiftRegister.step();
    }

    shiftRegister.activate();

    // Fill to trigger dataThrough
    for (int i = 1; i <= 8; i++) {
      shiftRegister.dataInput(80 + i * 10);
      shiftRegister.step();
    }

    REQUIRE(shiftRegister.dataThrough() == 0);
    shiftRegister.dataInput(160);
    shiftRegister.step();

    REQUIRE(shiftRegister.dataThrough() == 90);

    REQUIRE(shiftRegister.get(0) == 10);
    REQUIRE(shiftRegister.get(1) == 20);
    REQUIRE(shiftRegister.get(2) == 30);
    REQUIRE(shiftRegister.get(3) == 40);
    REQUIRE(shiftRegister.get(4) == 50);
    REQUIRE(shiftRegister.get(5) == 60);
    REQUIRE(shiftRegister.get(6) == 70);
    REQUIRE(shiftRegister.get(7) == 80);
  }
}

SCENARIO("test continuous data flow") { // NOLINT
  ShiftRegister shiftRegister = ShiftRegister(4);

  WHEN("continuous data stream") {
    // Simple test: fill, activate, verify.
    for (int i = 1; i <= 4; i++) {
      shiftRegister.dataInput(i);
      shiftRegister.step();
    }

    // Activate to make data visible.
    shiftRegister.activate();

    // Verify we can read the data we wrote.
    REQUIRE(shiftRegister.get(0) == 1);
    REQUIRE(shiftRegister.get(1) == 2);
    REQUIRE(shiftRegister.get(2) == 3);
    REQUIRE(shiftRegister.get(3) == 4);

    // Write new data but don't activate yet.
    for (int i = 5; i <= 8; i++) { // NOLINT
      shiftRegister.dataInput(i);
      shiftRegister.step();
    }

    // Should still see old data.
    REQUIRE(shiftRegister.get(0) == 1);
    REQUIRE(shiftRegister.get(1) == 2);
    REQUIRE(shiftRegister.get(2) == 3);
    REQUIRE(shiftRegister.get(3) == 4);

    // Now activate to see new data.
    shiftRegister.activate();
    REQUIRE(shiftRegister.get(0) == 5);
    REQUIRE(shiftRegister.get(1) == 6);
    REQUIRE(shiftRegister.get(2) == 7);
    REQUIRE(shiftRegister.get(3) == 8);
  }
}
