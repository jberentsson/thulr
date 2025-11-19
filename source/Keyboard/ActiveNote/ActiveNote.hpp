#pragma once

class AbstractActiveNote {
    private:
        int m_originalPitch;
        int m_processedPitch;
        int m_velocity;

    public:
        AbstractActiveNote(int originalPitch, int processedPitch, int velocity) 
            : m_originalPitch(originalPitch), m_processedPitch(processedPitch), m_velocity(velocity) {
        }

        virtual int originalPitch() const {
            return m_originalPitch;
        }

        virtual int pitch() const {
            return m_processedPitch;
        }

        virtual int velocity() const {
            return m_velocity;
        }
};
