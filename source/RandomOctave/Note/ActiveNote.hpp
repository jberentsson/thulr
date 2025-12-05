#pragma once

#include "Utils/MIDI.hpp"

class ActiveNote {
private:
    int mOriginalPitch_;
    int mProcessedPitch_;
    int mVelocity_;

public:
    ActiveNote(int originalPitch, int processedPitch, int velocity)  // NOLINT
        : mOriginalPitch_(originalPitch), 
          mProcessedPitch_(processedPitch), 
          mVelocity_(velocity) {
    }

    [[nodiscard]] virtual auto originalPitch() const -> int {
        return mOriginalPitch_;
    }

    [[nodiscard]] virtual auto pitch() const -> int {
        return mProcessedPitch_;
    }

    virtual auto pitchClass() -> int {
        return MIDI::getPitchClass(this->mOriginalPitch_);
    }

    [[nodiscard]] virtual auto velocity() const -> int {
        return mVelocity_;
    }

    [[nodiscard]] auto equals(const ActiveNote& other) const -> bool {
        return mOriginalPitch_ == other.mOriginalPitch_ &&
               mProcessedPitch_ == other.mProcessedPitch_ &&
               mVelocity_ == other.mVelocity_;
    }
};