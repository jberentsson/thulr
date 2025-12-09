#include <catch2/catch.hpp>
#include "Chords.hpp"

using namespace MIDI::Notes;

constexpr int NOTE_ON = 127;
constexpr int NOTE_OFF = 0;

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
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);

        // Play a C Major chord.
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteE4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteG4, NOTE_ON) == 0);

        // Release the notes.
        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteG4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteE4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.noteQueue().empty());
        REQUIRE(!chordsTest.isRecievingNotes());

        // Play a single note.
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);

        // And a C Major chord is played.
        REQUIRE(!chordsTest.noteQueue().empty());
        REQUIRE(chordsTest.noteQueue().size() == 3);

        REQUIRE(chordsTest.noteQueue().at(0)->pitch() == NoteC4);
        REQUIRE(chordsTest.noteQueue().at(1)->pitch() == NoteE4);
        REQUIRE(chordsTest.noteQueue().at(2)->pitch() == NoteG4);

        REQUIRE(chordsTest.noteQueue().at(0)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(1)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(2)->velocity() == NOTE_ON);

        REQUIRE_NOTHROW(chordsTest.noteQueue().clear());
        REQUIRE(chordsTest.noteQueue().empty());

        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);

        REQUIRE(!chordsTest.noteQueue().empty());
        REQUIRE(chordsTest.noteQueue().size() == 3);

        REQUIRE(chordsTest.noteQueue().at(0)->pitch() == NoteC4);
        REQUIRE(chordsTest.noteQueue().at(1)->pitch() == NoteE4);
        REQUIRE(chordsTest.noteQueue().at(2)->pitch() == NoteG4);

        REQUIRE(chordsTest.noteQueue().at(0)->velocity() == NOTE_OFF);
        REQUIRE(chordsTest.noteQueue().at(1)->velocity() == NOTE_OFF);
        REQUIRE(chordsTest.noteQueue().at(2)->velocity() == NOTE_OFF);
    }
}

SCENARIO("make sure we can trigger more than one key at a time") {
    Chords chordsTest = Chords();
    REQUIRE(true);
    REQUIRE(chordsTest.noteQueue().empty());

    GIVEN("poulate couple of keys") {
        // We assign notes to the first key.
        REQUIRE(chordsTest.reciveNotes());
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteE4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteG4, NOTE_ON) == 0);

        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteE4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteG4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.noteQueue().empty());

        // We assign notes to the second key.
        REQUIRE(chordsTest.reciveNotes());
        REQUIRE(chordsTest.note(NoteD4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteD4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.note(NoteC5, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteE5, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteG5, NOTE_ON) == 0);

        REQUIRE(chordsTest.note(NoteC5, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteE5, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteG5, NOTE_OFF) == 0);

        REQUIRE(chordsTest.noteQueue().empty());

        // Try to press two at the same time.
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteD4, NOTE_ON) == 0);

        REQUIRE(!chordsTest.noteQueue().empty());
        REQUIRE(chordsTest.noteQueue().size() == 6);

        // Check the outputs.
        REQUIRE(chordsTest.noteQueue().at(0)->pitch() == NoteC4);
        REQUIRE(chordsTest.noteQueue().at(1)->pitch() == NoteE4);
        REQUIRE(chordsTest.noteQueue().at(2)->pitch() == NoteG4);
        REQUIRE(chordsTest.noteQueue().at(3)->pitch() == NoteC5);
        REQUIRE(chordsTest.noteQueue().at(4)->pitch() == NoteE5);
        REQUIRE(chordsTest.noteQueue().at(5)->pitch() == NoteG5);

        REQUIRE(chordsTest.noteQueue().at(0)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(1)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(2)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(3)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(4)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(5)->velocity() == NOTE_ON);
    }

    GIVEN("populate couple of keys with overlaping notes") {
        // We assign notes to the first key.
        REQUIRE(chordsTest.reciveNotes());
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteE4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteG4, NOTE_ON) == 0);

        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteE4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteG4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.noteQueue().empty());

        // We assign notes to the second key.
        REQUIRE(chordsTest.reciveNotes());
        REQUIRE(chordsTest.note(NoteD4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteD4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.note(NoteC5, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteF5, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteA5, NOTE_ON) == 0);

        REQUIRE(chordsTest.note(NoteC5, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteF5, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteA5, NOTE_OFF) == 0);

        REQUIRE(chordsTest.noteQueue().empty());

        // Try to press two at the same time.
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteD4, NOTE_ON) == 0);

        REQUIRE(!chordsTest.noteQueue().empty());
        REQUIRE(chordsTest.noteQueue().size() == 6);

        // Check the outputs.
        REQUIRE(chordsTest.noteQueue().at(0)->pitch() == NoteC4);
        REQUIRE(chordsTest.noteQueue().at(1)->pitch() == NoteE4);
        REQUIRE(chordsTest.noteQueue().at(2)->pitch() == NoteG4);
        REQUIRE(chordsTest.noteQueue().at(3)->pitch() == NoteC5);
        REQUIRE(chordsTest.noteQueue().at(4)->pitch() == NoteF5);
        REQUIRE(chordsTest.noteQueue().at(5)->pitch() == NoteA5);

        REQUIRE(chordsTest.noteQueue().at(0)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(1)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(2)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(3)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(4)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(5)->velocity() == NOTE_ON);
    }

    GIVEN("populate couple of keys with overlaping notes where we release the fist key after pressing the second") {
        // We assign notes to the first key.
        REQUIRE(chordsTest.reciveNotes());
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteE4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteG4, NOTE_ON) == 0);

        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteE4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteG4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.noteQueue().empty());

        // We assign notes to the second key.
        REQUIRE(chordsTest.reciveNotes());
        REQUIRE(chordsTest.note(NoteD4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteD4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteF4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteA4, NOTE_ON) == 0);

        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteF4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteA4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.noteQueue().empty());

        // Try to press two at the same time.
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteD4, NOTE_ON) == 0);

        // Release the first note.
        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);

        REQUIRE(!chordsTest.noteQueue().empty());
        REQUIRE(chordsTest.noteQueue().size() == 8);

        // Check the note outputs.
        REQUIRE(chordsTest.noteQueue().at(0)->pitch() == NoteC4);
        REQUIRE(chordsTest.noteQueue().at(1)->pitch() == NoteE4);
        REQUIRE(chordsTest.noteQueue().at(2)->pitch() == NoteG4);

        REQUIRE(chordsTest.noteQueue().at(3)->pitch() == NoteC4);
        REQUIRE(chordsTest.noteQueue().at(4)->pitch() == NoteF4);
        REQUIRE(chordsTest.noteQueue().at(5)->pitch() == NoteA4);

        REQUIRE(chordsTest.noteQueue().at(6)->pitch() == NoteE4);
        REQUIRE(chordsTest.noteQueue().at(7)->pitch() == NoteG4);

        // Check the velocity outputs.
        REQUIRE(chordsTest.noteQueue().at(0)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(1)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(2)->velocity() == NOTE_ON);

        REQUIRE(chordsTest.noteQueue().at(3)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(4)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(5)->velocity() == NOTE_ON);

        REQUIRE(chordsTest.noteQueue().at(6)->velocity() == NOTE_OFF);
        REQUIRE(chordsTest.noteQueue().at(7)->velocity() == NOTE_OFF);
    }
}
