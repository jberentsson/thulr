#define CATCH_CONFIG_ENABLE_STRINGIFICATION
#include <iostream>
#include <catch2/catch.hpp>
#include "Utils/TestHelpers.hpp"
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

        // // Play a single note.
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

        // // Check the outputs.
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
        std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
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
        std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
    }

    GIVEN("populate couple of keys with overlaping notes where we release the fist key after pressing the second") {
        // Make sure we have the right mode enabled.
        REQUIRE(chordsTest.setNoteMode(Chords::NoteMode::LEGATO));
        
        // We assign notes to the first key.
        REQUIRE(chordsTest.reciveNotes());
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);

        // C Major
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

        // F Major
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteF4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteA4, NOTE_ON) == 0);

        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteF4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.note(NoteA4, NOTE_OFF) == 0);

        REQUIRE(chordsTest.noteQueue().empty());

        // Try to press two at the same time.
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(!chordsTest.noteQueue().empty());
        REQUIRE(chordsTest.noteQueue().size() == 3);

        // Check the note outputs.
        REQUIRE(chordsTest.noteQueue().at(0)->pitch() == NoteC4);
        REQUIRE(chordsTest.noteQueue().at(1)->pitch() == NoteE4);
        REQUIRE(chordsTest.noteQueue().at(2)->pitch() == NoteG4);

        // Check the velocity outputs.
        REQUIRE(chordsTest.noteQueue().at(0)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(1)->velocity() == NOTE_ON);
        REQUIRE(chordsTest.noteQueue().at(2)->velocity() == NOTE_ON);

        REQUIRE_NOTHROW(chordsTest.noteQueue().clear());

        REQUIRE(chordsTest.note(NoteD4, NOTE_ON) == 0);
        REQUIRE(!chordsTest.noteQueue().empty());
        // REQUIRE(chordsTest.noteQueue().size() == 2);

        // REQUIRE(chordsTest.noteQueue().at(0)->pitch() == NoteF4);
        // REQUIRE(chordsTest.noteQueue().at(1)->pitch() == NoteA4);

        // REQUIRE(chordsTest.noteQueue().at(0)->velocity() == NOTE_ON);
        // REQUIRE(chordsTest.noteQueue().at(1)->velocity() == NOTE_ON);

        // REQUIRE_NOTHROW(chordsTest.noteQueue().clear());
        
        // // Release the first note.
        // REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);
        // REQUIRE(!chordsTest.noteQueue().empty());
        // REQUIRE(chordsTest.noteQueue().size() == 3);

        // REQUIRE(chordsTest.noteQueue().at(0)->pitch() == NoteC4);
        // REQUIRE(chordsTest.noteQueue().at(1)->pitch() == NoteE4);
        // REQUIRE(chordsTest.noteQueue().at(2)->pitch() == NoteG4);

        // REQUIRE(chordsTest.noteQueue().at(0)->velocity() == NOTE_OFF);
        // REQUIRE(chordsTest.noteQueue().at(1)->velocity() == NOTE_OFF);
        // REQUIRE(chordsTest.noteQueue().at(2)->velocity() == NOTE_OFF);

        // REQUIRE_NOTHROW(chordsTest.noteQueue().clear());

        // REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        // REQUIRE(!chordsTest.noteQueue().empty());
        // REQUIRE(chordsTest.noteQueue().size() == 2);        
    }
}

