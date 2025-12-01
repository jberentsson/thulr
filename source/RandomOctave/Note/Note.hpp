#pragma once

#include <memory>
#include <algorithm>
#include <vector>
#include "ActiveNote.hpp"
#include "Utils/MIDI.hpp"

// TODO: Rename this to key?
class Note {
private:
    enum : uint8_t {
        MAX_NOTES = 4
    };

    int mPitch_ = 0;
    int rangeHigh_ = MIDI::RANGE_HIGH;
    int rangeLow_ = MIDI::RANGE_LOW;
    
    std::vector<std::shared_ptr<ActiveNote>> activeNotes;

public:
    Note() = default;
    Note(int originalPitch) : mPitch_(originalPitch) {}
    
    [[nodiscard]] virtual auto pitch() const -> int {
        return mPitch_;
    }

    auto add(int originalPitch, int randomPitch, int velocity) -> std::shared_ptr<ActiveNote>{
        // Add an active note to the key.
        if (!isValidNote(originalPitch, randomPitch, velocity)) {
            return nullptr;
        }

        auto newNote = std::make_shared<ActiveNote>(originalPitch, randomPitch, velocity);

        // TODO: Do we need this?
        if (MIDI::RANGE_LOW < velocity && velocity <= MIDI::RANGE_HIGH) {
            activeNotes.push_back(newNote);
        }

        return newNote;
    }

    auto getActiveNotes() -> std::vector<std::shared_ptr<ActiveNote>>& {
        return activeNotes;
    }

    // TODO: Can this be simplified with pointers?
    auto setRange(int low, int high) -> void { // NOLINT
        rangeLow_ = low;
        rangeHigh_ = high;
    }

    auto contains(int note) -> bool {
        // Check if note is active.
        for (const auto &activeNote : activeNotes) { // NOLINT
            if (activeNote->pitch() == note) {
                return true;
            }
        }
        
        return false;
    }

    auto isValidNote(int originalPitch, int randomPitch, int velocity) -> bool { // NOLINT
        // Octave check
        if (Note::getPitchClass(originalPitch) != Note::getPitchClass(randomPitch)) {
            return false;
        }
        
        // Note limit check (only for note-on events)
        if (velocity > 0) {
            if (this->activeNotes.size() >= MAX_NOTES || this->contains(randomPitch)) {
                return false;
            }
        }
        
        return true;
    }
    
    static auto getPitchClass(int pitch) -> int {
        return pitch % MIDI::OCTAVE;
    }
};
