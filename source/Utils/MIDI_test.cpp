#include <catch2/catch.hpp>
#include "MIDI.hpp"

using namespace MIDI::Notes;

/*
sSCENARIO("create a new midi message") {
    GIVEN("message is full of data") {
        auto mMessage = MIDI::Message(0x9FFFFF); // NOLINT
        REQUIRE(mMessage.type() == MIDI::MessageTypes::NOTE_ON);
        REQUIRE(mMessage.data(0) == 0xFF);
        REQUIRE(mMessage.data(1) == 0xFF);
        REQUIRE(mMessage.channel() == 0xF);
    }

    GIVEN("message has no data") {
        auto mMessage = MIDI::Message(0x900000); // NOLINT
        REQUIRE(mMessage.type() == MIDI::MessageTypes::NOTE_ON);
        REQUIRE(mMessage.data(0) == 0x0);
        REQUIRE(mMessage.data(1) == 0x0);
        REQUIRE(mMessage.channel() == 0x0);
    }

    GIVEN("message has some data") {
        auto mMessage = MIDI::Message(0x901337); // NOLINT
        REQUIRE(mMessage.type() == MIDI::MessageTypes::NOTE_ON);
        REQUIRE(mMessage.data(0) == 0x13);
        REQUIRE(mMessage.data(1) == 0x37);
        REQUIRE(mMessage.channel() == 0x0);
    }

    GIVEN("message is in the other format") {
        auto mMessage = MIDI::Message(128, 74, 64); // NOLINT
        REQUIRE(mMessage.type() == MIDI::MessageTypes::NOTE_OFF);
        REQUIRE(mMessage.data(0) == 74);
        REQUIRE(mMessage.data(1) == 64);
        REQUIRE(mMessage.channel() == 0x0);
    }
}
*/
