#include "../Utils/MIDI.hpp"
#include "RandomOctave.hpp"
#include <catch2/catch.hpp>

using namespace MIDI::Notes;

TEST_CASE("RandomOctave basic functionality") {
    RandomOctave randomOctave;

    SECTION("Note on adds a note") {
        int result = randomOctave.note(NoteC5, 100); // NOLINT
        REQUIRE(result == 1);
        REQUIRE(randomOctave.getActiveNotes().size() == 1);
    }

    SECTION("Note off clears notes by pitch class") {
        randomOctave.note(NoteC4, 100); // NOLINT
        randomOctave.note(NoteC5, 100); // NOLINT
        REQUIRE(randomOctave.getActiveNotes().size() == 2);

        int cleared = randomOctave.clearNotesByPitchClass(NoteC5);
        REQUIRE(cleared == 2);
        REQUIRE(randomOctave.getActiveNotes().empty());
    }

    SECTION("Remove all clears all notes") {
        randomOctave.note(NoteC5, 100); // NOLINT
        randomOctave.note(NoteE5, 100); // NOLINT
        REQUIRE(randomOctave.getActiveNotes().size() == 2);

        unsigned int cleared = randomOctave.removeAll();
        REQUIRE(cleared == 2);
        REQUIRE(randomOctave.getActiveNotes().empty());
    }

    SECTION("Range setting works") {
        randomOctave.setRandomRange(2, 5); // NOLINT
        REQUIRE(randomOctave.minCapacity() == 2);
        REQUIRE(randomOctave.maxCapacity() == 5);
    }
};

