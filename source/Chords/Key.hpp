#pragma once

#include "Utils/MIDI.hpp"
#include <cstdint>
#include <memory>
#include <iostream>
#include <vector>

class Key {
private:
    std::vector<std::shared_ptr<MIDI::Note>> notes_;
    int pitch_ = 0;

public:
    Key(int noteValue) : pitch_(noteValue) {}
    ~Key() = default;

    auto notes() const -> const std::vector<std::shared_ptr<MIDI::Note>>& {
        return this->notes_;
    }

    auto notes() -> std::vector<std::shared_ptr<MIDI::Note>>& {
        return this->notes_;
    }

    auto clear() -> int {
        this->notes_.clear();
        return 0;
    }

    auto add(int pitchValue, int velocityValue = 0) -> int { // NOLINT
        // Add a note to a key.

        this->notes_.push_back(std::make_shared<MIDI::Note>(MIDI::Note(pitchValue, velocityValue)));
        std::cout << "key: note puched back " << pitchValue << " "
                  << velocityValue << " size:" << this->notes_.size() << "\n";
        return 0;
    }

    auto remove(std::shared_ptr<MIDI::Note>& newNote) -> int {
        // Remove a note from key.

        for (auto it = this->notes_.begin(); it != this->notes_.end(); ) {
            if ((*it)->pitch() == newNote->pitch()) {
                it = this->notes_.erase(it);
            } else {
                ++it;
            }
        }

        return 0;
    }

    auto count() -> size_t {
        return this->notes_.size();
    }

    [[nodiscard]] auto pitch() const -> uint8_t {
        return this->pitch_;
    }
};
