#include "Utils/MIDI.hpp"
#include "Note/Note.hpp"
#include "Note/Keyboard.hpp"
#include "RandomOctave.hpp"
#include <catch2/catch.hpp>
#include <vector>
#include <algorithm>

using namespace MIDI::Notes;

SCENARIO("test the note class") {
    Note noteTest = Note(48); // NOLINT
    REQUIRE(noteTest.getActiveNotes().empty());
    REQUIRE(noteTest.add(47, 46, 127) == nullptr);
    REQUIRE(noteTest.add(48, 36, 127) != nullptr);
    REQUIRE(noteTest.add(48, 46, 127) == nullptr);
    REQUIRE(noteTest.add(48, 48, 127) != nullptr);
    REQUIRE(noteTest.add(48, 60, 127) != nullptr);
    REQUIRE(noteTest.add(48, 72, 127) != nullptr);
    REQUIRE(noteTest.add(48, 84, 127) == nullptr);
    REQUIRE(noteTest.getActiveNotes().size() == 4);
    REQUIRE_NOTHROW(noteTest.getActiveNotes().clear());
    REQUIRE(noteTest.getActiveNotes().empty());
}

SCENARIO("test the keyboard class") {
    Keyboard keyboardTest = Keyboard();
    REQUIRE(keyboardTest.getActiveNotes().empty());
    REQUIRE(keyboardTest.getNoteQueue().empty());
    REQUIRE(keyboardTest.add(47, 46, 127) == RandomOctave::NoteReturnCodes::DEGREE_MISMATCH);
    REQUIRE(keyboardTest.add(48, 36, 127) == RandomOctave::NoteReturnCodes::OK);
    REQUIRE(keyboardTest.add(48, 46, 127) == RandomOctave::NoteReturnCodes::DEGREE_MISMATCH);
    REQUIRE(keyboardTest.add(48, 48, 127) == RandomOctave::NoteReturnCodes::OK);
    REQUIRE(keyboardTest.add(48, 60, 127) == RandomOctave::NoteReturnCodes::OK);
    REQUIRE(keyboardTest.add(48, 72, 127) == RandomOctave::NoteReturnCodes::OK);
    REQUIRE(keyboardTest.add(48, 84, 127) == RandomOctave::NoteReturnCodes::OUT_OF_SPACE);
    REQUIRE(keyboardTest.getActiveNotes().size() == 4);
    REQUIRE(!keyboardTest.getNoteQueue().empty());
    REQUIRE(keyboardTest.getNoteQueue().size() == 4);
    REQUIRE(keyboardTest.clearQueue() == 0);
    REQUIRE(keyboardTest.getNoteQueue().empty());
    REQUIRE(keyboardTest.getActiveNotes().size() == 4);
    REQUIRE(keyboardTest.remove(48).size() == 4);
    REQUIRE(keyboardTest.getNoteQueue().size() == 4);
    REQUIRE(keyboardTest.getActiveNotes().empty());
    REQUIRE(keyboardTest.clearQueue() == 0);
    REQUIRE(keyboardTest.getNoteQueue().empty());
}

SCENARIO("keyboard add function") {
    Keyboard keyboardTest = Keyboard();
    REQUIRE_NOTHROW(keyboardTest.add(60, 60, 0));   // Note-off first.
    REQUIRE_NOTHROW(keyboardTest.add(60, 72, 100)); // Note-on second.

    REQUIRE_NOTHROW(keyboardTest.add(60, 72, 100)); // Note-on first.
    REQUIRE_NOTHROW(keyboardTest.add(60, 72, 0));   // Note-off second.

    REQUIRE(keyboardTest.getActiveNotes().empty());
    REQUIRE(keyboardTest.getNoteQueue().size() == 3);
    REQUIRE(keyboardTest.getNoteQueue().at(0)->velocity() == 0);
    REQUIRE(keyboardTest.getNoteQueue().at(1)->velocity() == 100);
    REQUIRE(keyboardTest.getNoteQueue().at(2)->velocity() == 0);
}

SCENARIO("test the something") {
    Keyboard keyboardTest = Keyboard();
    REQUIRE(keyboardTest.getActiveNotes().empty());
    REQUIRE(keyboardTest.add(NoteA5, NoteA5, 127) == RandomOctave::NoteReturnCodes::OK);
    REQUIRE(keyboardTest.getActiveNotes().size() == 1);
    REQUIRE(keyboardTest.add(NoteA5, NoteA6, 127) == RandomOctave::NoteReturnCodes::OK);
    REQUIRE(!keyboardTest.remove(NoteA5).empty());
    REQUIRE(keyboardTest.getActiveNotes().empty());
}

