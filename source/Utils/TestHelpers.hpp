#pragma once

#define CATCH_CONFIG_ENABLE_STRINGIFICATION
#ifdef __has_include
    #if __has_include(<catch2/catch.hpp>)
        #include <catch2/catch.hpp>
    #else
        #include <catch.hpp>
    #endif
//#else
//    #include <catch.hpp>
#endif
#include "Chords/Chords.hpp"

namespace Catch {
    template<>
    struct StringMaker<Chords::NoteMode> {
        static auto convert(Chords::NoteMode mode) -> std::string {
            //using namespace Chords;
            switch(mode) {
                case Chords::NoteMode::RETRIGGER: return "RETRIGGER";
                case Chords::NoteMode::LEGATO: return "LEGATO";
                default: 
                    return "UNKNOWN(" + std::to_string(static_cast<int>(mode)) + ")";
            }
        }
    };
}
