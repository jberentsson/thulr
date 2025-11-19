#pragma once

class Quantizer {
public:
    enum {
        OCTAVE_SIZE = 12,
        KEYBOARD_SIZE = 128,
        KEYBOARD_OCTAVES = 10,
        INVALID_NOTE = -1
    };
    
    enum class RoundDirection : int { UP, DOWN };
    enum class QuantizeMode : int { TWELVE_NOTES, ALL_NOTES };
    enum class Note : int { OFF, ON };

    Quantizer();
    ~Quantizer();

    int quantize(int n);
    int add_note(int n);
    int set_range(int l, int h);
    int clear();
    Note get_note(int n);
    int round(int n);
    int round_up(int n);
    int round_down(int n);
    int set_round_direction(RoundDirection direction);
    int set_mode(QuantizeMode mode);

private:
    bool keyboard[KEYBOARD_SIZE];
    int range_low = 0;
    int range_high = 0;
    int note_count = 0;

    RoundDirection round_direction = RoundDirection::UP;
    QuantizeMode mode = QuantizeMode::ALL_NOTES;
};
