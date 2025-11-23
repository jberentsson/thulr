#include "ShiftRegister.cpp" // NOLINT
#include "ShiftRegister.hpp"
#include <catch2/catch.hpp>

SCENARIO("create a new instance") { // NOLINT
    auto shiftRegisterTest = ShiftRegister();
    REQUIRE(true);
}

SCENARIO("fresh instance") { // NOLINT
    auto shiftRegisterTest = ShiftRegister();

    REQUIRE(true);

    WHEN("all bits are unset") {
        // Check if all the outputs are all zero.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.get(i) == 0);
        }
    }

    WHEN("push data to register") {
        // Write all ones to the inactive register.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.dataInput(1) == 1);
            REQUIRE(shiftRegisterTest.step() == i);
        }

        // Data should not be visible yet (no activate called).
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.get(i) == 0);
        }

        // Manually activate to swap registers.
        REQUIRE(shiftRegisterTest.activate() == 1);

        // Now data should be visible.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.get(i) == 1);
        }

        // Write zeros to the new inactive register.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.dataInput(0) == 0);
            REQUIRE(shiftRegisterTest.step() == i);
        }

        // Data should still show ones.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.get(i) == 1);
        }

        // Activate again to swap.
        REQUIRE(shiftRegisterTest.activate() == 0);

        // Now should see zeros.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.get(i) == 0);
        }
    }
}

SCENARIO("with a custom size") {               // NOLINT
    auto shiftRegisterTest = ShiftRegister(5); // NOLINT

    REQUIRE(true);

    WHEN("all bits are unset") {
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.get(i) == 0);
        }
    }

    WHEN("push data to register") {
        // Write data to inactive register.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.dataInput(1) == 1);
            REQUIRE(shiftRegisterTest.step() == i);
        }

        // Data not visible yet.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.get(i) == 0);
        }

        // Activate to swap.
        REQUIRE(shiftRegisterTest.activate() == 1);

        // Now data visible.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.get(i) == 1);
        }
    }
}

SCENARIO("test manual activate function") { // NOLINT
    auto shiftRegisterTest = ShiftRegister();

    WHEN("using activate to manually swap registers") {
        // Write partial data.
        for (int i = 0; i < 4; i++) {
            shiftRegisterTest.dataInput(1);
            shiftRegisterTest.step();
        }

        // Data not visible.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.get(i) == 0);
        }

        // Manually activate.
        REQUIRE(shiftRegisterTest.activate() == 1);

        // Now partial data should be visible.
        for (int i = 0; i < 4; i++) {
            REQUIRE(shiftRegisterTest.get(i) == 1);
        }
        // Remaining positions should be zero.
        for (int i = 4; i < shiftRegisterTest.size(); i++) {
            REQUIRE(shiftRegisterTest.get(i) == 0);
        }
    }
}

SCENARIO("test the data through functionality") { // NOLINT
    auto shiftRegisterTest = ShiftRegister();

    WHEN("add data to the register and check dataThrough") {
        // Fill the inactive register first
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            // 1,2,3,4,5,6,7,8
            shiftRegisterTest.dataInput(i + 1);
            shiftRegisterTest.step();
        }

        // Activate to swap.
        shiftRegisterTest.activate();

        // Fill the NEW inactive register to trigger dataThrough.
        for (int i = 0; i < shiftRegisterTest.size(); i++) {
            // 9,10,11,12,13,14,15,16
            shiftRegisterTest.dataInput(9 + i); // NOLINT
            shiftRegisterTest.step();
        }

        // Write one more to trigger dataThrough.
        shiftRegisterTest.dataInput(17); // NOLINT
        shiftRegisterTest.step();

        // dataThrough should show the oldest value from active register.
        REQUIRE(shiftRegisterTest.dataThrough() == 9);

        // Write another to see the next oldest value.
        shiftRegisterTest.dataInput(18); // NOLINT
        shiftRegisterTest.step();
        REQUIRE(shiftRegisterTest.dataThrough() == 10);
    }
}

SCENARIO("test the bias function") { // NOLINT
    auto shiftRegisterTest = ShiftRegister();

    WHEN("we add more data than the register can handle") {
        // Fill inactive register
        for (int i = 1; i <= 8; i++) { // NOLINT
            // 10,20,30,...,80
            shiftRegisterTest.dataInput(i * 10); // NOLINT
            shiftRegisterTest.step();
        }

        shiftRegisterTest.activate();

        // Fill to trigger dataThrough
        for (int i = 1; i <= 8; i++) {                // NOLINT
            shiftRegisterTest.dataInput(80 + i * 10); // NOLINT
            shiftRegisterTest.step();
        }

        REQUIRE(shiftRegisterTest.dataThrough() == 0);
        shiftRegisterTest.dataInput(160); // NOLINT
        shiftRegisterTest.step();

        REQUIRE(shiftRegisterTest.dataThrough() == 90);

        REQUIRE(shiftRegisterTest.get(0) == 10);
        REQUIRE(shiftRegisterTest.get(1) == 20);
        REQUIRE(shiftRegisterTest.get(2) == 30);
        REQUIRE(shiftRegisterTest.get(3) == 40);
        REQUIRE(shiftRegisterTest.get(4) == 50);
        REQUIRE(shiftRegisterTest.get(5) == 60);
        REQUIRE(shiftRegisterTest.get(6) == 70);
        REQUIRE(shiftRegisterTest.get(7) == 80);
    }
}

SCENARIO("test continuous data flow") { // NOLINT
    auto shiftRegisterTest = ShiftRegister(4);

    WHEN("continuous data stream") {
        // Simple test: fill, activate, verify.
        for (int i = 1; i <= 4; i++) {
            shiftRegisterTest.dataInput(i);
            shiftRegisterTest.step();
        }

        // Activate to make data visible.
        shiftRegisterTest.activate();

        // Verify we can read the data we wrote.
        REQUIRE(shiftRegisterTest.get(0) == 1);
        REQUIRE(shiftRegisterTest.get(1) == 2);
        REQUIRE(shiftRegisterTest.get(2) == 3);
        REQUIRE(shiftRegisterTest.get(3) == 4);

        // Write new data but don't activate yet.
        for (int i = 5; i <= 8; i++) { // NOLINT
            shiftRegisterTest.dataInput(i);
            shiftRegisterTest.step();
        }

        // Should still see old data.
        REQUIRE(shiftRegisterTest.get(0) == 1);
        REQUIRE(shiftRegisterTest.get(1) == 2);
        REQUIRE(shiftRegisterTest.get(2) == 3);
        REQUIRE(shiftRegisterTest.get(3) == 4);

        // Now activate to see new data.
        shiftRegisterTest.activate();
        REQUIRE(shiftRegisterTest.get(0) == 5);
        REQUIRE(shiftRegisterTest.get(1) == 6);
        REQUIRE(shiftRegisterTest.get(2) == 7);
        REQUIRE(shiftRegisterTest.get(3) == 8);
    }
}
