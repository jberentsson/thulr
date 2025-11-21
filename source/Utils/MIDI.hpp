#pragma once

#include "../Exceptions/Exceptions.hpp"

#include <cstdint>
#include <stdexcept>

namespace MIDI {
    constexpr int OCTAVE = 12;
    constexpr int KEYBOARD_SIZE = 128;
    constexpr int KEYBOARD_OCTAVES = 10;
    constexpr int RANGE_LOW = 0;
    constexpr int RANGE_HIGH = 127;

    class Note {
    private:
        uint8_t value_;
        
    public:        
        // MIDI Note Datatype
        constexpr Note() : value_(0) {}
        constexpr explicit Note(uint8_t note) : value_(note) {
            if (note > RANGE_HIGH) {
                throw NoteOutOfRangeException();
            }
        }
    };

    class Velocity : public Note {};

    namespace Notes {
        // Octave 0 (lowest octave)
        constexpr int NoteC0 = 0;
        constexpr int NoteCS0 = 1;
        constexpr int NoteDB0 = 1;
        constexpr int NoteD0 = 2;
        constexpr int NoteDS0 = 3;
        constexpr int NoteEB0 = 3;
        constexpr int NoteE0 = 4;
        constexpr int NoteF0 = 5;
        constexpr int NoteFS0 = 6;
        constexpr int NoteGB0 = 6;
        constexpr int NoteG0 = 7;
        constexpr int NoteGS0 = 8;
        constexpr int NoteAB0 = 8;
        constexpr int NoteA0 = 9;
        constexpr int NoteAS0 = 10;
        constexpr int NoteBB0 = 10;
        constexpr int NoteB0 = 11;

        // Octave 1
        constexpr int NoteC1 = 12;
        constexpr int NoteCS1 = 13;
        constexpr int NoteDB1 = 13;
        constexpr int NoteD1 = 14;
        constexpr int NoteDS1 = 15;
        constexpr int NoteEB1 = 15;
        constexpr int NoteE1 = 16;
        constexpr int NoteF1 = 17;
        constexpr int NoteFS1 = 18;
        constexpr int NoteGB1 = 18;
        constexpr int NoteG1 = 19;
        constexpr int NoteGS1 = 20;
        constexpr int NoteAB1 = 20;
        constexpr int NoteA1 = 21;
        constexpr int NoteAS1 = 22;
        constexpr int NoteBB1 = 22;
        constexpr int NoteB1 = 23;

        // Octave 2
        constexpr int NoteC2 = 24;
        constexpr int NoteCS2 = 25;
        constexpr int NoteDB2 = 25;
        constexpr int NoteD2 = 26;
        constexpr int NoteDS2 = 27;
        constexpr int NoteEB2 = 27;
        constexpr int NoteE2 = 28;
        constexpr int NoteF2 = 29;
        constexpr int NoteFS2 = 30;
        constexpr int NoteGB2 = 30;
        constexpr int NoteG2 = 31;
        constexpr int NoteGS2 = 32;
        constexpr int NoteAB2 = 32;
        constexpr int NoteA2 = 33;
        constexpr int NoteAS2 = 34;
        constexpr int NoteBB2 = 34;
        constexpr int NoteB2 = 35;

        // Octave 3
        constexpr int NoteC3 = 36;
        constexpr int NoteCS3 = 37;
        constexpr int NoteDB3 = 37;
        constexpr int NoteD3 = 38;
        constexpr int NoteDS3 = 39;
        constexpr int NoteEB3 = 39;
        constexpr int NoteE3 = 40;
        constexpr int NoteF3 = 41;
        constexpr int NoteFS3 = 42;
        constexpr int NoteGB3 = 42;
        constexpr int NoteG3 = 43;
        constexpr int NoteGS3 = 44;
        constexpr int NoteAB3 = 44;
        constexpr int NoteA3 = 45;
        constexpr int NoteAS3 = 46;
        constexpr int NoteBB3 = 46;
        constexpr int NoteB3 = 47;

        // Octave 4 (Middle C)
        constexpr int NoteC4 = 48;
        constexpr int NoteCS4 = 49;
        constexpr int NoteDB4 = 49;
        constexpr int NoteD4 = 50;
        constexpr int NoteDS4 = 51;
        constexpr int NoteEB4 = 51;
        constexpr int NoteE4 = 52;
        constexpr int NoteF4 = 53;
        constexpr int NoteFS4 = 54;
        constexpr int NoteGB4 = 54;
        constexpr int NoteG4 = 55;
        constexpr int NoteGS4 = 56;
        constexpr int NoteAB4 = 56;
        constexpr int NoteA4 = 57;
        constexpr int NoteAS4 = 58;
        constexpr int NoteBB4 = 58;
        constexpr int NoteB4 = 59;

