#include <memory>
#include <vector>
#include <random>
#include "Utils/MIDI.hpp"
#include "Chords.hpp"

Chords::Chords() {
    this->noteMode_ = NoteMode::RETRIGGER;

    for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
        this->keyboard_.push_back(std::make_shared<Key>(Key(i)));
        this->noteCount_[i] = 0;
    }
}

auto Chords::reciveNotes() -> bool {
    this->isRecievingNotes_ = true;
    return this->isRecievingNotes_;
}

auto Chords::note(MIDI::Note note) -> int {
    if (this->isRecievingNotes_) {
        if (this->activeKey_ < MIDI::RANGE_LOW) {
            // Active key ON.
            this->setActiveKey(note);
        } else if (note.pitch() == this->activeKey_ && 
                   note.velocity() == 0 && 
                   this->keyboard_[note.pitch()]->notes().empty()) {
            // Active key OFF.
        } else {
            this->chordNote(note);
        }
    } else {    
        if (!this->keyboard_[note.pitch()]->notes().empty()) {
            // Play chord if key has one recorded.
            if (this->noteOrder_ == NoteOrder::RANDOM) {
                this->playNotesRandom(note);
            } else {
                this->playNotesInOrder(note);
            }
        }
    }

    return 0;
}

auto Chords::chordNote(MIDI::Note note) -> int { // NOLINT
     if (note.velocity() > 0) {
        this->addChordNote(note);
    } else {
        this->releaseChordNote(note);
    }

    return 0;
}

auto Chords::setActiveKey(int key) -> int {
    if (this->activeKey_ < MIDI::RANGE_LOW) {
        // Here we choose which key we want to assign the notes to.
        this->activeKey_ = key;

        // Clear any old notes from the key.
        auto& keyPtr = this->keyboard_.at(key);
        if (!keyPtr->notes().empty()) {
            keyPtr->notes().clear();
        }
    }
    
    return this->activeKey_;
}

auto Chords::addChordNote(MIDI::Note note) -> int {
    if (!note.valid()) {
        return 1;
    }
    
    // The rest of the notes are assigned to the key.
    this->keyboard_.at(this->activeKey_)->add(note.pitch());
    this->addToActive(note.pitch());

    return 0;
}

auto Chords::addToActive(int notePitch) -> int {
    // Store the notes untill we have released them all.
    if (notePitch >= MIDI::RANGE_LOW && notePitch <= MIDI::RANGE_HIGH)
    this->noteCount_[notePitch]++;
    return this->noteCount_[notePitch];
}

auto Chords::removeFromActive(int pitchValue) -> int {
    // Just update the count
    if (this->noteCount_[pitchValue] > 0) {
        this->noteCount_[pitchValue]--;
    }
    
    return this->noteCount_[pitchValue];
}

auto Chords::releaseChordNote(MIDI::Note note) -> int {
    this->removeFromActive(note.pitch());

    // Check if all released
    if (this->activeKey_ >= 0) {
        bool allReleased = true;
        auto& chordNotes = this->keyboard_[this->activeKey_]->notes();
        
        for (const auto& chordNote : chordNotes) {
            int pitch = chordNote->pitch();
            if (this->noteCount_[pitch] > 0) {
                allReleased = false;
                break;
            }
        }
        
        if (allReleased) {
            auto& chordToPlay = this->keyboard_[this->activeKey_]->notes();
            this->isRecievingNotes_ = false;
            this->activeKey_ = -1;
        }
    }
    
    return 0;
}

auto Chords::playNotesRandom(MIDI::Note note) -> int { // NOLINT
    if (!this->keyboard_[note.pitch()]->notes().empty()) {
        std::vector<std::shared_ptr<MIDI::Note>> currentNotes = this->keyboard_[note.pitch()]->notes();
        size_t notesRemaining = currentNotes.size();

        while (notesRemaining > 0) {
            std::uniform_int_distribution<> dis(0, currentNotes.size() - 1);
            int randomIndex = dis(this->gen);
            notesRemaining--;

            std::shared_ptr<MIDI::Note> currentNote = currentNotes.at(randomIndex);

            currentNotes.erase(currentNotes.begin() + randomIndex);

            this->handleNoteOut(MIDI::Note(currentNote->pitch(), note.velocity()));
        }
    }

    return 0;
}

auto Chords::playNotesInOrder(MIDI::Note note) -> int { // NOLINT
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
    if (!note.valid()) {
        return;
    }

    int& count = this->noteCount_[note.pitch()];

    if (note.velocity() == 0) { // NOTE OFF
        if (count > 0) {
            count--;
            
            if (count == 0) {
                // Only send NOTE_OFF when no chords need this note
                this->queueNote(note);
            }
        }
    } else { // NOTE ON
        if (this->sendNoteOn(note)) {
            this->queueNote(note);
        }
    }
}

auto Chords::sendNoteOn(int pitch) -> bool {
    int& count = this->noteCount_[pitch];

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
        this->noteCount_[noteValue] = 0;
    }
}

auto Chords::clearActiveNotes() -> void {
    for (const auto &currentNoteCount : this->noteCount_) {
        this->noteCount_[currentNoteCount] = 0;
    }
}
