#include "Keyboard.hpp"
#include "../Utils/NoteValues.hpp"

TEST_CASE("Keyboard basic functionality") {
    Keyboard keyboard;
    
    SECTION("Note on adds a note") {
        int result = keyboard.note(C5, 100);
        REQUIRE(result == 1);
        REQUIRE(keyboard.getActiveNotes().size() == 1);
    }
    
    SECTION("Note off clears notes by pitch class") {
        keyboard.note(C4, 100);
        keyboard.note(C5, 100);
        REQUIRE(keyboard.getActiveNotes().size() == 2);
        
        int cleared = keyboard.clearNotesByPitchClass(60);
        REQUIRE(cleared == 2);
        REQUIRE(keyboard.getActiveNotes().size() == 0);
    }
    
    SECTION("Remove all clears all notes") {
        keyboard.note(C5, 100);
        keyboard.note(E5, 100);
        REQUIRE(keyboard.getActiveNotes().size() == 2);
        
        int cleared = keyboard.removeAll();
        REQUIRE(cleared == 2);
        REQUIRE(keyboard.getActiveNotes().size() == 0);
    }
    
    SECTION("Range setting works") {
        keyboard.setRandomRange(2, 5);
        REQUIRE(keyboard.minCapacity() == 2);
        REQUIRE(keyboard.maxCapacity() == 5);
    }
}
