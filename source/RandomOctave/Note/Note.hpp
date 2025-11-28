#pragma once

#include <memory>
#include <algorithm>
#include <vector>
#include "ActiveNote.hpp"
#include "Utils/MIDI.hpp"

#define MAX_NOTES 4 // NOLINT

class AbstractNote {
private:
    using ActiveNote = AbstractActiveNote;

    int mPitch_ = 0;
    int rangeHigh_ = MIDI::RANGE_HIGH;
    int rangeLow_ = MIDI::RANGE_LOW;
    
    std::vector<std::shared_ptr<AbstractActiveNote>> activeNotes;

public:
    AbstractNote() = default;
    AbstractNote(int originalPitch) : mPitch_(originalPitch) {}
    
    [[nodiscard]] virtual auto pitch() const -> int {
        return mPitch_;
    }

    auto add(int originalPitch, int randomPitch, int velocity) -> std::shared_ptr<ActiveNote>{
        if ((originalPitch % MIDI::OCTAVE != randomPitch % MIDI::OCTAVE) || 
            originalPitch != this->mPitch_ || 
           (this->activeNotes.size() >= MAX_NOTES || 
            this->contains(randomPitch)) && velocity > 0) {
            return NULL;
        }

        auto newNote = std::make_shared<AbstractActiveNote>(originalPitch, randomPitch, velocity);
        
        if (MIDI::RANGE_LOW < velocity && velocity <= MIDI::RANGE_HIGH) {
            activeNotes.push_back(newNote);
        } else {
            this->removeByPitch(randomPitch);
        }

        return newNote;
    }

    auto removeByPitch(int pitch) -> void {
        auto it = std::find_if(activeNotes.begin(), activeNotes.end(),
            [pitch](const auto& note) {
                return note->pitch() == pitch;
            });
        
        if (it != activeNotes.end()) {
            activeNotes.erase(it);
        }
    }

    auto clear() -> int {
        int activeCount = activeNotes.size();
        activeNotes.clear();
        return activeCount;
    }

    // Const version for read-only access
    [[nodiscard]] auto getActiveNotes() const -> const std::vector<std::shared_ptr<AbstractActiveNote>>& {
        return activeNotes;
    }

    // Non-const version for modification
    auto getActiveNotes() -> std::vector<std::shared_ptr<AbstractActiveNote>>& {
        return this->activeNotes;
    }

    auto getActiveNoteCount() -> int {
        return this->activeNotes.size();
    }

    auto setRange(int low, int high) -> void { // NOLINT
        rangeLow_ = low;
        rangeHigh_ = high;
    }

    auto contains(int note) -> bool {
        for (const auto &activeNote : activeNotes) {
            if (activeNote->pitch() == note) {
                return true;
            }
        }
        
        return false;
    }
};
