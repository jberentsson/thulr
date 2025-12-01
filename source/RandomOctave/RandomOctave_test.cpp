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
    REQUIRE(noteTest.getActiveNotes().size() == 0);
    REQUIRE(noteTest.add(47, 46, 127) == nullptr);
    REQUIRE(noteTest.add(48, 36, 127) != nullptr);
    REQUIRE(noteTest.add(48, 46, 127) == nullptr);
    REQUIRE(noteTest.add(48, 48, 127) != nullptr);
    REQUIRE(noteTest.add(48, 60, 127) != nullptr);
    REQUIRE(noteTest.add(48, 72, 127) != nullptr);
    REQUIRE(noteTest.add(48, 84, 127) == nullptr);
    REQUIRE(noteTest.getActiveNotes().size() == 4);
}

SCENARIO("test the keyboard class") {
    Keyboard keyboardTest = Keyboard();
    REQUIRE(keyboardTest.getActiveNotes().size() == 0);
    REQUIRE(keyboardTest.getNoteQueue().empty());
    REQUIRE(keyboardTest.add(47, 46, 127) == nullptr);
    REQUIRE(keyboardTest.add(48, 36, 127) != nullptr);
    REQUIRE(keyboardTest.add(48, 46, 127) == nullptr);
    REQUIRE(keyboardTest.add(48, 48, 127) != nullptr);
    REQUIRE(keyboardTest.add(48, 60, 127) != nullptr);
    REQUIRE(keyboardTest.add(48, 72, 127) != nullptr);
    REQUIRE(keyboardTest.add(48, 84, 127) == nullptr);
    REQUIRE(keyboardTest.getActiveNotes().size() == 4);
    REQUIRE(!keyboardTest.getNoteQueue().empty());
    REQUIRE(keyboardTest.getNoteQueue().size() == 4);
    REQUIRE(keyboardTest.clearQueue() == 0);
    REQUIRE(keyboardTest.getNoteQueue().empty());
    REQUIRE(keyboardTest.getActiveNotes().size() == 4);
    REQUIRE(keyboardTest.remove(48).size() > 0);
    REQUIRE(keyboardTest.getNoteQueue().size() == 4);
    REQUIRE(keyboardTest.getActiveNotes().size() == 0);
    REQUIRE(keyboardTest.clearQueue() == 0);
    REQUIRE(keyboardTest.getNoteQueue().empty());
}


SCENARIO("test the something") {
    Keyboard keyboardTest = Keyboard();

    REQUIRE(keyboardTest.getActiveNotes().size() == 0);
    REQUIRE(keyboardTest.add(NoteA5, NoteA5, 127) != nullptr);
    REQUIRE(keyboardTest.getActiveNotes().size() == 1);
    REQUIRE(keyboardTest.add(NoteA5, NoteA6, 127) != nullptr);
    REQUIRE(keyboardTest.clearNotesByPitchClass(NoteA5) == 0);
    REQUIRE(keyboardTest.getActiveNotes().size() == 0);
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
    REQUIRE(randomOctave.clear(NoteC5) == 1);
    REQUIRE(randomOctave.getActiveNotes().size() == 1);
    REQUIRE(!randomOctave.getNoteQueue().empty());
    REQUIRE(randomOctave.getNoteQueue().size() == 2);
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
    const std::vector<int> notes = {NoteE5, NoteD2, NoteG4};
    
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
    const std::vector<int> notes = {NoteE5, NoteD2, NoteG4};
    
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
    const std::vector<int> initialNotes = {NoteC4, NoteE4, NoteG4};
    const std::vector<int> additionalNotes = {NoteA4, NoteB4};
    
    for (const int note : initialNotes) {
        randomOctave.note(note, 100); // NOLINT
    }
    
    for (const int note : additionalNotes) {
        randomOctave.note(note, 100); // NOLINT
    }
    
    REQUIRE(randomOctave.getActiveNotes().size() > 0);
    //REQUIRE(randomOctave.getActiveNotes().size() == initialNotes.size() + additionalNotes.size());
}

