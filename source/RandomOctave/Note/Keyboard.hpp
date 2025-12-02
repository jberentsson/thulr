#pragma once
#include <memory>
#include <vector>
#include <Utils/MIDI.hpp>
#include "RandomOctave/Note/ActiveNote.hpp"
#include "RandomOctave/Note/Note.hpp"
#include <iostream>

class Keyboard {
private:
    std::vector<Note> keyboard_;
    std::vector<std::shared_ptr<ActiveNote>> activeNotes_;
    std::vector<std::shared_ptr<ActiveNote>> noteQueue_;

    enum : uint8_t {
        MAX_NOTES = 4
    };

public:
    enum class NoteReturnCodes : uint8_t {
        OK = 0,
        OUT_OF_RANGE = 1,
        SOMETHING_ELSE = 2,
        DEGREE_MISMATCH = 3,
        OUT_OF_SPACE = 4
    };
    
    Keyboard() {
        // Populate the keyboard.
        keyboard_.reserve(MIDI::KEYBOARD_SIZE);

        for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
            keyboard_.emplace_back(MIDI::Note(i)); // NOLINT
        }
    }

    auto add(int originalPitch, int randomPitch, int velocity) -> NoteReturnCodes {
        // Validate pitches are in range.
        if (originalPitch < 0 || originalPitch >= static_cast<int>(keyboard_.size())) {
            return NoteReturnCodes::OUT_OF_RANGE;
        }
        
        // Check degree match (pitch class must be same).
        if (originalPitch % MIDI::OCTAVE != randomPitch % MIDI::OCTAVE) {
            return NoteReturnCodes::DEGREE_MISMATCH;
        }

        if (velocity > 0) {
            // === NOTE ON ===
            // Check if key has space for more notes.
            if (keyboard_[originalPitch].getActiveNotes().size() >= MAX_NOTES) {
                return NoteReturnCodes::OUT_OF_SPACE;
            }
            
            // Add the note.
            auto result = keyboard_[originalPitch].add(originalPitch, randomPitch, velocity);
            if (result == nullptr) {
                return NoteReturnCodes::SOMETHING_ELSE;
            }
            
            // Add to queues.
            noteQueue_.push_back(result);

            // Refresh active notes list.
            updateActiveNotes();
            
        } else {
            // === NOTE OFF ===
            // Find and remove all active notes with this original pitch.
            for (auto it = activeNotes_.begin(); it != activeNotes_.end(); ) {
                if ((*it)->originalPitch() == originalPitch) {
                    // Create note-off message,
                    noteQueue_.push_back(std::make_shared<ActiveNote>(
                        ActiveNote(originalPitch, (*it)->pitch(), 0)
                    ));
                    it = activeNotes_.erase(it);
                } else {
                    ++it;
                }
            }
            
            // Also clear from the specific key.
            keyboard_[originalPitch].getActiveNotes().clear();
        }
        
        return NoteReturnCodes::OK;
    }

    auto remove(int originalPitch) -> std::vector<std::shared_ptr<ActiveNote>> {
        // Create a note-off message.
        this->add(originalPitch, originalPitch, 0);
        return this->noteQueue_;
    }

    auto getActiveNotes(int note) -> std::vector<std::shared_ptr<ActiveNote>> {
        // Get all active notes from each of the notes of the keyboard.
        static std::vector<std::shared_ptr<ActiveNote>> empty;
        
        if (note >= 0 && note < keyboard_.size()) {
            return keyboard_[note].getActiveNotes();
        }
        
        return empty;
    }

    [[nodiscard]] auto getNote(int noteValue) const -> const Note& {
        // Get the note data from each of the keys.
        return keyboard_[noteValue];
    }

    auto containsNote(int noteValue) -> bool {
        // Check if a key has an active note with a specific pitch.
        for (int i = (noteValue % MIDI::OCTAVE); i < keyboard_.size(); i += MIDI::OCTAVE) { // NOLINT
            // Check each of the active notes for every key.
            for (const auto &currentActiveNote : keyboard_[i].getActiveNotes()) {
                if ((currentActiveNote->pitch() % MIDI::OCTAVE) == (noteValue % MIDI::OCTAVE)) {
                    return true;
                }
            }
        }

        return false;
    }

    auto removeAll() -> size_t {
        // Remove all active notes from each key.
        size_t removed_count = 0;

        for (auto& note : keyboard_) {
            removed_count += note.getActiveNotes().size();
            note.getActiveNotes().clear();
        }

        this->activeNotes_.clear();
        return removed_count;
    }

    auto getNoteQueue() -> std::vector<std::shared_ptr<ActiveNote>>& {
        // Return the keyboard notequeue.
        // Used when sending MIDI messages.
        return this->noteQueue_;
    }

    auto clearQueue() -> int {
        // Clear the note queue.
        // Used after sending all of the MIDI messages.
        this->noteQueue_.clear();
        return 0;
    }

    void updateActiveNotes() {
        // Update the active notes vector with data 
        // from each of the notes of the keyboard.
        this->activeNotes_.clear();
        for (auto& currNote : this->keyboard_) {
            const auto& notes = currNote.getActiveNotes();
            this->activeNotes_.insert(this->activeNotes_.end(), notes.begin(), notes.end());
        }
    }

    auto getActiveNotes() ->  std::vector<std::shared_ptr<ActiveNote>>& {
        return this->activeNotes_;
    }
};
