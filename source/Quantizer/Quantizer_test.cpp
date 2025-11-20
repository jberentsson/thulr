#include "../Utils/MIDI.hpp"
#include "Quantizer.hpp"
#include <catch2/catch.hpp>
#include <iostream>
#include "Quantizer.hpp"

using namespace MIDI::Notes;

enum class Note : int { ON, OFF };

SCENARIO("create a new instance") {
  Quantizer qt = Quantizer();
  REQUIRE(true);
}

SCENARIO("one note"){
    Quantizer qt = Quantizer();
    qt.set_mode(Quantizer::QuantizeMode::ALL_NOTES);

  GIVEN("note is added") {
    REQUIRE(qt.add_note(NoteC4) == 0);

        WHEN("we quantize nearby notes"){
            THEN("notes quantize correctly"){
                REQUIRE(qt.quantize(NoteC4) == NoteC4);
                
                // Test round down.
                qt.set_round_direction(Quantizer::RoundDirection::UP);
                REQUIRE(qt.quantize(NoteB3) == NoteC4);
                qt.set_round_direction(Quantizer::RoundDirection::DOWN);
                REQUIRE(qt.quantize(NoteCS4) == NoteC4);

                // Test round up.
                qt.set_round_direction(Quantizer::RoundDirection::UP);
                REQUIRE(qt.quantize(NoteB3) == NoteC4);
                qt.set_round_direction(Quantizer::RoundDirection::DOWN);
                REQUIRE(qt.quantize(NoteCS4) == NoteC4);
            }
        }
    }
  }


SCENARIO("two notes"){
    Quantizer qt = Quantizer();
    qt.set_mode(Quantizer::QuantizeMode::ALL_NOTES);

  GIVEN("notes are added") {
    REQUIRE(qt.add_note(NoteC4) == 0);
    REQUIRE(qt.add_note(NoteG4) == 0);

        WHEN("we quantize nearby notes"){
            THEN("notes are quantized correctly"){
                REQUIRE(qt.quantize(NoteC4) == NoteC4);
                REQUIRE(qt.quantize(NoteG4) == NoteG4);
                
                qt.set_round_direction(Quantizer::RoundDirection::DOWN);
                REQUIRE(qt.quantize(NoteD4) == NoteC4);
                
                qt.set_round_direction(Quantizer::RoundDirection::UP);
                REQUIRE(qt.quantize(NoteD4) == NoteG4);
                
                qt.set_round_direction(Quantizer::RoundDirection::DOWN);
                REQUIRE(qt.quantize(NoteF4) == NoteC4);
                
                qt.set_round_direction(Quantizer::RoundDirection::UP);
                REQUIRE(qt.quantize(NoteF4) == NoteG4);
            }
        }
    }
  }


SCENARIO("edge cases") {
    Quantizer qt = Quantizer();
    qt.set_mode(Quantizer::QuantizeMode::ALL_NOTES);

  GIVEN("boundary notes are added") {
    qt.add_note(NoteC0);
    qt.add_note(NoteG10);

    THEN("boundary notes work correctly") {
      REQUIRE(qt.quantize(NoteC0) == 0);
      REQUIRE(qt.quantize(NoteG10) == NoteG10);
      REQUIRE(qt.quantize(NoteC0 - 1) == -1);  // Invalid low
      REQUIRE(qt.quantize(NoteG10 + 1) == -1); // Invalid high
    }
  }
}


SCENARIO("twelve notes mode") {
    Quantizer qt = Quantizer();
    qt.set_mode(Quantizer::QuantizeMode::TWELVE_NOTES);

  GIVEN("C3 is added") {
    qt.add_note(NoteC4);

        THEN("all C notes are active across octaves") {
            REQUIRE(qt.get_note(NoteC0) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(NoteC1) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(NoteC2) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(NoteC3) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(NoteC4) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(NoteC5) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(NoteC6) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(NoteC7) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(NoteC8) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(NoteC9) == Quantizer::Note::ON);
        }
    }
  }


SCENARIO("debug test") {
    Quantizer qt = Quantizer();
    qt.set_mode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("note C4 is added") {
        qt.add_note(NoteC4);
        
        THEN("verify basic functionality") {
            qt.set_round_direction(Quantizer::RoundDirection::UP);
            int result = qt.quantize(NoteB3);
            
            REQUIRE(qt.get_note(NoteC4) == Quantizer::Quantizer::Note::ON);  // Should be active
            REQUIRE(qt.get_note(NoteB3) == Quantizer::Quantizer::Note::OFF); // Should be inactive
            REQUIRE(result == NoteC4);                 // Should find note C4
        }
    }
  }


SCENARIO("set the range") {
  Quantizer qt = Quantizer();

    qt.set_mode(Quantizer::QuantizeMode::TWELVE_NOTES);
    qt.set_round_direction(Quantizer::RoundDirection::DOWN);
    qt.set_range(NoteC2, NoteC3);

    GIVEN("note C4 is added") {
        qt.add_note(NoteC2);
        
        THEN("verify basic functionality") {
            REQUIRE(qt.quantize(NoteB1) == NoteC2);
            REQUIRE(qt.quantize(NoteDS3) == NoteC3);
        }
    }
  }

