#pragma once

class AbstractActiveNote {
    private:
        int mOriginalPitch;
        int mProcessedPitch;
        int mVelocity;

    public:
        AbstractActiveNote(int originalPitch, int processedPitch, int velocity) 
            : mOriginalPitch(originalPitch), mProcessedPitch(processedPitch), mVelocity(velocity) {
        }

        virtual int originalPitch() const {
            return mOriginalPitch;
        }

        virtual int pitch() const {
            return mProcessedPitch;
        }

        virtual int velocity() const {
            return mVelocity;
        }
};
