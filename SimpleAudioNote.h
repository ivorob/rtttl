#pragma once

#include "SimpleAudioDefs.h"

namespace SimpleAudio {

class Note {
public:
    Note();
    Note(int toneIndex, int noteDuration, int noteOctave);

    Duration getDuration() const;
    int getFrequency() const;
private:
    int frequency;
    Duration duration;
};

}