SCENARIO("make sure the note modes work") {
    Chords chordsTest = Chords();

    // We assign notes to the first key.
    REQUIRE(chordsTest.reciveNotes());
    REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
    REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);

    // C Major
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

    // F Major
    REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
    REQUIRE(chordsTest.note(NoteF4, NOTE_ON) == 0);
    REQUIRE(chordsTest.note(NoteA4, NOTE_ON) == 0);

    REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);
    REQUIRE(chordsTest.note(NoteF4, NOTE_OFF) == 0);
    REQUIRE(chordsTest.note(NoteA4, NOTE_OFF) == 0);

    REQUIRE(chordsTest.noteQueue().empty());

    GIVEN("legato mode is on"){
        REQUIRE(chordsTest.setNoteMode(Chords::NoteMode::LEGATO) == Chords::NoteMode::LEGATO);
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteD4, NOTE_ON) == 0);
        REQUIRE(!chordsTest.noteQueue().empty());
        REQUIRE(chordsTest.noteQueue().size() == 5);        
    }

    GIVEN("retrigger mod is on"){
        REQUIRE(chordsTest.setNoteMode(Chords::NoteMode::RETRIGGER) == Chords::NoteMode::RETRIGGER);
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.note(NoteD4, NOTE_ON) == 0);
        REQUIRE(!chordsTest.noteQueue().empty());
        REQUIRE(chordsTest.noteQueue().size() == 6);        
    }
}

SCENARIO("make sure the note modes work 2") {
    std::cout << "ææææææææææææææææææææææææææææææææææææææææææææææ\n";
    Chords chordsTest = Chords();

    // We assign notes to the first key.
    REQUIRE(chordsTest.reciveNotes());
    REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
    REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);

    REQUIRE(chordsTest.note(NoteE4, NOTE_ON) == 0);
    REQUIRE(chordsTest.note(NoteG4, NOTE_ON) == 0);
    REQUIRE(chordsTest.note(NoteB4, NOTE_ON) == 0);

    REQUIRE(chordsTest.note(NoteE4, NOTE_OFF) == 0);
    REQUIRE(chordsTest.note(NoteG4, NOTE_OFF) == 0);
    REQUIRE(chordsTest.note(NoteB4, NOTE_OFF) == 0);

    REQUIRE(chordsTest.noteQueue().empty());
    REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
    REQUIRE(!chordsTest.noteQueue().empty());

    std::cout << "ææææææææææææææææææææææææææææææææææææææææææææææ\n";
}

SCENARIO("make sure the basic functions work") {
    std::cout << "ææææææææææææææææææææææææææææææææææææææææææææææ\n";
    Chords chordsTest = Chords();

    WHEN("Add and remove from active.") {
        REQUIRE(chordsTest.addToActive(NoteC4) == 1);
        REQUIRE(chordsTest.removeFromActive(NoteC4) == 0);
    }

    WHEN("We choose an active note") {
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.getActiveKey() == -1);
        REQUIRE(chordsTest.reciveNotes());
        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(chordsTest.getActiveKey() == NoteC4);
    }

    WHEN("Add and remove a chord note.") {
        REQUIRE(chordsTest.getKey(NoteC4)->notes().empty());
        REQUIRE(chordsTest.setActiveKey(NoteC4) == NoteC4);

        REQUIRE(chordsTest.chordNote(NoteF4, NOTE_ON) == 0);
        REQUIRE(!chordsTest.getKey(NoteC4)->notes().empty());
        REQUIRE(chordsTest.getKey(NoteC4)->notes().size() == 1);
    
        REQUIRE(chordsTest.chordNote(NoteA4, NOTE_ON) == 0);
        REQUIRE(!chordsTest.getKey(NoteC4)->notes().empty());
        REQUIRE(chordsTest.getKey(NoteC4)->notes().size() == 2);
    
        REQUIRE(chordsTest.chordNote(NoteC5, NOTE_ON) == 0);
        REQUIRE(!chordsTest.getKey(NoteC4)->notes().empty());
        REQUIRE(chordsTest.getKey(NoteC4)->notes().size() == 3);

        REQUIRE(chordsTest.chordNote(NoteF4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.chordNote(NoteA4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.chordNote(NoteC5, NOTE_OFF) == 0);
        REQUIRE(chordsTest.getKey(NoteC4)->notes().size() == 3);

        REQUIRE(chordsTest.note(NoteC4, NOTE_ON) == 0);
        REQUIRE(!chordsTest.noteQueue().empty());
        REQUIRE(chordsTest.noteQueue().size() == 3);

        REQUIRE(chordsTest.note(NoteC4, NOTE_OFF) == 0);
        REQUIRE(chordsTest.noteQueue().size() == 6);
    }
}
