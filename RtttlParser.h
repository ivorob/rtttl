#pragma once

#include <stdint.h>
#include "SimpleAudioDefs.h"
#include "SimpleAudioNote.h"

namespace SimpleAudio {

class RtttlParser {
public:
    RtttlParser();
    ~RtttlParser();

    const char *getName() const;

    Duration getDefaultDuration() const;
    Octave getDefaultOctave() const;
    uint8_t getDefaultTempo() const;

    Note parseNextNote();

    bool parseSong(const char *song);
private:
    bool parseName(const char *song, int& position);
    bool parseSettings(const char *song, int& position);
private:
    void updateName(char *name);
    Note obtainNote(int noteDuration, const char *note, int noteOctave, int dotsCount);
    bool setSettingValue(char key, int value);
private:
    bool isDurationValid(int duration) const;
    bool isOctaveValid(int octave) const;
private:
    void freeResources();
private:
    char *name;
    Duration duration;
    Octave octave;
    int tempo;
    int position;
    const char *song;
};

}
