#include "Utils/MIDI.hpp"
#include "Quantizer.hpp"
#include <catch2/catch.hpp>

using namespace MIDI::Notes;

SCENARIO("create a new instance") {
    Quantizer quantizer = Quantizer();
    REQUIRE(true);
}

SCENARIO("one note") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("note is added") {
        REQUIRE(quantizer.noteCount() == 0);
        REQUIRE(quantizer.addNote(MIDI::Note(NoteC4)) == 0);
        REQUIRE(quantizer.noteCount() == 1);

        WHEN("we quantize nearby notes") {
            THEN("notes quantize correctly") {
                REQUIRE(quantizer.quantize(MIDI::Note(NoteC4)) == MIDI::Note(NoteC4));

                // Test round down.
                quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(quantizer.quantize(MIDI::Note(NoteB3)) == MIDI::Note(NoteC4));
                quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(quantizer.quantize(MIDI::Note(MIDI::Note(NoteCS4))) == MIDI::Note(NoteC4));

                // Test round up.
                quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(quantizer.quantize(MIDI::Note(NoteB3)) == MIDI::Note(NoteC4));
                quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(quantizer.quantize(MIDI::Note(NoteCS4)) == MIDI::Note(NoteC4));
            }
        }
    }
}

SCENARIO("two notes") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("notes are added") {
        REQUIRE(quantizer.addNote(MIDI::Note(NoteC4)) == MIDI::Note(0));
        REQUIRE(quantizer.addNote(MIDI::Note(NoteG4)) == MIDI::Note(0));

        WHEN("we quantize nearby notes") {
            THEN("notes are quantized correctly") {
                REQUIRE(quantizer.quantize(MIDI::Note(NoteC4)) == MIDI::Note(NoteC4));
                REQUIRE(quantizer.quantize(MIDI::Note(NoteG4)) == MIDI::Note(NoteG4));

                quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(quantizer.quantize(MIDI::Note(NoteD4)) == MIDI::Note(NoteC4));

                quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(quantizer.quantize(MIDI::Note(NoteD4)) == MIDI::Note(NoteG4));

                quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
                REQUIRE(quantizer.quantize(MIDI::Note(NoteF4)) == MIDI::Note(NoteC4));

                quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
                REQUIRE(quantizer.quantize(MIDI::Note(NoteF4)) == MIDI::Note(NoteG4));
            }
        }
    }
}

SCENARIO("edge cases") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("boundary notes are added") {
        quantizer.addNote(MIDI::Note(NoteC0));
        quantizer.addNote(MIDI::Note(NoteG10));

        THEN("boundary notes work correctly") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteC0)) == MIDI::Note(NoteC0));
            REQUIRE(quantizer.quantize(MIDI::Note(NoteG10)) == MIDI::Note(NoteG10));
            REQUIRE(quantizer.quantize(MIDI::Note(NoteC0 - 1)) == Quantizer::INVALID_NOTE);  // Invalid low
            REQUIRE(quantizer.quantize(MIDI::Note(NoteG10 + 1)) == Quantizer::INVALID_NOTE); // Invalid high
        }
    }
}

SCENARIO("twelve notes mode") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);

    GIVEN("C3 is added") {
        quantizer.addNote(MIDI::Note(NoteC4));

        THEN("all C notes are active across octaves") {
            REQUIRE(quantizer.getNote(MIDI::Note(NoteC0)) == Quantizer::NoteData::ON);
            REQUIRE(quantizer.getNote(MIDI::Note(NoteC1)) == Quantizer::NoteData::ON);
            REQUIRE(quantizer.getNote(MIDI::Note(NoteC2)) == Quantizer::NoteData::ON);
            REQUIRE(quantizer.getNote(MIDI::Note(NoteC3)) == Quantizer::NoteData::ON);
            REQUIRE(quantizer.getNote(MIDI::Note(NoteC4)) == Quantizer::NoteData::ON);
            REQUIRE(quantizer.getNote(MIDI::Note(NoteC5)) == Quantizer::NoteData::ON);
            REQUIRE(quantizer.getNote(MIDI::Note(NoteC6)) == Quantizer::NoteData::ON);
            REQUIRE(quantizer.getNote(MIDI::Note(NoteC7)) == Quantizer::NoteData::ON);
            REQUIRE(quantizer.getNote(MIDI::Note(NoteC8)) == Quantizer::NoteData::ON);
            REQUIRE(quantizer.getNote(MIDI::Note(NoteC9)) == Quantizer::NoteData::ON);
        }
    }
}

