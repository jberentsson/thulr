#include <catch2/catch.hpp>
#include "Keyboard.hpp"
#include "../Utils/MIDI.hpp"

using namespace MIDI::Notes;

TEST_CASE("Keyboard basic functionality") {
    Keyboard keyboard;
    
    SECTION("Note on adds a note") {
        int result = keyboard.note(NoteC5, 100); // NOLINT
        REQUIRE(result == 1);
        REQUIRE(keyboard.getActiveNotes().size() == 1);
    }
    
    SECTION("Note off clears notes by pitch class") {
        keyboard.note(NoteC4, 100); // NOLINT
        keyboard.note(NoteC5, 100); // NOLINT
        REQUIRE(keyboard.getActiveNotes().size() == 2);
        
        int cleared = keyboard.clearNotesByPitchClass(60);
        REQUIRE(cleared == 2);
        REQUIRE(keyboard.getActiveNotes().size() == 0);
    }
    
    SECTION("Remove all clears all notes") {
        keyboard.note(NoteC5, 100); // NOLINT
        keyboard.note(NoteE5, 100); // NOLINT
        REQUIRE(keyboard.getActiveNotes().size() == 2);
        
        int cleared = keyboard.removeAll();
        REQUIRE(cleared == 2);
        REQUIRE(keyboard.getActiveNotes().empty());
    }
    
    SECTION("Range setting works") {
        keyboard.setRandomRange(2, 5); // NOLINT
        REQUIRE(keyboard.minCapacity() == 2);
        REQUIRE(keyboard.maxCapacity() == 5);
    }
}
