#pragma once

#include <memory>
#include <algorithm>
#include <vector>
#include "Range.hpp"
#include "ActiveNote.hpp"
#include "Utils/MIDI.hpp"

class Key {
private:
    enum : uint8_t {
        MAX_NOTES = 4
    };

    int mPitch_ = 0;
    Range &range_;
    
    std::vector<std::shared_ptr<ActiveNote>> activeNotes;

public:
    Key(Range &range, int originalPitch) : mPitch_(originalPitch),range_(range) {}
    
    [[nodiscard]] virtual auto pitch() const -> int {
        return mPitch_;
    }

    auto add(int originalPitch, int randomPitch, int velocity) -> std::shared_ptr<ActiveNote>{
        // Add an active note to the key.
        if (!isValidNote(originalPitch, randomPitch, velocity)) {
            return nullptr;
        }

        auto newNote = std::make_shared<ActiveNote>(originalPitch, randomPitch, velocity);

        if (MIDI::RANGE_LOW < velocity && velocity <= MIDI::RANGE_HIGH) {
            activeNotes.push_back(newNote);
        }

        return newNote;
    }

    auto getActiveNotes() -> std::vector<std::shared_ptr<ActiveNote>>& {
        return activeNotes;
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
        if (MIDI::getPitchClass(originalPitch) != MIDI::getPitchClass(randomPitch)) {
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
};