#pragma once

namespace MIDI {
    constexpr int OCTAVE = 12;

    namespace Notes {
        // Octave 0 (lowest octave)
        constexpr int C0 = 0;
        constexpr int CS0 = 1;
        constexpr int DB0 = 1;
        constexpr int D0 = 2;
        constexpr int DS0 = 3;
        constexpr int EB0 = 3;
        constexpr int E0 = 4;
        constexpr int F0 = 5;
        constexpr int FS0 = 6;
        constexpr int GB0 = 6;
        constexpr int G0 = 7;
        constexpr int GS0 = 8;
        constexpr int AB0 = 8;
        constexpr int A0 = 9;
        constexpr int AS0 = 10;
        constexpr int BB0 = 10;
        constexpr int B0 = 11;

        // Octave 1
        constexpr int C1 = 12;
        constexpr int CS1 = 13;
        constexpr int DB1 = 13;
        constexpr int D1 = 14;
        constexpr int DS1 = 15;
        constexpr int EB1 = 15;
        constexpr int E1 = 16;
        constexpr int F1 = 17;
        constexpr int FS1 = 18;
        constexpr int GB1 = 18;
        constexpr int G1 = 19;
        constexpr int GS1 = 20;
        constexpr int AB1 = 20;
        constexpr int A1 = 21;
        constexpr int AS1 = 22;
        constexpr int BB1 = 22;
        constexpr int B1 = 23;

        // Octave 2
        constexpr int C2 = 24;
        constexpr int CS2 = 25;
        constexpr int DB2 = 25;
        constexpr int D2 = 26;
        constexpr int DS2 = 27;
        constexpr int EB2 = 27;
        constexpr int E2 = 28;
        constexpr int F2 = 29;
        constexpr int FS2 = 30;
        constexpr int GB2 = 30;
        constexpr int G2 = 31;
        constexpr int GS2 = 32;
        constexpr int AB2 = 32;
        constexpr int A2 = 33;
        constexpr int AS2 = 34;
        constexpr int BB2 = 34;
        constexpr int B2 = 35;

        // Octave 3
        constexpr int C3 = 36;
        constexpr int CS3 = 37;
        constexpr int DB3 = 37;
        constexpr int D3 = 38;
        constexpr int DS3 = 39;
        constexpr int EB3 = 39;
        constexpr int E3 = 40;
        constexpr int F3 = 41;
        constexpr int FS3 = 42;
        constexpr int GB3 = 42;
        constexpr int G3 = 43;
        constexpr int GS3 = 44;
        constexpr int AB3 = 44;
        constexpr int A3 = 45;
        constexpr int AS3 = 46;
        constexpr int BB3 = 46;
        constexpr int B3 = 47;

        // Octave 4 (Middle C)
        constexpr int C4 = 48;
        constexpr int CS4 = 49;
        constexpr int DB4 = 49;
        constexpr int D4 = 50;
        constexpr int DS4 = 51;
        constexpr int EB4 = 51;
        constexpr int E4 = 52;
        constexpr int F4 = 53;
        constexpr int FS4 = 54;
        constexpr int GB4 = 54;
        constexpr int G4 = 55;
        constexpr int GS4 = 56;
        constexpr int AB4 = 56;
        constexpr int A4 = 57;
        constexpr int AS4 = 58;
        constexpr int BB4 = 58;
        constexpr int B4 = 59;

        // Octave 5
        constexpr int C5 = 60;
        constexpr int CS5 = 61;
        constexpr int DB5 = 61;
        constexpr int D5 = 62;
        constexpr int DS5 = 63;
        constexpr int EB5 = 63;
        constexpr int E5 = 64;
        constexpr int F5 = 65;
        constexpr int FS5 = 66;
        constexpr int GB5 = 66;
        constexpr int G5 = 67;
        constexpr int GS5 = 68;
        constexpr int AB5 = 68;
        constexpr int A5 = 69;
        constexpr int AS5 = 70;
        constexpr int BB5 = 70;
        constexpr int B5 = 71;

        // Octave 6
        constexpr int C6 = 72;
        constexpr int CS6 = 73;
        constexpr int DB6 = 73;
        constexpr int D6 = 74;
        constexpr int DS6 = 75;
        constexpr int EB6 = 75;
        constexpr int E6 = 76;
        constexpr int F6 = 77;
        constexpr int FS6 = 78;
        constexpr int GB6 = 78;
        constexpr int G6 = 79;
        constexpr int GS6 = 80;
        constexpr int AB6 = 80;
        constexpr int A6 = 81;
        constexpr int AS6 = 82;
        constexpr int BB6 = 82;
        constexpr int B6 = 83;

        // Octave 7
        constexpr int C7 = 84;
        constexpr int CS7 = 85;
        constexpr int DB7 = 85;
        constexpr int D7 = 86;
        constexpr int DS7 = 87;
        constexpr int EB7 = 87;
        constexpr int E7 = 88;
        constexpr int F7 = 89;
        constexpr int FS7 = 90;
        constexpr int GB7 = 90;
        constexpr int G7 = 91;
        constexpr int GS7 = 92;
        constexpr int AB7 = 92;
        constexpr int A7 = 93;
        constexpr int AS7 = 94;
        constexpr int BB7 = 94;
        constexpr int B7 = 95;

        // Octave 8
        constexpr int C8 = 96;
        constexpr int CS8 = 97;
        constexpr int DB8 = 97;
        constexpr int D8 = 98;
        constexpr int DS8 = 99;
        constexpr int EB8 = 99;
        constexpr int E8 = 100;
        constexpr int F8 = 101;
        constexpr int FS8 = 102;
        constexpr int GB8 = 102;
        constexpr int G8 = 103;
        constexpr int GS8 = 104;
        constexpr int AB8 = 104;
        constexpr int A8 = 105;
        constexpr int AS8 = 106;
        constexpr int BB8 = 106;
        constexpr int B8 = 107;

        // Octave 9
        constexpr int C9 = 108;
        constexpr int CS9 = 109;
        constexpr int DB9 = 109;
        constexpr int D9 = 110;
        constexpr int DS9 = 111;
        constexpr int EB9 = 111;
        constexpr int E9 = 112;
        constexpr int F9 = 113;
        constexpr int FS9 = 114;
        constexpr int GB9 = 114;
        constexpr int G9 = 115;
        constexpr int GS9 = 116;
        constexpr int AB9 = 116;
        constexpr int A9 = 117;
        constexpr int AS9 = 118;
        constexpr int BB9 = 118;
        constexpr int B9 = 119;

        // Octave 10 (highest octave)
        constexpr int C10 = 120;
        constexpr int CS10 = 121;
        constexpr int DB10 = 121;
        constexpr int D10 = 122;
        constexpr int DS10 = 123;
        constexpr int EB10 = 123;
        constexpr int E10 = 124;
        constexpr int F10 = 125;
        constexpr int FS10 = 126;
        constexpr int GB10 = 126;
        constexpr int G10 = 127;

    } // namespace Notes
} // namespace MIDI
