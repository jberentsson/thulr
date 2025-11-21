#include "../Utils/MIDI.hpp"
#include "Quantizer.hpp"
#include <catch2/catch.hpp> // NOLINT
#include <iostream>

using namespace MIDI::Notes;

enum class Note : uint8_t { ON, OFF };

SCENARIO("create a new instance") {
    Quantizer quantizer = Quantizer();
    REQUIRE(true);
}

SCENARIO("one note"){
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("note is added") {
        REQUIRE(quantizer.addNote(NoteC4) == 0);

            WHEN("we quantize nearby notes"){
                THEN("notes quantize correctly"){
                    REQUIRE(quantizer.quantize(NoteC4) == NoteC4);
                    
                    // Test round down.
                    quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
                    REQUIRE(quantizer.quantize(NoteB3) == NoteC4);
                    quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
                    REQUIRE(quantizer.quantize(NoteCS4) == NoteC4);

                    // Test round up.
                    quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
                    REQUIRE(quantizer.quantize(NoteB3) == NoteC4);
                    quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
                    REQUIRE(quantizer.quantize(NoteCS4) == NoteC4);
            }
        }
    }
}


SCENARIO("two notes"){
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("notes are added") {
        REQUIRE(quantizer.addNote(NoteC4) == 0);
        REQUIRE(quantizer.addNote(NoteG4) == 0);

        WHEN("we quantize nearby notes"){
            THEN("notes are quantized correctly"){
                REQUIRE(quantizer.quantize(NoteC4) == NoteC4);
                REQUIRE(quantizer.quantize(NoteG4) == NoteG4);
                
                quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(quantizer.quantize(NoteD4) == NoteC4);
                
                quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(quantizer.quantize(NoteD4) == NoteG4);
                
                quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(quantizer.quantize(NoteF4) == NoteC4);
                
                quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(quantizer.quantize(NoteF4) == NoteG4);
            }
        }
    }
}

SCENARIO("edge cases") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("boundary notes are added") {
        quantizer.addNote(NoteC0);
        quantizer.addNote(NoteG10);

        THEN("boundary notes work correctly") {
            REQUIRE(quantizer.quantize(NoteC0) == 0);
            REQUIRE(quantizer.quantize(NoteG10) == NoteG10);
            REQUIRE(quantizer.quantize(NoteC0 - 1) == Quantizer::INVALID_NOTE);  // Invalid low
            REQUIRE(quantizer.quantize(NoteG10 + 1) == Quantizer::INVALID_NOTE); // Invalid high
        }
    }
}

SCENARIO("twelve notes mode") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);

    GIVEN("C3 is added") {
        quantizer.addNote(NoteC4);

        THEN("all C notes are active across octaves") {
            REQUIRE(quantizer.getNote(NoteC0) == Quantizer::Note::ON);
            REQUIRE(quantizer.getNote(NoteC1) == Quantizer::Note::ON);
            REQUIRE(quantizer.getNote(NoteC2) == Quantizer::Note::ON);
            REQUIRE(quantizer.getNote(NoteC3) == Quantizer::Note::ON);
            REQUIRE(quantizer.getNote(NoteC4) == Quantizer::Note::ON);
            REQUIRE(quantizer.getNote(NoteC5) == Quantizer::Note::ON);
            REQUIRE(quantizer.getNote(NoteC6) == Quantizer::Note::ON);
            REQUIRE(quantizer.getNote(NoteC7) == Quantizer::Note::ON);
            REQUIRE(quantizer.getNote(NoteC8) == Quantizer::Note::ON);
            REQUIRE(quantizer.getNote(NoteC9) == Quantizer::Note::ON);
        }
    }
}


SCENARIO("debug test") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("note C4 is added") {
        quantizer.addNote(NoteC4);
        
        THEN("verify basic functionality") {
            quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
            int result = quantizer.quantize(NoteB3);
            
            REQUIRE(quantizer.getNote(NoteC4) == Quantizer::Quantizer::Note::ON);  // Should be active
            REQUIRE(quantizer.getNote(NoteB3) == Quantizer::Quantizer::Note::OFF); // Should be inactive
            REQUIRE(result == NoteC4);                 // Should find note C4
        }
    }
}


SCENARIO("set the range") {
  Quantizer quantizer = Quantizer();

    quantizer.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);
    quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
    quantizer.setRange(NoteC2, NoteC3);

    GIVEN("note C4 is added") {
        quantizer.addNote(NoteC2);
        
        THEN("verify basic functionality") {
            REQUIRE(quantizer.quantize(NoteB1) == NoteC2);
            REQUIRE(quantizer.quantize(NoteDS3) == NoteC3);
        }
    }
}
