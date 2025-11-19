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
    REQUIRE(qt.add_note(C4) == 0);

        WHEN("we quantize nearby notes"){
            THEN("notes quantize correctly"){
                REQUIRE(qt.quantize(C4) == C4);
                
                // Test round down.
                qt.set_round_direction(Quantizer::RoundDirection::UP);
                REQUIRE(qt.quantize(B3) == C4);
                qt.set_round_direction(Quantizer::RoundDirection::DOWN);
                REQUIRE(qt.quantize(CS4) == C4);

                // Test round up.
                qt.set_round_direction(Quantizer::RoundDirection::UP);
                REQUIRE(qt.quantize(B3) == C4);
                qt.set_round_direction(Quantizer::RoundDirection::DOWN);
                REQUIRE(qt.quantize(CS4) == C4);
            }
        }
    }
  }


SCENARIO("two notes"){
    Quantizer qt = Quantizer();
    qt.set_mode(Quantizer::QuantizeMode::ALL_NOTES);

  GIVEN("notes are added") {
    REQUIRE(qt.add_note(C4) == 0);
    REQUIRE(qt.add_note(G4) == 0);

        WHEN("we quantize nearby notes"){
            THEN("notes are quantized correctly"){
                REQUIRE(qt.quantize(C4) == C4);
                REQUIRE(qt.quantize(G4) == G4);
                
                qt.set_round_direction(Quantizer::RoundDirection::DOWN);
                REQUIRE(qt.quantize(D4) == C4);
                
                qt.set_round_direction(Quantizer::RoundDirection::UP);
                REQUIRE(qt.quantize(D4) == G4);
                
                qt.set_round_direction(Quantizer::RoundDirection::DOWN);
                REQUIRE(qt.quantize(F4) == C4);
                
                qt.set_round_direction(Quantizer::RoundDirection::UP);
                REQUIRE(qt.quantize(F4) == G4);
            }
        }
    }
  }


SCENARIO("edge cases") {
    Quantizer qt = Quantizer();
    qt.set_mode(Quantizer::QuantizeMode::ALL_NOTES);

  GIVEN("boundary notes are added") {
    qt.add_note(C0);
    qt.add_note(G10);

    THEN("boundary notes work correctly") {
      REQUIRE(qt.quantize(C0) == 0);
      REQUIRE(qt.quantize(G10) == G10);
      REQUIRE(qt.quantize(C0 - 1) == -1);  // Invalid low
      REQUIRE(qt.quantize(G10 + 1) == -1); // Invalid high
    }
  }
}


SCENARIO("twelve notes mode") {
    Quantizer qt = Quantizer();
    qt.set_mode(Quantizer::QuantizeMode::TWELVE_NOTES);

  GIVEN("C3 is added") {
    qt.add_note(C4);

        THEN("all C notes are active across octaves") {
            REQUIRE(qt.get_note(C0) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(C1) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(C2) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(C3) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(C4) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(C5) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(C6) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(C7) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(C8) == Quantizer::Note::ON);
            REQUIRE(qt.get_note(C9) == Quantizer::Note::ON);
        }
    }
  }


SCENARIO("debug test") {
    Quantizer qt = Quantizer();
    qt.set_mode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("note C4 is added") {
        qt.add_note(C4);
        
        THEN("verify basic functionality") {
            qt.set_round_direction(Quantizer::RoundDirection::UP);
            int result = qt.quantize(B3);
            
            REQUIRE(qt.get_note(C4) == Quantizer::Quantizer::Note::ON);  // Should be active
            REQUIRE(qt.get_note(B3) == Quantizer::Quantizer::Note::OFF); // Should be inactive
            REQUIRE(result == C4);                 // Should find note C4
        }
    }
  }


SCENARIO("set the range") {
  Quantizer qt = Quantizer();

    qt.set_mode(Quantizer::QuantizeMode::TWELVE_NOTES);
    qt.set_round_direction(Quantizer::RoundDirection::DOWN);
    qt.set_range(C2, C3);

    GIVEN("note C4 is added") {
        qt.add_note(C2);
        
        THEN("verify basic functionality") {
            REQUIRE(qt.quantize(B1) == C2);
            REQUIRE(qt.quantize(DS3) == C3);
        }
    }
  }