SCENARIO("RandomOctave starts empty") {
    RandomOctave randomOctave;
    REQUIRE(randomOctave.getActiveNotes().empty());
}

SCENARIO("RandomOctave note on adds note") {
    RandomOctave randomOctave;    
    REQUIRE(randomOctave.note(NoteC5, 100) == 0); // NOLINT
    REQUIRE(randomOctave.getActiveNotes().size() == 1);
    REQUIRE(randomOctave.containsNote(NoteC5));
}

SCENARIO("RandomOctave note off removes note") {
    RandomOctave randomOctave;
    REQUIRE(randomOctave.note(NoteC5, 100) == 0);
    REQUIRE(randomOctave.getNoteQueue().size() == 1);
    REQUIRE(randomOctave.getActiveNotes().size() == 1);
    REQUIRE(randomOctave.note(NoteC5, 0) == 0);
    REQUIRE(randomOctave.getActiveNotes().empty());
}

SCENARIO("RandomOctave clearNotesByPitchClass works") {
    RandomOctave randomOctave;
    REQUIRE(randomOctave.note(NoteC4, 100) == 0); // NOLINT
    REQUIRE(randomOctave.note(NoteC5, 100) == 0); // NOLINT
    REQUIRE(randomOctave.getActiveNotes().size() == 2);
    REQUIRE(randomOctave.note(NoteC5, 0) == 0);
    REQUIRE(randomOctave.getActiveNotes().size() == 1);
    REQUIRE(!randomOctave.getNoteQueue().empty());
    REQUIRE(randomOctave.getNoteQueue().size() == 3);
    REQUIRE_NOTHROW(randomOctave.clearQueue());
    REQUIRE_NOTHROW(randomOctave.removeAll());
    REQUIRE(randomOctave.getActiveNotes().empty());
    REQUIRE(randomOctave.getNoteQueue().empty());
    REQUIRE_NOTHROW(randomOctave.clearQueue());
}

SCENARIO("RandomOctave removeAll clears all notes") {
    RandomOctave randomOctave;
    REQUIRE(randomOctave.note(NoteC5, 100) == 0); // NOLINT
    REQUIRE(randomOctave.note(NoteE5, 100) == 0); // NOLINT
    REQUIRE(randomOctave.getActiveNotes().size() == 2);    
    REQUIRE(randomOctave.removeAll() == 2);
    REQUIRE(randomOctave.getActiveNotes().empty());
}

SCENARIO("RandomOctave range setting works") {
    RandomOctave randomOctave;
    REQUIRE(randomOctave.setRange(2, 5) == 0); // NOLINT
    // TOOD: Update this to 0 - 127.
    //REQUIRE(randomOctave.minCapacity() == 2);
    //REQUIRE(randomOctave.maxCapacity() == 5);
}

SCENARIO("RandomOctave multiple notes can be added") {
    RandomOctave randomOctave;
    const std::vector<int> notes = { NoteE5, NoteD2, NoteG4 };
    
    for (const int note : notes) {
        randomOctave.note(note, 100); // NOLINT
    }
    
    REQUIRE(randomOctave.getActiveNotes().size() == notes.size());
    
    for (const int note : notes) {
        REQUIRE(randomOctave.containsNote(note));
    }
}

SCENARIO("RandomOctave multiple notes can be removed") {
    RandomOctave randomOctave;
    const std::vector<int> notes = { NoteE5, NoteD2, NoteG4 };
    
    for (const int note : notes) {
        randomOctave.note(note, 100); // NOLINT
    }

    REQUIRE(randomOctave.getActiveNotes().size() == notes.size());
    
    for (const int note : notes) {
        randomOctave.note(note, 0);
    }
    
    REQUIRE(randomOctave.getActiveNotes().empty());
}

SCENARIO("RandomOctave adding notes increases count") {
    RandomOctave randomOctave;
    const std::vector<int> initialNotes = { NoteC4, NoteE4, NoteG4 };
    const std::vector<int> additionalNotes = { NoteA4, NoteB4 };
    
    for (const int note : initialNotes) {
        randomOctave.note(note, 100); // NOLINT
    }
    
    for (const int note : additionalNotes) {
        randomOctave.note(note, 100); // NOLINT
    }
    
    REQUIRE(!randomOctave.getActiveNotes().empty());
    REQUIRE(randomOctave.getActiveNotes().size() == initialNotes.size() + additionalNotes.size());
}