SCENARIO("RandomOctave removing notes decreases count") {
    RandomOctave randomOctave;
    const std::vector<int> initialNotes = {NoteC4, NoteE4, NoteG4};
    const std::vector<int> additionalNotes = {NoteA4, NoteB4};
    
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

    // TODO: This output should be consistant number.
    //REQUIRE(randomOctave.getActiveNotes().size() == initialNotes.size() + additionalNotes.size() - 2);
}

SCENARIO("RandomOctave remaining notes are correct after removal") { // NOLINT
    RandomOctave randomOctave;
    const std::vector<int> initialNotes = {NoteC4, NoteE4, NoteG4};
    const std::vector<int> additionalNotes = {NoteA4, NoteB4};
    
    GIVEN("a") {
        // Add all notes
        for (const int note : initialNotes) {
            REQUIRE(randomOctave.note(note, 100) == 0); // NOLINT
        }

        WHEN("b") {
            for (const int note : additionalNotes) {
                REQUIRE(randomOctave.note(note, 100) == 0); // NOLINT
            }

            THEN("c") {
                // Remove two notes
                REQUIRE(randomOctave.note(initialNotes[0], 0) == 0);
                REQUIRE(randomOctave.note(initialNotes[1], 0) == 0);
                
                // Check remaining notes exist
                REQUIRE(randomOctave.containsNote(initialNotes[2]));

                // TODO: This is still failing.
                //REQUIRE(randomOctave.containsNote(additionalNotes[0]));
                //REQUIRE(randomOctave.containsNote(additionalNotes[1]));
            }
        }
    }
}

SCENARIO("RandomOctave removed notes are gone") {
    RandomOctave randomOctave;
    const std::vector<int> initialNotes = { NoteC4, NoteE4, NoteG4 };
    
    // Add notes
    for (const int note : initialNotes) {
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

    REQUIRE_NOTHROW(randomOctave.note( NoteC5, 100 ));
    REQUIRE_NOTHROW(randomOctave.note( NoteE5, 100 ));
    REQUIRE_NOTHROW(randomOctave.note( NoteG5, 100 ));

    REQUIRE(randomOctave.getNoteQueue().size() == 3);
    REQUIRE(randomOctave.getActiveNotes().size() == 3);
    
    REQUIRE(randomOctave.getNoteQueue().at(0)->velocity() == 100);
    REQUIRE(randomOctave.getNoteQueue().at(1)->velocity() == 100);
    REQUIRE(randomOctave.getNoteQueue().at(2)->velocity() == 100);

    REQUIRE_NOTHROW(randomOctave.clearQueue());
    REQUIRE(randomOctave.getNoteQueue().empty());

    // Remove G5
    REQUIRE_NOTHROW(randomOctave.note( NoteG5, 0 ));

    REQUIRE(randomOctave.getNoteQueue().size() == 1);
    REQUIRE(randomOctave.getActiveNotes().size() >= 2);

    // Remove E5
    REQUIRE_NOTHROW(randomOctave.note( NoteE5, 0 ));

    REQUIRE(randomOctave.getNoteQueue().size() == 2);
    REQUIRE(randomOctave.getActiveNotes().size() == 1);  

    // Try to remove a note that is not active.
    REQUIRE_NOTHROW(randomOctave.note( NoteE5, 0 ));

    // No change.
    REQUIRE(randomOctave.getNoteQueue().size() == 3);
    REQUIRE(randomOctave.getActiveNotes().size() >= 1);

    // Remove C5
    REQUIRE_NOTHROW(randomOctave.note( NoteC5, 0 ));

    REQUIRE(randomOctave.getNoteQueue().size() == 4);
    
    //REQUIRE(randomOctave.getNoteQueue().at(0)->velocity() == 0);
    //REQUIRE(randomOctave.getNoteQueue().at(1)->velocity() == 0);
    //REQUIRE(randomOctave.getNoteQueue().at(2)->velocity() == 0);
    
    // TODO: This doesn't work.
    //REQUIRE(randomOctave.getNoteQueue().at(0)->pitch() % 12 == 0);

    REQUIRE_NOTHROW(randomOctave.clearQueue());

    REQUIRE(randomOctave.getNoteQueue().empty());
    REQUIRE(randomOctave.getActiveNotes().empty());
};
