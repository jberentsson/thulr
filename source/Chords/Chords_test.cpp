#include <catch2/catch.hpp>
#include "Chords.hpp"

using namespace MIDI::Notes;

SCENARIO("create a new instance") {
    Chords chordsTest = Chords();
    REQUIRE(true);
}

SCENARIO("parsse some notes") {
    Chords chordsTest = Chords();
    REQUIRE(true);

    GIVEN("we play a note with no notes added") {
        REQUIRE(chordsTest.note(NoteC4, 100) == 0);
        REQUIRE(chordsTest.noteQueue().empty());
    }

    GIVEN("we play a note with notes added") {
        REQUIRE(chordsTest.reciveNotes());
        REQUIRE(chordsTest.isRecievingNotes());

        // Assign a chord to C4.
        REQUIRE(chordsTest.note(NoteC4, 100) == 0);
        REQUIRE(chordsTest.note(NoteC4, 0) == 0);

        // Play a C Major chord.
        REQUIRE(chordsTest.note(NoteC4, 100) == 0);
        REQUIRE(chordsTest.note(NoteE4, 100) == 0);
        REQUIRE(chordsTest.note(NoteG4, 100) == 0);

        // Release the notes.
        REQUIRE(chordsTest.note(NoteC4, 0) == 0);
        REQUIRE(chordsTest.note(NoteG4, 0) == 0);
        REQUIRE(chordsTest.note(NoteE4, 0) == 0);

        REQUIRE(chordsTest.noteQueue().empty());
        REQUIRE(!chordsTest.isRecievingNotes());

        // Play a single note.
        REQUIRE(chordsTest.note(NoteC4, 100) == 0);

        // And a C Major chord is played.
        REQUIRE(!chordsTest.noteQueue().empty());
        REQUIRE(chordsTest.noteQueue().size() == 3);

        REQUIRE(*chordsTest.noteQueue().at(0) == NoteC4);
        REQUIRE(*chordsTest.noteQueue().at(1) == NoteE4);
        REQUIRE(*chordsTest.noteQueue().at(2) == NoteG4);

        REQUIRE_NOTHROW(chordsTest.noteQueue().clear());
        REQUIRE(chordsTest.noteQueue().empty());
    }
}
