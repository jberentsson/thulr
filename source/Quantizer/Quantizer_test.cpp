#include "Utils/MIDI.hpp"
#include "Quantizer.hpp"
#include <catch2/catch.hpp>

using Note = MIDI::Note;
using namespace MIDI::Notes;

SCENARIO("create a new instance") {
    Quantizer quantizerTestObject = Quantizer();
    REQUIRE(true);
}

SCENARIO("one note") {
    Quantizer quantizerTestObject = Quantizer();
    quantizerTestObject.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("note is added") {
        REQUIRE(quantizerTestObject.noteCount() == 0);
        REQUIRE(quantizerTestObject.addNote(MIDI::Note(NoteC4)) == 0);
        REQUIRE(quantizerTestObject.noteCount() == 1);

        WHEN("we quantize nearby notes") {
            THEN("notes quantize correctly") {
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteC4)) == MIDI::Note(NoteC4));
                REQUIRE(quantizerTestObject.getLastNote() != nullptr);
                REQUIRE(*quantizerTestObject.getLastNote() == MIDI::Note(NoteC4));

                // Test round down.
                quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB3)) == MIDI::Note(NoteC4));
                quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(MIDI::Note(NoteCS4))) == MIDI::Note(NoteC4));

                // Test round up.
                quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB3)) == MIDI::Note(NoteC4));
                quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteCS4)) == MIDI::Note(NoteC4));
            }
        }
    }
}

SCENARIO("Specific failing test case") {
    Quantizer quantizerTestObject;
    
    auto result = quantizerTestObject.quantize(MIDI::Note(NoteC4));
    
    REQUIRE(result == MIDI::Note(NoteC4));
    REQUIRE(quantizerTestObject.getLastNote() != nullptr);
    REQUIRE(*(quantizerTestObject.getLastNote()) == MIDI::Note(NoteC4));
}

SCENARIO("two notes") {
    Quantizer quantizerTestObject = Quantizer();
    quantizerTestObject.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("notes are added") {
        REQUIRE(quantizerTestObject.addNote(MIDI::Note(NoteC4)) == MIDI::Note(0));
        REQUIRE(quantizerTestObject.addNote(MIDI::Note(NoteG4)) == MIDI::Note(0));

        WHEN("we quantize nearby notes") {
            THEN("notes are quantized correctly") {
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteC4)) == MIDI::Note(NoteC4));
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteG4)) == MIDI::Note(NoteG4));

                quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteD4)) == MIDI::Note(NoteC4));

                quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteD4)) == MIDI::Note(NoteG4));

                quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteF4)) == MIDI::Note(NoteC4));

                quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteF4)) == MIDI::Note(NoteG4));
            }
        }
    }
}

SCENARIO("edge cases") {
    Quantizer quantizerTestObject = Quantizer();
    quantizerTestObject.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("boundary notes are added") {
        quantizerTestObject.addNote(MIDI::Note(NoteC0));
        quantizerTestObject.addNote(MIDI::Note(NoteG10));
        quantizerTestObject.setRange(MIDI::Note(0), MIDI::Note(127)); // NOLINT

        THEN("boundary notes work correctly") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteC0)) == MIDI::Note(NoteC0));
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteG10)) == MIDI::Note(NoteG10));
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteC0 - 1)) == MIDI::Note(NoteG10));  // Invalid low
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteG10 + 1)) == MIDI::Note(NoteG10)); // Invalid high
        }
    }
}

SCENARIO("twelve notes mode") {
    Quantizer quantizerTestObject = Quantizer();
    quantizerTestObject.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);

    GIVEN("C3 is added") {
        quantizerTestObject.addNote(MIDI::Note(NoteC4));

        THEN("all C notes are active across octaves") {
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC0)) == NoteC0);
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC1)) == NoteC1);
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC2)) == NoteC2);
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC3)) == NoteC3);
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC4)) == NoteC4);
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC5)) == NoteC5);
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC6)) == NoteC6);
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC7)) == NoteC7);
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC8)) == NoteC8);
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC9)) == NoteC9);
        }
    }
}

SCENARIO("debug test") {
    Quantizer quantizerTestObject = Quantizer();
    quantizerTestObject.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("note C4 is added") {
        quantizerTestObject.addNote(MIDI::Note(NoteC4));

        THEN("verify basic functionality") {
            quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::UP);
            int result = quantizerTestObject.quantize(MIDI::Note(NoteB3));

            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteC4)) == MIDI::Note(NoteC4));             // Should be active
            REQUIRE(quantizerTestObject.getNote(MIDI::Note(NoteB3)) == MIDI::Note(MIDI::INVALID_NOTE)); // Should be inactive
            REQUIRE(result == MIDI::Note(NoteC4));                                                      // Should find note C4
        }
    }
}

