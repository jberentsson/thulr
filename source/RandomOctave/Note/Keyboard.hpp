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

public:
    Keyboard() {
        keyboard_.reserve(MIDI::KEYBOARD_SIZE);

        for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
            keyboard_.emplace_back(MIDI::Note(i)); // NOLINT
        }
    }

    auto add(int originalPitch, int randomPitch, int velocity) -> std::shared_ptr<ActiveNote> {
        // Add a new note to a key.
        if (originalPitch >= 0 && originalPitch < keyboard_.size()) {
            size_t noteCount = keyboard_[originalPitch].getActiveNotes().size();
            std::shared_ptr<ActiveNote> result = keyboard_[originalPitch].add(originalPitch, randomPitch, velocity);
            
            if (result != nullptr) {
                // If our new note message is valid.
                noteQueue_.push_back(result);
            }
            this->updateActiveNotes();
            return result;
        }

        return nullptr;
    }

    auto remove(int originalPitch) -> std::vector<std::shared_ptr<ActiveNote>> {
        // 1. Remove an active note.
        // 2. Add it to the noteQueue.
        // 3. Return the note queue.
        if (originalPitch >= 0 && originalPitch < keyboard_.size()) {
            for (const auto &cuan : this->keyboard_[originalPitch].getActiveNotes()) {
                this->noteQueue_.push_back(cuan);
            }

            this->keyboard_[originalPitch].clear();
            this->updateActiveNotes();
        }

        return this->noteQueue_;
    }

    auto clearNotesByPitchClass(int note) -> int { // NOLINT
        int pitchClass = note % MIDI::OCTAVE;

        for (int i = pitchClass; i < MIDI::KEYBOARD_SIZE; i += MIDI::OCTAVE) {
            this->keyboard_[i].clear();
        }

        return 0;
    }

    auto clear(int note) -> size_t {
        // Clear each of the active notes from each key.
        // TODO: Is this a duplicated function?
        if (note >= 0 && note < keyboard_.size()) {
            size_t cleared = keyboard_[note].clear();
            this->updateActiveNotes();
            return cleared;
        }

        return 0;
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
            note.clear();
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
        for (const auto& currNote : this->keyboard_) {
            const auto& notes = currNote.getActiveNotes();
            this->activeNotes_.insert(this->activeNotes_.end(), notes.begin(), notes.end());
        }
    }

    auto getActiveNotes() const -> const std::vector<std::shared_ptr<ActiveNote>>& {
        return this->activeNotes_;
    }
};
