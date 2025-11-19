#include "Quantizer.hpp"
#include <iostream>

Quantizer::Quantizer() {
    this->clear();
    this->range_low = 0;
    this->range_high = KEYBOARD_SIZE - 1;
}

Quantizer::~Quantizer() {}

int Quantizer::quantize(int n) {
    // Check if the note is in range.
    if (n < 0 || n >= KEYBOARD_SIZE) {
        return INVALID_NOTE;
    }
    
    // If current note is already a valid key.
    if (this->keyboard[n]) {
        return n;
    }
    
    // Return the rounded value.
    return this->round(n);
}

int Quantizer::round(int n) {
    // Round depending on what rounding mode is active.
    if (this->round_direction == RoundDirection::UP) {
        return this->round_up(n);
    }
    
    return this->round_down(n);
}

int Quantizer::round_up(int n) {
    // Search up from n.
    for (int i = n + 1; i < KEYBOARD_SIZE; i++) {
        // We have reached the high limit.
        if (i > this->range_high) {
            return this->round_down(n);
        }

        // We have found the correct note.
        if (this->keyboard[i]) {
            return i;
        }

    // We have found the correct note.
    if (this->keyboard[i] == 1) {
      return i;
    }
  }

  return INVALID_NOTE;
}

int Quantizer::round_down(int n) {
    // Search down from n.
    for (int i = n - 1; i >= 0; i--) {
        // We have reached the low limit.
        if (i < this->range_low) {
            return this->round_up(n);
        }

        // We have found the correct note.
        if (this->keyboard[i]) {
            return i;
        }
    }
    
    return INVALID_NOTE;
}

int Quantizer::set_range(int l, int h) {
    // Set the quantizer output range.
    if (l >= 0 && l < KEYBOARD_SIZE && h >= 0 && h < KEYBOARD_SIZE && l <= h) {
        this->range_low = l;
        this->range_high = h;
        return 0;
    }
    return -1;
}

int Quantizer::clear() {
    // Clear all set notes from the keyboard.
    for (int i = 0; i < KEYBOARD_SIZE; i++) {
        this->keyboard[i] = false;
    }

  // Return error if the note value is out of range.
  //if (n < 0 || n >= KEYBOARD_SIZE) {
  //  return -1;
  //}

  return 0;
}

Quantizer::Note Quantizer::get_note(int n) {
    // Check if the note has been set.
    if (n < 0 || n >= KEYBOARD_SIZE) {
        return Note::OFF;
    }

    return this->keyboard[n] ? Note::ON : Note::OFF;
}

int Quantizer::add_note(int n) {
    // Add note to the keyboard.

    // Return error if the note value is out of range.
    if (n < 0 || n >= KEYBOARD_SIZE) {
        return -1;
    }

    // Depending on the mode add the note or notes to the keyboard.
    if (this->mode == QuantizeMode::TWELVE_NOTES) {
        int degree = n % OCTAVE_SIZE;
        
        // Add this note degree in every octave
        for (int octave = 0; octave < KEYBOARD_OCTAVES; octave++) {
            int current_note = (OCTAVE_SIZE * octave) + degree;
            if (current_note < KEYBOARD_SIZE) {
                this->keyboard[current_note] = true;
                this->note_count++;
            }
        }
    } else {
        // ALL_NOTES mode - just add the single note
        this->keyboard[n] = true;
        this->note_count++;
      }

  return 0;
}

int Quantizer::set_round_direction(RoundDirection direction) {
    this->round_direction = direction;
    return 0;
}

int Quantizer::set_mode(QuantizeMode mode) {
    this->mode = mode;
    return 0;
}
