#pragma once

#include <cstdint>

namespace QuantizerEnums {
    enum class RoundDirection : uint8_t { 
        UP = 0,
        DOWN = 1,
        UP_DOWN = 2,
        DOWN_UP = 3,
        UP_OVERFLOW = 4,
        DOWN_UNDERFLOW = 5,
        NEAREST = 6,
        FURTHEST = 7
    };

    enum class QuantizeMode : uint8_t { 
        TWELVE_NOTES = 0, 
        ALL_NOTES = 1
    };

    enum class NoteData : uint8_t { 
        OFF = 0,
        ON = 1
    };
};
