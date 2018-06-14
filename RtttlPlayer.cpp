#include <Arduino.h>
#include "RtttlPlayer.h"

SimpleAudio::RtttlPlayer::RtttlPlayer(int outputPin, AbstractRtttlSongReader& reader)
    : reader(reader),
      currentNote(),
      outputPin(outputPin)
{
}

bool
SimpleAudio::RtttlPlayer::eof() const
{
    return this->currentNote >= this->parser.getNotesCount();
}

void
SimpleAudio::RtttlPlayer::play()
{
    if (this->currentNote < this->parser.getNotesCount()) {
        const SimpleAudio::Note& note = this->parser.getNote(this->currentNote);

        unsigned long duration = note.getDuration() / this->parser.getDefaultTempo();
        
        if (note.getFrequency() != 0) {
            tone(this->outputPin, note.getFrequency());
        }

        delay(duration);
    }

    noTone(this->outputPin);

    ++this->currentNote;
}

void
SimpleAudio::RtttlPlayer::stepNext()
{
    this->parser.parseSong(reader.nextSong());
    this->currentNote = 0;
}

void
SimpleAudio::RtttlPlayer::stepPrev()
{
    this->parser.parseSong(reader.prevSong());
    this->currentNote = 0;
}

void
SimpleAudio::RtttlPlayer::reset()
{
    this->currentNote = 0;
}

const char *
SimpleAudio::RtttlPlayer::getSongName() const
{
    return this->parser.getName();
}
