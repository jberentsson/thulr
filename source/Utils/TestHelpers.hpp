#pragma once

#define CATCH_CONFIG_ENABLE_STRINGIFICATION
#include <catch.hpp>
#include "Chords/Chords.hpp"

namespace Catch {
    template<>
    struct StringMaker<Chords::NoteMode> {
        static std::string convert(Chords::NoteMode mode) {
            //using namespace Chords;
            switch(mode) {
                case NoteMode::RETRIGGER: return "RETRIGGER";
                case NoteMode::LEGATO: return "LEGATO";
                default: 
                    return "UNKNOWN(" + std::to_string(static_cast<int>(mode)) + ")";
            }
        }
    };
}
