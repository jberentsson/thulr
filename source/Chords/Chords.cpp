#include <memory>
#include <vector>
#include <random>
#include "Utils/MIDI.hpp"
#include "Chords.hpp"

Chords::Chords() {
    this->noteMode_ = NoteMode::RETRIGGER;
    this->isRecievingNotes_ = false;

    for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
        this->keyboard_.push_back(std::make_shared<Key>(Key(i)));
        this->recordingNoteCount_[i] = 0;
        this->playbackNoteCount_[i] = 0;
    }
}

auto Chords::reciveNotes() -> bool {
    // Trigger the note assign mode.
    this->isRecievingNotes_ = true;
    return this->isRecievingNotes_;
}

auto Chords::note(MIDI::Note note) -> int {
    if (this->isRecievingNotes_) {
        if (this->activeKey_ == nullptr) {
            // Active key ON.
            this->setActiveKey(note.pitch());
        } else {
            // Add note to key.
            this->chordNote(note);
        }
    } else {    
        if (!this->keyboard_[note.pitch()]->notes().empty()) {
            if (this->noteOrder_ == NoteOrder::RANDOM) {
                this->playNotesRandom(note);
            } else {
                this->playNotesInOrder(note);
            }
        }
    }

    return 0;
}

auto Chords::chordNote(MIDI::Note note) -> int {
    // Handle chord note assignment.
     if (note.velocity() > 0) {
        this->addChordNote(note);
    } else {
        this->releaseChordNote(note);
    }

    return 0;
}

auto Chords::setActiveKey(int key) -> std::shared_ptr<Key>& {
    if (key >= MIDI::RANGE_LOW && key <= MIDI::RANGE_HIGH) {
        this->activeKey_ = this->keyboard_.at(key);
        this->activeKey_->clear();
    }

    return this->activeKey_;
}

auto Chords::addChordNote(MIDI::Note note) -> int {
    // Handle adding a note to a key.
    if (!note.valid()) {
        return 1;
    }

    if(this->activeKey_ == nullptr) {
        return 2;
    }
    
    // The rest of the notes are assigned to the key.
    this->activeKey_->add(note.pitch());
    this->addToActive(note.pitch());

    return 0;
}

auto Chords::addToActive(int notePitch) -> int {
    // Store the notes untill we have released them all.
    if ((notePitch <= MIDI::RANGE_HIGH) && (notePitch >= MIDI::RANGE_LOW)) {
        this->recordingNoteCount_[notePitch]++;
        return this->recordingNoteCount_[notePitch];
    }

    return -1;
}

auto Chords::removeFromActive(int pitchValue) -> int {
    // Remove a released note from active array.
    if (this->recordingNoteCount_[pitchValue] > 0) {
        this->recordingNoteCount_[pitchValue]--;
    }
    
    return this->recordingNoteCount_[pitchValue];
}

auto Chords::releaseChordNote(MIDI::Note note) -> int {
    this->removeFromActive(note.pitch());

    // Check if all released..
    if (this->activeKey_ != nullptr) {
        auto& chordNotes = this->activeKey_->notes();
        
        // Only check if chord actually has notes.
        if (!chordNotes.empty()) {
            bool allReleased = true;
            
            for (const auto& chordNote : chordNotes) {
                int pitch = chordNote->pitch();
                if (this->recordingNoteCount_[pitch] > 0) {
                    allReleased = false;
                    break;
                }
            }
            
            if (allReleased) {
                this->isRecievingNotes_ = false;
                this->activeKey_ = nullptr;
            }
        }
        // If chord is empty, don't end recording.
    }
    
    return 0;
}