        // Octave 5
        constexpr int NoteC5 = 60;
        constexpr int NoteCS5 = 61;
        constexpr int NoteDB5 = 61;
        constexpr int NoteD5 = 62;
        constexpr int NoteDS5 = 63;
        constexpr int NoteEB5 = 63;
        constexpr int NoteE5 = 64;
        constexpr int NoteF5 = 65;
        constexpr int NoteFS5 = 66;
        constexpr int NoteGB5 = 66;
        constexpr int NoteG5 = 67;
        constexpr int NoteGS5 = 68;
        constexpr int NoteAB5 = 68;
        constexpr int NoteA5 = 69;
        constexpr int NoteAS5 = 70;
        constexpr int NoteBB5 = 70;
        constexpr int NoteB5 = 71;

        // Octave 6
        constexpr int NoteC6 = 72;
        constexpr int NoteCS6 = 73;
        constexpr int NoteDB6 = 73;
        constexpr int NoteD6 = 74;
        constexpr int NoteDS6 = 75;
        constexpr int NoteEB6 = 75;
        constexpr int NoteE6 = 76;
        constexpr int NoteF6 = 77;
        constexpr int NoteFS6 = 78;
        constexpr int NoteGB6 = 78;
        constexpr int NoteG6 = 79;
        constexpr int NoteGS6 = 80;
        constexpr int NoteAB6 = 80;
        constexpr int NoteA6 = 81;
        constexpr int NoteAS6 = 82;
        constexpr int NoteBB6 = 82;
        constexpr int NoteB6 = 83;

        // Octave 7
        constexpr int NoteC7 = 84;
        constexpr int NoteCS7 = 85;
        constexpr int NoteDB7 = 85;
        constexpr int NoteD7 = 86;
        constexpr int NoteDS7 = 87;
        constexpr int NoteEB7 = 87;
        constexpr int NoteE7 = 88;
        constexpr int NoteF7 = 89;
        constexpr int NoteFS7 = 90;
        constexpr int NoteGB7 = 90;
        constexpr int NoteG7 = 91;
        constexpr int NoteGS7 = 92;
        constexpr int NoteAB7 = 92;
        constexpr int NoteA7 = 93;
        constexpr int NoteAS7 = 94;
        constexpr int NoteBB7 = 94;
        constexpr int NoteB7 = 95;

        // Octave 8
        constexpr int NoteC8 = 96;
        constexpr int NoteCS8 = 97;
        constexpr int NoteDB8 = 97;
        constexpr int NoteD8 = 98;
        constexpr int NoteDS8 = 99;
        constexpr int NoteEB8 = 99;
        constexpr int NoteE8 = 100;
        constexpr int NoteF8 = 101;
        constexpr int NoteFS8 = 102;
        constexpr int NoteGB8 = 102;
        constexpr int NoteG8 = 103;
        constexpr int NoteGS8 = 104;
        constexpr int NoteAB8 = 104;
        constexpr int NoteA8 = 105;
        constexpr int NoteAS8 = 106;
        constexpr int NoteBB8 = 106;
        constexpr int NoteB8 = 107;

        // Octave 9
        constexpr int NoteC9 = 108;
        constexpr int NoteCS9 = 109;
        constexpr int NoteDB9 = 109;
        constexpr int NoteD9 = 110;
        constexpr int NoteDS9 = 111;
        constexpr int NoteEB9 = 111;
        constexpr int NoteE9 = 112;
        constexpr int NoteF9 = 113;
        constexpr int NoteFS9 = 114;
        constexpr int NoteGB9 = 114;
        constexpr int NoteG9 = 115;
        constexpr int NoteGS9 = 116;
        constexpr int NoteAB9 = 116;
        constexpr int NoteA9 = 117;
        constexpr int NoteAS9 = 118;
        constexpr int NoteBB9 = 118;
        constexpr int NoteB9 = 119;

        // Octave 10 (highest octave)
        constexpr int NoteC10 = 120;
        constexpr int NoteCS10 = 121;
        constexpr int NoteDB10 = 121;
        constexpr int NoteD10 = 122;
        constexpr int NoteDS10 = 123;
        constexpr int NoteEB10 = 123;
        constexpr int NoteE10 = 124;
        constexpr int NoteF10 = 125;
        constexpr int NoteFS10 = 126;
        constexpr int NoteGB10 = 126;
        constexpr int NoteG10 = 127;

    } // namespace Notes
} // namespace MIDI
