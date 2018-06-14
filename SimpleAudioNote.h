#pragma once

#include "SimpleAudioDefs.h"

namespace SimpleAudio {

class Note {
public:
    Note();
    Note(int toneIndex, int noteDuration, int noteOctave, int dotsCount = 0);

    unsigned long getDuration() const;
    unsigned int getFrequency() const;

    bool isValid() const;
private:
    unsigned long calculateDuration(int duration, int dotsCount) const;
private:
    unsigned int frequency;
    unsigned long duration;
    bool valid;
};

}
