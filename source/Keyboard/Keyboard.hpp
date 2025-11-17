#pragma once
#include "ActiveNote/ActiveNote.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>

#define MIN_CAPACITY 2
#define MAX_CAPACITY 5
<<<<<<< HEAD
#define RANGE_LOW    0
#define RANGE_HIGH   127
#define MIN_OCTAVE   0
#define MAX_OCTAVE   10
#define OCTAVE       12

class Keyboard {
    private:
        using ActiveNote = AbstractActiveNote;

        std::vector<std::unique_ptr<Keyboard::ActiveNote>> activeNotes;
        int rangeLow = RANGE_LOW;
        int rangeHigh = RANGE_HIGH;
        int minOctave = MIN_OCTAVE;
        int maxOctave = MAX_OCTAVE;

        int getPitchClass(int pitch) const;
        int clampPitchToRange(int pitch);
        int randomizeNote(int pitch);

    public:
        Keyboard(int low = RANGE_LOW, int high = RANGE_HIGH);
        
        int note(int pitch, int velocity);
        
        int clearNotesByPitchClass(int pitch);
        int removeAll();
        
        void updateRange(int low, int high);
        void setRandomRange(int low, int high);
        
        int maxCapacity() const {
            return MAX_CAPACITY;
        }
            
        int minCapacity() const {
            return MIN_CAPACITY;
        }

        const std::vector<std::unique_ptr<ActiveNote>>& getActiveNotes() const;
        
        void debugPrintActiveNotes() const;
=======
#define RANGE_LOW 0
#define RANGE_HIGH 127
#define MIN_OCTAVE 0
#define MAX_OCTAVE 10

class ActiveNote {
private:
  int m_originalPitch;
  int m_processedPitch;
  int m_velocity;

public:
  ActiveNote(int originalPitch, int processedPitch, int velocity)
      : m_originalPitch(originalPitch), m_processedPitch(processedPitch),
        m_velocity(velocity) {}

  int originalPitch() const { return m_originalPitch; }

  int pitch() const { return m_processedPitch; }

  int velocity() const { return m_velocity; }
};

class Keyboard {
private:
  static const int OCTAVE = 12;
  std::vector<std::unique_ptr<ActiveNote>> activeNotes;
  int rangeLow = RANGE_LOW;
  int rangeHigh = RANGE_HIGH;
  int minOctave = MIN_OCTAVE;
  int maxOctave = MAX_OCTAVE;

  int getPitchClass(int pitch) const;
  int clampPitchToRange(int pitch);
  int randomizeNote(int pitch);

public:
  Keyboard(int low = 0, int high = 127);

  int note(int pitch, int velocity);

  int clearNotesByPitchClass(int pitch);
  int removeAll();

  void updateRange(int low, int high);
  void setRandomRange(int low, int high);

  int maxCapacity() const { return MAX_CAPACITY; }

  int minCapacity() const { return MIN_CAPACITY; }

  const std::vector<std::unique_ptr<ActiveNote>> &getActiveNotes() const;

  void debugPrintActiveNotes() const;
>>>>>>> 1df950b89d1a5cc04b7a7befd778fe420ea4d224
};