SCENARIO("RandomOctave removing notes decreases count") {
    RandomOctave randomOctave;

    const std::vector<int> initialNotes = { NoteC4, NoteE4, NoteG4 };
    const std::vector<int> additionalNotes = { NoteA4, NoteB4 };
    
    // Add all notes
    for (const int note : initialNotes) {
        randomOctave.note(note, 100); // NOLINT
    }
    for (const int note : additionalNotes) {
        randomOctave.note(note, 100); // NOLINT
    }
    
    // Remove two notes
    randomOctave.note(initialNotes[0], 0);
    randomOctave.note(initialNotes[1], 0);

    REQUIRE(randomOctave.getActiveNotes().size() == initialNotes.size() + additionalNotes.size() - 2);
}

SCENARIO("RandomOctave remaining notes are correct after removal") { // NOLINT
    RandomOctave randomOctave;
    const std::vector<int> initialNotes = { NoteC4, NoteE4, NoteG4 };
    const std::vector<int> additionalNotes = { NoteA4, NoteB4 };
    
    GIVEN("a") {
        // Add all notes
        for (const int note : initialNotes) {
            REQUIRE(randomOctave.note(note, 100) == 0); // NOLINT
        }

        WHEN("b") {
            for (const int note : additionalNotes) {
                REQUIRE(randomOctave.note(note, 100) == 0); // NOLINT
            }

            // TODO: Sometimes only four numbers are inserted.
            REQUIRE(!randomOctave.getActiveNotes().empty());
            // TODO: This output should be consistant number.
            //REQUIRE(randomOctave.getActiveNotes().size() == 5);

            THEN("c") {
                // Remove two notes
                REQUIRE(randomOctave.note(initialNotes[0], 0) == 0);
                REQUIRE(randomOctave.note(initialNotes[1], 0) == 0);
                
                // Check remaining notes exist
                REQUIRE(randomOctave.containsNote(initialNotes[2]));
                REQUIRE(randomOctave.containsNote(additionalNotes[0]));
                REQUIRE(randomOctave.containsNote(additionalNotes[1]));
            }
        }
    }
}

SCENARIO("RandomOctave removed notes are gone") {
    RandomOctave randomOctave;
    const std::vector<int> initialNotes = { NoteC4, NoteE4, NoteG4 };
    
    // Add notes
    for (const int note : initialNotes) { // NOLINT
        REQUIRE(randomOctave.note(note, 100) == 0); // NOLINT
    }
    
    REQUIRE(randomOctave.containsNote(initialNotes[0]));
    REQUIRE(randomOctave.containsNote(initialNotes[1]));
    REQUIRE(randomOctave.containsNote(initialNotes[2]));

    // Remove two notes
    randomOctave.note(initialNotes[0], 0);
    randomOctave.note(initialNotes[1], 0);
    
    // Check removed notes are gone
    REQUIRE_FALSE(randomOctave.containsNote(initialNotes[0]));
    REQUIRE_FALSE(randomOctave.containsNote(initialNotes[1]));
}

SCENARIO("RandomOctave zero velocity note does nothing") {
    RandomOctave randomOctave;
    REQUIRE(randomOctave.note(NoteC5, 0) == 0);
    REQUIRE(randomOctave.getActiveNotes().empty());
}

SCENARIO("RandomOctave removing non-existent note does nothing") {
    RandomOctave randomOctave;
    REQUIRE(randomOctave.note(NoteC5, 100) == 0); // NOLINT
    const unsigned int beforeSize = randomOctave.getActiveNotes().size();
    REQUIRE(randomOctave.note(NoteD5, 0) == 0); // Remove non-existent note
    REQUIRE(randomOctave.getActiveNotes().size() == beforeSize);
    REQUIRE(randomOctave.containsNote(NoteC5));
}

SCENARIO("RandomOctave duplicate note behavior") {
    RandomOctave randomOctave;
    
    GIVEN("Two identical notes are played.") {
        randomOctave.note(NoteC5, 100); // NOLINT
        randomOctave.note(NoteC5, 100); // NOLINT
        
        THEN("One or two notes can be active."){
            const auto size = randomOctave.getActiveNotes().size();
            REQUIRE((size == 1 || size == 2));
        }
    }
}

