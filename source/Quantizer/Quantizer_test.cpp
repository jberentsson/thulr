#include "../Utils/MIDI.hpp"
#include "Quantizer.hpp"
#include <catch2/catch.hpp> // NOLINT
#include <iostream>

using namespace MIDI::Notes;

enum class Note : uint8_t { ON, OFF };

SCENARIO("create a new instance") {
    Quantizer qt = Quantizer();
    REQUIRE(true);
}

SCENARIO("one note"){
    Quantizer qt = Quantizer();
    qt.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("note is added") {
        REQUIRE(qt.addNote(NoteC4) == 0);

            WHEN("we quantize nearby notes"){
                THEN("notes quantize correctly"){
                    REQUIRE(qt.quantize(NoteC4) == NoteC4);
                    
                    // Test round down.
                    qt.setRoundDirection(Quantizer::RoundDirection::UP);
                    REQUIRE(qt.quantize(NoteB3) == NoteC4);
                    qt.setRoundDirection(Quantizer::RoundDirection::DOWN);
                    REQUIRE(qt.quantize(NoteCS4) == NoteC4);

                    // Test round up.
                    qt.setRoundDirection(Quantizer::RoundDirection::UP);
                    REQUIRE(qt.quantize(NoteB3) == NoteC4);
                    qt.setRoundDirection(Quantizer::RoundDirection::DOWN);
                    REQUIRE(qt.quantize(NoteCS4) == NoteC4);
            }
        }
    }
}


SCENARIO("two notes"){
    Quantizer qt = Quantizer();
    qt.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("notes are added") {
        REQUIRE(qt.addNote(NoteC4) == 0);
        REQUIRE(qt.addNote(NoteG4) == 0);

        WHEN("we quantize nearby notes"){
            THEN("notes are quantized correctly"){
                REQUIRE(qt.quantize(NoteC4) == NoteC4);
                REQUIRE(qt.quantize(NoteG4) == NoteG4);
                
                qt.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(qt.quantize(NoteD4) == NoteC4);
                
                qt.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(qt.quantize(NoteD4) == NoteG4);
                
                qt.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(qt.quantize(NoteF4) == NoteC4);
                
                qt.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(qt.quantize(NoteF4) == NoteG4);
            }
        }
    }
}

SCENARIO("edge cases") {
    Quantizer qt = Quantizer();
    qt.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("boundary notes are added") {
        qt.addNote(NoteC0);
        qt.addNote(NoteG10);

        THEN("boundary notes work correctly") {
            REQUIRE(qt.quantize(NoteC0) == 0);
            REQUIRE(qt.quantize(NoteG10) == NoteG10);
            REQUIRE(qt.quantize(NoteC0 - 1) == Quantizer::INVALID_NOTE);  // Invalid low
            REQUIRE(qt.quantize(NoteG10 + 1) == Quantizer::INVALID_NOTE); // Invalid high
        }
    }
}

SCENARIO("twelve notes mode") {
    Quantizer qt = Quantizer();
    qt.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);

    GIVEN("C3 is added") {
        qt.addNote(NoteC4);

        THEN("all C notes are active across octaves") {
            REQUIRE(qt.getNote(NoteC0) == Quantizer::Note::ON);
            REQUIRE(qt.getNote(NoteC1) == Quantizer::Note::ON);
            REQUIRE(qt.getNote(NoteC2) == Quantizer::Note::ON);
            REQUIRE(qt.getNote(NoteC3) == Quantizer::Note::ON);
            REQUIRE(qt.getNote(NoteC4) == Quantizer::Note::ON);
            REQUIRE(qt.getNote(NoteC5) == Quantizer::Note::ON);
            REQUIRE(qt.getNote(NoteC6) == Quantizer::Note::ON);
            REQUIRE(qt.getNote(NoteC7) == Quantizer::Note::ON);
            REQUIRE(qt.getNote(NoteC8) == Quantizer::Note::ON);
            REQUIRE(qt.getNote(NoteC9) == Quantizer::Note::ON);
        }
    }
}


SCENARIO("debug test") {
    Quantizer qt = Quantizer();
    qt.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("note C4 is added") {
        qt.addNote(NoteC4);
        
        THEN("verify basic functionality") {
            qt.setRoundDirection(Quantizer::RoundDirection::UP);
            int result = qt.quantize(NoteB3);
            
            REQUIRE(qt.getNote(NoteC4) == Quantizer::Quantizer::Note::ON);  // Should be active
            REQUIRE(qt.getNote(NoteB3) == Quantizer::Quantizer::Note::OFF); // Should be inactive
            REQUIRE(result == NoteC4);                 // Should find note C4
        }
    }
}


SCENARIO("set the range") {
  Quantizer qt = Quantizer();

    qt.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);
    qt.setRoundDirection(Quantizer::RoundDirection::DOWN);
    qt.setRange(NoteC2, NoteC3);

    GIVEN("note C4 is added") {
        qt.addNote(NoteC2);
        
        THEN("verify basic functionality") {
            REQUIRE(qt.quantize(NoteB1) == NoteC2);
            REQUIRE(qt.quantize(NoteDS3) == NoteC3);
        }
    }
}