SCENARIO("debug test") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);

    GIVEN("note C4 is added") {
        quantizer.addNote(MIDI::Note(NoteC4));

        THEN("verify basic functionality") {
            quantizer.setRoundDirection(Quantizer::RoundDirection::UP);
            int result = quantizer.quantize(MIDI::Note(NoteB3));

            REQUIRE(quantizer.getNote(MIDI::Note(NoteC4)) == Quantizer::NoteData::ON);  // Should be active
            REQUIRE(quantizer.getNote(MIDI::Note(NoteB3)) == Quantizer::NoteData::OFF); // Should be inactive
            REQUIRE(result == MIDI::Note(NoteC4));                                             // Should find note C4
        }
    }
}

SCENARIO("set the range") {
    Quantizer quantizer = Quantizer();

    quantizer.setMode(Quantizer::QuantizeMode::TWELVE_NOTES);
    quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN);
    quantizer.setRange(MIDI::Note(NoteC2), MIDI::Note(NoteC3));

    GIVEN("note C2 is added") {
        quantizer.addNote(MIDI::Note(NoteC2));

        THEN("verify basic functionality") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteC2));
            REQUIRE(quantizer.quantize(MIDI::Note(NoteDS3)) == MIDI::Note(NoteC3));
        }
    }
}

SCENARIO("round with UP_OVERFLOW") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);
    quantizer.setRoundDirection(Quantizer::RoundDirection::UP_OVERFLOW);
    
    GIVEN("no notes have been added") {
        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteB1));
        }
    }

    GIVEN("note C2 is added") {
        quantizer.addNote(MIDI::Note(NoteC2));

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteCS2)) == MIDI::Note(NoteC2));
        }
    }
}

SCENARIO("round with DOWN_UNDERFLOW") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);
    quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN_UNDERFLOW);

    GIVEN("no notes have been added") {
        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteB1));
        }
    }

    GIVEN("note C4 is added") {
        quantizer.addNote(MIDI::Note(NoteC2));

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteC2));
        }
    }
}

SCENARIO("round with NEAREST") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);
    quantizer.setRoundDirection(Quantizer::RoundDirection::NEAREST);
    
    GIVEN("no notes have been added") {
        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteB1));
        }

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB2)) == MIDI::Note(NoteB2));
        }
    }

    GIVEN("note C4 is added") {
        quantizer.addNote(MIDI::Note(NoteC2));
        quantizer.addNote(MIDI::Note(NoteC3));

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteC2));
        }

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB2)) == MIDI::Note(NoteC3));
        }
    }
}

SCENARIO("first we round UP then DOWN") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);
    quantizer.setRoundDirection(Quantizer::RoundDirection::UP_DOWN);
    
    GIVEN("no notes have been added") {
        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteB1));
        }

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB2)) == MIDI::Note(NoteB2));
        }
    }

    GIVEN("note C4 is added") {
        quantizer.addNote(MIDI::Note(NoteC2));
        quantizer.addNote(MIDI::Note(NoteC3));

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteC2));
        }

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB3)) == MIDI::Note(NoteC3));
        }
    }
}

SCENARIO("first we round DOWN then UP") {
    Quantizer quantizer = Quantizer();
    quantizer.setMode(Quantizer::QuantizeMode::ALL_NOTES);
    quantizer.setRoundDirection(Quantizer::RoundDirection::DOWN_UP);
    
    GIVEN("no notes have been added") {
        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteB1));
        }

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB2)) == MIDI::Note(NoteB2));
        }
    }

    GIVEN("note C4 is added") {
        quantizer.addNote(MIDI::Note(NoteC2));
        quantizer.addNote(MIDI::Note(NoteC3));

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB2)) == MIDI::Note(NoteC2));
        }

        THEN("we try to round") {
            REQUIRE(quantizer.quantize(MIDI::Note(NoteB1)) == MIDI::Note(NoteC2));
        }
    }
}
