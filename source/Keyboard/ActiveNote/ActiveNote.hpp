#pragma once

class AbstractActiveNote {
    private:
        int mOriginalPitch_;
        int mProcessedPitch_;
        int mVelocity_;

    public:
        AbstractActiveNote(int originalPitch, int processedPitch, int velocity) 
            : mOriginalPitch_(originalPitch), mProcessedPitch_(processedPitch), mVelocity_(velocity) {
        }

        [[nodiscard]] virtual int originalPitch() const {
            return mOriginalPitch_;
        }

        [[nodiscard]] virtual int pitch() const {
            return mProcessedPitch_;
        }

        [[nodiscard]] virtual int velocity() const {
            return mVelocity_;
        }
};
