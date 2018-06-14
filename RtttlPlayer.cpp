#include <Arduino.h>
#include "RtttlPlayer.h"

SimpleAudio::RtttlPlayer::RtttlPlayer(int outputPin, AbstractRtttlSongReader& reader)
    : reader(reader),
      outputPin(outputPin)
{
}

bool
SimpleAudio::RtttlPlayer::play()
{
    const SimpleAudio::Note& note = this->parser.parseNextNote();
    if (note.isValid()) {
        unsigned long duration = note.getDuration() / this->parser.getDefaultTempo();

        if (note.getFrequency() != 0) {
            tone(this->outputPin, note.getFrequency(), duration);
        }

        delay(duration);
        noTone(this->outputPin);
        return true;
    }

    return false;
}

void
SimpleAudio::RtttlPlayer::stepNext()
{
    this->parser.parseSong(reader.nextSong());
}

void
SimpleAudio::RtttlPlayer::stepPrev()
{
    this->parser.parseSong(reader.prevSong());
}

void
SimpleAudio::RtttlPlayer::reset()
{
}

const char *
SimpleAudio::RtttlPlayer::getSongName() const
{
    return this->parser.getName();
}