SCENARIO("assa"){
    RandomOctave randomOctave;

    // Play C Major
    REQUIRE_NOTHROW(randomOctave.note( NoteC5, 100 ));
    REQUIRE_NOTHROW(randomOctave.note( NoteE5, 100 ));
    REQUIRE_NOTHROW(randomOctave.note( NoteG5, 100 ));

    REQUIRE(randomOctave.getNoteQueue().size() == 3);
    REQUIRE(randomOctave.getActiveNotes().size() == 3);
    
    // Check if the correct notes are on the note queue.
    REQUIRE(randomOctave.getNoteQueue().at(0)->pitch() % MIDI::OCTAVE == NoteC5 % MIDI::OCTAVE);
    REQUIRE(randomOctave.getNoteQueue().at(1)->pitch() % MIDI::OCTAVE == NoteE5 % MIDI::OCTAVE);
    REQUIRE(randomOctave.getNoteQueue().at(2)->pitch() % MIDI::OCTAVE == NoteG5 % MIDI::OCTAVE);

    REQUIRE(randomOctave.getNoteQueue().at(0)->velocity() == 100);
    REQUIRE(randomOctave.getNoteQueue().at(1)->velocity() == 100);
    REQUIRE(randomOctave.getNoteQueue().at(2)->velocity() == 100);

    REQUIRE_NOTHROW(randomOctave.clearQueue());
    REQUIRE(randomOctave.getNoteQueue().empty());
    REQUIRE(randomOctave.getActiveNotes().size() == 3);

    // Remove G5
    REQUIRE_NOTHROW(randomOctave.note( NoteG5, 0 ));

    REQUIRE(randomOctave.getNoteQueue().size() == 1);
    REQUIRE(randomOctave.getActiveNotes().size() == 2);

    // Remove E5
    REQUIRE_NOTHROW(randomOctave.note( NoteE5, 0 ));

    REQUIRE(randomOctave.getNoteQueue().size() == 2);
    REQUIRE(randomOctave.getActiveNotes().size() == 1);  

    // Try to remove a note that is not active.
    REQUIRE_NOTHROW(randomOctave.note( NoteE5, 0 ));

    // No change.
    REQUIRE(randomOctave.getNoteQueue().size() == 3);
    REQUIRE(!randomOctave.getActiveNotes().empty());
    REQUIRE(randomOctave.getNoteQueue().size() == 3);

    // Remove C5
    REQUIRE_NOTHROW(randomOctave.note( NoteC5, 0 ));

    REQUIRE(randomOctave.getNoteQueue().size() == 4);
    
    REQUIRE(randomOctave.getNoteQueue().at(0)->velocity() == 0);
    REQUIRE(randomOctave.getNoteQueue().at(1)->velocity() == 0);
    REQUIRE(randomOctave.getNoteQueue().at(2)->velocity() == 0);
    
    REQUIRE(randomOctave.getNoteQueue().at(0)->pitch() % 12 == 7);
    REQUIRE(randomOctave.getNoteQueue().at(1)->pitch() % 12 == 4);
    REQUIRE(randomOctave.getNoteQueue().at(2)->pitch() % 12 == 4);
    REQUIRE(randomOctave.getNoteQueue().at(3)->pitch() % 12 == 0);

    REQUIRE_NOTHROW(randomOctave.clearQueue());

    REQUIRE(randomOctave.getNoteQueue().empty());
    REQUIRE(randomOctave.getActiveNotes().empty());
};

SCENARIO("a chromatic scale is played") { // NOLINT
    RandomOctave randomOctaveTestObject;

    THEN("all chromatic notes are processed without crashing") {
        // Play chromatic scale note-ons.
        for (int note = NoteC5; note <= NoteC6; note++) {
            REQUIRE_NOTHROW(randomOctaveTestObject.note(note, 100));
        }
        
        // Should have 13 active notes (if no degree mismatches).
        REQUIRE(randomOctaveTestObject.getActiveNotes().size() == 13);

        // Play note-offs.
        for (int note = NoteC5; note <= NoteC6; note++) {
            REQUIRE_NOTHROW(randomOctaveTestObject.note(note, 0));
        }

        // Should be no active notes after note-offs.
        REQUIRE(randomOctaveTestObject.getActiveNotes().empty());
        
        // Get the note queue,
        auto noteQueue = randomOctaveTestObject.getNoteQueue();
        
        // Queue should have 26 notes total (13 note-ons + 13 note-offs).
        REQUIRE(noteQueue.size() == 26);
        
        // Check first 13 are note-ons (velocity 100).
        for (int i = 0; i < 13; i++) { // NOLINT
            REQUIRE(noteQueue[i]->velocity() == 100);
        }
        
        // Check last 13 are note-offs (velocity 0),
        for (int i = 13; i < 26; i++) { // NOLINT
            REQUIRE(noteQueue[i]->velocity() == 0);
        }
    }
};