auto Chords::playNotesRandom(MIDI::Note note) -> int {
    if (this->isRecievingNotes_) {
        return 0;
    }

    // Play the key notes in a random order.
    if (!this->keyboard_[note.pitch()]->notes().empty()) {
        auto currentNotes = this->keyboard_[note.pitch()]->notes();
        auto notesRemaining = currentNotes.size();

        while (notesRemaining > 0) {
            std::uniform_int_distribution<> dis(0, (int) currentNotes.size() - 1);
            int randomIndex = dis(this->gen);
            notesRemaining--;
            auto currentNote = currentNotes.at(randomIndex);
            currentNotes.erase(currentNotes.begin() + randomIndex);
            this->handleNoteOut(MIDI::Note(currentNote->pitch(), note.velocity()));
        }
    }

    return 0;
}

auto Chords::playNotesInOrder(MIDI::Note note) -> int {
    if (this->isRecievingNotes_) {
        return 0;
    }
    
    // Play the notes in the order they were recorded in.
    if (!this->keyboard_[note.pitch()]->notes().empty()) {
        const auto& sourceNotes = this->keyboard_[note.pitch()]->notes();
        
        for(const auto& currentNote : sourceNotes) {
            MIDI::Note currentNoteOut = MIDI::Note(currentNote->pitch(), note.velocity());
            this->handleNoteOut(currentNoteOut);
        }
    }

    return 0;
}

auto Chords::handleNoteOut(MIDI::Note note) -> void {
    // Handle note out messages.

    if (!note.valid()) {
        return;
    }

    int& count = this->playbackNoteCount_[note.pitch()];

    if (note.velocity() == 0) { // NOTE OFF
        if (count > 0) {
            count--;
            
            if (count == 0) {
                // Only send NOTE_OFF when 
                // no chords need this note.
                this->queueNote(note);
            }
        }
    } else { // NOTE ON
        if (this->sendNoteOn(note)) {
            this->queueNote(note);
        }
    }
}

auto Chords::sendNoteOn(MIDI::Note note) -> bool {
    // Handle what happens when we send notes out depending on note mode.

    int& count = this->playbackNoteCount_[note.pitch()];

    bool sendNoteOn = false;
    bool noteWasActive = (count > 0);
    
    count++;

    if (count == 1) {
        sendNoteOn = true; // First activation
    } else {
        if (this->noteMode_ == NoteMode::RETRIGGER) {
            sendNoteOn = true; // Always retrigger
        } else if (this->noteMode_ == NoteMode::LEGATO) {
            sendNoteOn = !noteWasActive;
        }
        // else NORMAL mode.
    }

    return sendNoteOn;
}

auto Chords::queueNote(MIDI::Note note) -> void {
    // Push a note to the note queue.
    this->noteQueue_.push_back(std::make_shared<MIDI::Note>(note));
}

auto Chords::setNoteMode(NoteMode mode) -> NoteMode {
    this->noteMode_ = mode;
    return this->noteMode_;
}

auto Chords::setNoteOrder(NoteOrder order) -> NoteOrder {
    this->noteOrder_ = order;
    return this->noteOrder_;
}

auto Chords::clear() -> void {
    // Clear all notes from every key.
    for (const auto &currentNote : this->keyboard_) {
        currentNote->clear();
    }
    
    this->clearActiveNotes();
}

auto Chords::clear(int noteValue) -> void {
    // Clear notes from a specific key.
    
    if ((noteValue >= MIDI::RANGE_LOW) && (noteValue <= MIDI::RANGE_HIGH)) {
        this->keyboard_[noteValue]->clear();
        this->playbackNoteCount_[noteValue] = 0;
    }
}

auto Chords::clearActiveNotes() -> void {
    for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
        this->recordingNoteCount_[i] = 0;
        this->playbackNoteCount_[i] = 0;
    }
}

auto Chords::playbackNoteCount() -> int {
    int total = 0;

    for (const auto& currentNoteCount : this->playbackNoteCount_) {
        total += currentNoteCount;
    }

    return total;
}

auto Chords::recordingNoteCount() -> int {
    int total = 0;
    
    for (const auto& currentNoteCount : this->recordingNoteCount_) {
        total += currentNoteCount;
    }

    return total;
}
