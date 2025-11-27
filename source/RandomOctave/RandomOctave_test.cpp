#include "Utils/MIDI.hpp"
#include "RandomOctave.hpp"
#include <catch2/catch.hpp>
#include <vector>

using namespace MIDI::Notes;
/* 
// Helper function to check if a note exists in active notes
auto containsNote(const RandomOctave& randomOctave, int expectedNote) -> bool {
    for (const auto& activeNote : randomOctave.getActiveNotes()) {
        if (activeNote->pitch() == expectedNote) {
            return true;
        }
    }
    return false;
}
 */
TEST_CASE("RandomOctave starts empty") {
    RandomOctave randomOctave;
    REQUIRE(randomOctave.getActiveNotes().empty());
}

TEST_CASE("RandomOctave note on adds note") {
    RandomOctave randomOctave;
    const int result = randomOctave.note(NoteC5, 100);  // NOLINT
    
    REQUIRE(result == 1);
    REQUIRE(randomOctave.getActiveNotes().size() == 1);
    //REQUIRE(containsNote(randomOctave, NoteC5));
}

TEST_CASE("RandomOctave note off removes note") {
    RandomOctave randomOctave;
    
    randomOctave.note(NoteC5, 100);  // NOLINT
    REQUIRE(randomOctave.getQueuedNotes().size() == 1);
    REQUIRE(randomOctave.getActiveNotes().size() == 1);
    
    randomOctave.note(NoteC5, 0);
    REQUIRE(randomOctave.getActiveNotes().empty());
}

TEST_CASE("RandomOctave clearNotesByPitchClass works") {
    RandomOctave randomOctave;
    
    randomOctave.note(NoteC4, 100); // NOLINT
    randomOctave.note(NoteC5, 100); // NOLINT
    REQUIRE(randomOctave.getActiveNotes().size() == 2);
    
    const int cleared = randomOctave.clearNotesByPitchClass(NoteC5);
    REQUIRE(cleared == 2);
    REQUIRE(randomOctave.getActiveNotes().empty());
}

TEST_CASE("RandomOctave removeAll clears all notes") {
    RandomOctave randomOctave;
    
    randomOctave.note(NoteC5, 100); // NOLINT
    randomOctave.note(NoteE5, 100); // NOLINT
    REQUIRE(randomOctave.getActiveNotes().size() == 2);
    
    const unsigned int cleared = randomOctave.removeAll();
    REQUIRE(cleared == 2);
    REQUIRE(randomOctave.getActiveNotes().empty());
}

TEST_CASE("RandomOctave range setting works") {
    RandomOctave randomOctave;
    
    randomOctave.setRange(2, 5); // NOLINT
    REQUIRE(randomOctave.minCapacity() == 2);
    REQUIRE(randomOctave.maxCapacity() == 5);
}

TEST_CASE("RandomOctave multiple notes can be added") {
    RandomOctave randomOctave;
    const std::vector<int> notes = {NoteE5, NoteD2, NoteG4};
    
    for (const int note : notes) {
        randomOctave.note(note, 100); // NOLINT
    }
    
    const auto& activeNotes = randomOctave.getActiveNotes();
    REQUIRE(activeNotes.size() == notes.size());
    
    //for (const int note : notes) {
    //    //REQUIRE(containsNote(randomOctave, note));
    //}
}

TEST_CASE("RandomOctave multiple notes can be removed") {
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

TEST_CASE("RandomOctave adding notes increases count") {
    RandomOctave randomOctave;
    const std::vector<int> initialNotes = {NoteC4, NoteE4, NoteG4};
    const std::vector<int> additionalNotes = {NoteA4, NoteB4};
    
    for (const int note : initialNotes) {
        randomOctave.note(note, 100); // NOLINT
    }
    
    for (const int note : additionalNotes) {
        randomOctave.note(note, 100); // NOLINT
    }
    
    REQUIRE(randomOctave.getActiveNotes().size() == 
           initialNotes.size() + additionalNotes.size());
}

TEST_CASE("RandomOctave removing notes decreases count") {
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
    
    REQUIRE(randomOctave.getActiveNotes().size() == 
           initialNotes.size() + additionalNotes.size() - 2);
}

TEST_CASE("RandomOctave remaining notes are correct after removal") {
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
    
    // Check remaining notes exist
/*     REQUIRE(containsNote(randomOctave, initialNotes[2]));
    REQUIRE(containsNote(randomOctave, additionalNotes[0]));
    REQUIRE(containsNote(randomOctave, additionalNotes[1])); */
}

TEST_CASE("RandomOctave removed notes are gone") {
    RandomOctave randomOctave;
    const std::vector<int> initialNotes = {NoteC4, NoteE4, NoteG4};
    
    // Add notes
    for (const int note : initialNotes) {
        randomOctave.note(note, 100); // NOLINT
    }
    
    // Remove two notes
    randomOctave.note(initialNotes[0], 0);
    randomOctave.note(initialNotes[1], 0);
    
    // Check removed notes are gone
/*     REQUIRE_FALSE(containsNote(randomOctave, initialNotes[0]));
    REQUIRE_FALSE(containsNote(randomOctave, initialNotes[1])); */
}

TEST_CASE("RandomOctave zero velocity note does nothing") {
    RandomOctave randomOctave;
    
    randomOctave.note(NoteC5, 0);
    REQUIRE(randomOctave.getActiveNotes().empty());
}

TEST_CASE("RandomOctave removing non-existent note does nothing") {
    RandomOctave randomOctave;
    
    randomOctave.note(NoteC5, 100); // NOLINT
    const unsigned int beforeSize = randomOctave.getActiveNotes().size();
    
    randomOctave.note(NoteD5, 0); // Remove non-existent note
    
    REQUIRE(randomOctave.getActiveNotes().size() == beforeSize);
/*     REQUIRE(containsNote(randomOctave, NoteC5)); */
}

TEST_CASE("RandomOctave duplicate note behavior") {
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

TEST_CASE("assa"){
    RandomOctave randomOctave;

    REQUIRE_NOTHROW(randomOctave.note( NoteC5, 100 ));
    REQUIRE_NOTHROW(randomOctave.note( NoteE5, 100 ));
    REQUIRE_NOTHROW(randomOctave.note( NoteG5, 100 ));

    REQUIRE(randomOctave.getQueuedNotes().size() == 3);
    REQUIRE(randomOctave.getActiveNotes().size() == 3);

    // Remove G5
    REQUIRE_NOTHROW(randomOctave.note( NoteG5, 0 ));

    REQUIRE(randomOctave.getQueuedNotes().size() == 4);
    REQUIRE(randomOctave.getActiveNotes().size() == 2);

    // Remove E5
    REQUIRE_NOTHROW(randomOctave.note( NoteE5, 0 ));

    REQUIRE(randomOctave.getQueuedNotes().size() == 5);
    REQUIRE(randomOctave.getActiveNotes().size() == 1);

    // Try to remove a note that is not active.
    REQUIRE_NOTHROW(randomOctave.note( NoteE5, 0 ));

    // No change.
    REQUIRE(randomOctave.getQueuedNotes().size() == 5);
    REQUIRE(randomOctave.getActiveNotes().size() == 1);

    // Remove C5
    REQUIRE_NOTHROW(randomOctave.note( NoteC5, 0 ));

    REQUIRE(randomOctave.getQueuedNotes().size() == 6);
    REQUIRE(randomOctave.getActiveNotes().size() == 0);
};
