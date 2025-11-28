#pragma once

class AbstractActiveNote {
private:
    int mOriginalPitch_;
    int mProcessedPitch_;
    int mVelocity_;

public:
    AbstractActiveNote(int originalPitch, int processedPitch, int velocity)  // NOLINT
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

    [[nodiscard]] virtual auto velocity() const -> int {
        return mVelocity_;
    }

    // Optional: Add setters if you need to modify pitches after creation
    virtual void setProcessedPitch(int newPitch) {
        mProcessedPitch_ = newPitch;
    }

    // Optional: Add comparison for easier testing
    [[nodiscard]] auto equals(const AbstractActiveNote& other) const -> bool {
        return mOriginalPitch_ == other.mOriginalPitch_ &&
               mProcessedPitch_ == other.mProcessedPitch_ &&
               mVelocity_ == other.mVelocity_;
    }
};