SCENARIO("set the range") {
    Quantizer quantizerTestObject = Quantizer();

    quantizerTestObject.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);
    quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::DOWN);
    quantizerTestObject.setRange(MIDI::Note(NoteC2), MIDI::Note(NoteC3));

    GIVEN("note C2 is added") {
        quantizerTestObject.addNote(MIDI::Note(NoteC2));

        THEN("verify basic functionality") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteC2));
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteDS3)) == MIDI::Note(NoteC2));
        }
    }
}

SCENARIO("round with UP_OVERFLOW") {
    Quantizer quantizerTestObject = Quantizer();
    quantizerTestObject.setMode(Quantizer::QuantizeMode::ALL_NOTES);
    quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::UP_OVERFLOW);
    
    GIVEN("no notes have been added") {
        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteB1));
        }
    }

    GIVEN("note C2 is added") {
        quantizerTestObject.addNote(MIDI::Note(NoteC2));

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteCS2)) == MIDI::Note(NoteC2));
        }
    }
}

SCENARIO("round with DOWN_UNDERFLOW") {
    Quantizer quantizerTestObject = Quantizer();
    quantizerTestObject.setMode(Quantizer::QuantizeMode::ALL_NOTES);
    quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::DOWN_UNDERFLOW);

    GIVEN("no notes have been added") {
        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteB1));
        }
    }

    GIVEN("note C4 is added") {
        quantizerTestObject.addNote(MIDI::Note(NoteC2));

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteC2));
        }
    }
}

SCENARIO("round with NEAREST") {
    Quantizer quantizerTestObject = Quantizer();
    quantizerTestObject.setMode(Quantizer::QuantizeMode::ALL_NOTES);
    quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::NEAREST);
    
    GIVEN("no notes have been added") {
        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteB1));
        }

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB2)) == MIDI::Note(NoteB2));
        }
    }

    GIVEN("note C4 is added") {
        quantizerTestObject.addNote(MIDI::Note(NoteC2));
        quantizerTestObject.addNote(MIDI::Note(NoteC3));

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteC2));
        }

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB2)) == MIDI::Note(NoteC3));
        }
    }
}

SCENARIO("first we round UP then DOWN") {
    Quantizer quantizerTestObject = Quantizer();
    quantizerTestObject.setMode(Quantizer::QuantizeMode::ALL_NOTES);
    quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::UP_DOWN);
    
    GIVEN("no notes have been added") {
        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteB1));
        }

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB2)) == MIDI::Note(NoteB2));
        }
    }

    GIVEN("note C4 is added") {
        quantizerTestObject.addNote(MIDI::Note(NoteC2));
        quantizerTestObject.addNote(MIDI::Note(NoteC3));

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteC2));
        }

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB3)) == MIDI::Note(NoteC3));
        }
    }
}

SCENARIO("first we round DOWN then UP") {
    Quantizer quantizerTestObject = Quantizer();
    quantizerTestObject.setMode(Quantizer::QuantizeMode::ALL_NOTES);
    quantizerTestObject.setRoundDirection(Quantizer::RoundDirection::DOWN_UP);
    
    GIVEN("no notes have been added") {
        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteB1));
        }

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB2)) == MIDI::Note(NoteB2));
        }
    }

    GIVEN("note C4 is added") {
        quantizerTestObject.addNote(MIDI::Note(NoteC2));
        quantizerTestObject.addNote(MIDI::Note(NoteC3));

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB2)) == MIDI::Note(NoteC2));
        }

        THEN("we try to round") {
            REQUIRE(quantizerTestObject.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteC2));
        }
    }
}

SCENARIO("clear the notes") {
    GIVEN("OCTAVE MODE") {
        Quantizer quantizerTestObject = Quantizer();
        
        quantizerTestObject.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);

        WHEN("add and remove notes") {
            for (int i = 0; i <= 11; i++) { // NOLINT
                REQUIRE_NOTHROW(quantizerTestObject.addNote(MIDI::Note(i)));
            }

            REQUIRE(quantizerTestObject.noteCount() == 128);

            for (int i = 0; i <= 11; i++) { // NOLINT
                REQUIRE_NOTHROW(quantizerTestObject.deleteNote(Note(i)));
            }

            REQUIRE(quantizerTestObject.noteCount() == 0);            
        }
    }
    
    GIVEN("ALL KEYS MODE") {
        Quantizer quantizerTestObject = Quantizer();

        quantizerTestObject.setMode(Quantizer::QuantizeMode::ALL_NOTES);
        
        WHEN("add and remove notes") {
            REQUIRE(quantizerTestObject.noteCount() == 0);
            for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
                REQUIRE(quantizerTestObject.noteCount() == i);
                REQUIRE_NOTHROW(quantizerTestObject.addNote(MIDI::Note(i)));
                REQUIRE(quantizerTestObject.noteCount() == i + 1);
            }

            REQUIRE(quantizerTestObject.noteCount() == 128);

            for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
                REQUIRE_NOTHROW(quantizerTestObject.deleteNote(MIDI::Note(i)));
            }

            REQUIRE(quantizerTestObject.noteCount() == 0);
        }
    }
}

