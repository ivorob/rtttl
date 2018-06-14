#include <string.h>
#include <ctype.h>
#include <new.h>
#include "RtttlParser.h"

SimpleAudio::RtttlParser::RtttlParser()
    : name(),
      duration(Duration::WHOLE),
      octave(Octave::OCTAVE_4),
      tempo(100),
      song(),
      position(-1)
{
}

void
SimpleAudio::RtttlParser::freeResources()
{
    if (this->name != nullptr) {
        delete[] this->name;
    }
    
    this->name = nullptr;
    this->duration = Duration::WHOLE;
    this->octave = Octave::OCTAVE_4;
    this->tempo = 100;
    this->song = nullptr;
    this->position = -1;
}

SimpleAudio::RtttlParser::~RtttlParser()
{
    freeResources();
}

const char *
SimpleAudio::RtttlParser::getName() const
{
    return this->name != nullptr ? this->name : "";
}

SimpleAudio::Duration
SimpleAudio::RtttlParser::getDefaultDuration() const
{
    return this->duration;
}

SimpleAudio::Octave
SimpleAudio::RtttlParser::getDefaultOctave() const
{
    return this->octave;
}

uint8_t
SimpleAudio::RtttlParser::getDefaultTempo() const
{
    return this->tempo;
}

bool
SimpleAudio::RtttlParser::parseSong(const char *song)
{
    if (song == nullptr) {
        return false;
    }

    freeResources();

    this->position = 0;
    this->song = song;
    bool result = parseName(song, position) &&
        parseSettings(song, position) &&
        position != -1;
    if (!result) {
        freeResources();
    }

    return result;
}

void
SimpleAudio::RtttlParser::updateName(char *name)
{
    if (this->name != nullptr) {
        delete[] this->name;
    }

    this->name = name;
}

bool
SimpleAudio::RtttlParser::parseName(const char *song, int& position)
{
    if (position != -1) {
        int startPosition = position;
        while (song[position] != 0 && song[position] != ':') {
            ++position;
        }

        if (song[position] != 0 && position != startPosition) {
            int length = position - startPosition;

            char *buffer = new char[length + 1];
            if (buffer != nullptr) {
                memcpy(buffer, &song[startPosition], length);
                buffer[length] = 0;
                updateName(buffer);

                ++position;
                return true;
            }
        }
    }

    return false;
}

bool
SimpleAudio::RtttlParser::isDurationValid(int duration) const
{
    return duration >= SimpleAudio::Duration::WHOLE &&
           duration <= SimpleAudio::Duration::THIRTY_SECOND;
}

bool
SimpleAudio::RtttlParser::isOctaveValid(int octave) const
{
    return octave >= SimpleAudio::Octave::OCTAVE_4 &&
           octave < SimpleAudio::Octave::OCTAVE_MAX;
}

bool
SimpleAudio::RtttlParser::setSettingValue(char key, int value)
{
    switch (key) {
        case 'd':
            if (isDurationValid(value)) {
                this->duration = static_cast<SimpleAudio::Duration>(value);
            }
            break;
        case 'o':
            if (isOctaveValid(value)) {
                this->octave = static_cast<SimpleAudio::Octave>(value);
            }
            break;
        case 'b':
            this->tempo = value;
            break;
        default:
            return false;
    }

    return true;
}

bool
SimpleAudio::RtttlParser::parseSettings(const char *song, int& position)
{
    if (position != -1) {
        int startPosition = position;
        char key = 0;
        int value = 0;
        while (song[position] != 0 && song[position] != ':') {
            if (song[position] == 0x20 || song[position] == '\t') {
                if (!key) {
                    ++position;
                    continue;
                }

                break;
            }

            if (!key) {
                switch (song[position]) {
                    case 'd': //duration
                        /* passthru */
                    case 'b': //tempo
                        /* passthru */
                    case 'o': //octave
                        key = song[position++];
                        value = 0;
                        break;
                    default:
                        key = 0;
                        break;
                }

                if (song[position] != '=') {
                    ++position;
                    continue;
                }
            } else if (isdigit(song[position])) {
                value *= 10;
                value += song[position] - '0';
            } else if (song[position] == ',' && setSettingValue(key, value)) {
                key = 0;
            }

            ++position;
        }

        if (song[position] != 0 && position != startPosition) {
            ++position;
            return setSettingValue(key, value);
        }
    }

    return false;
}

SimpleAudio::Note
SimpleAudio::RtttlParser::obtainNote(int noteDuration, const char *note, int noteOctave, int dotsCount)
{
    int toneIndex = 0;
    switch ((note[0] | 0x20)) {
        case 'f':
        case 'g':
            toneIndex = ((note[0] | 0x20) - 'c') * 2 - 1;
            break;
        case 'c':
            /* passthru */
        case 'd':
            /* passthru */
        case 'e':
            /* passthru */
            toneIndex = ((note[0] | 0x20) - 'c') * 2;
            break;
        case 'a':
            /* passthru */
        case 'b':
            toneIndex = ((note[0] | 0x20) - 'a') * 2 + 9 /* +# */;
            break;
        case 'p':
            toneIndex = -1;
            break;
        default:
            return SimpleAudio::Note();
    }

    if (note[0] != 'P' && note[1] == '#') {
        ++toneIndex;
    }

    return SimpleAudio::Note(toneIndex,
        isDurationValid(noteDuration) ? noteDuration : getDefaultDuration(),
        isOctaveValid(noteOctave) ? noteOctave : getDefaultOctave(),
        dotsCount);
}

SimpleAudio::Note
SimpleAudio::RtttlParser::parseNextNote()
{
    if (this->song == nullptr || this->position == -1) {
        return SimpleAudio::Note();
    }

    int noteDuration = 0;
    int noteOctave = 0;
    int notePosition = -1;
    int endNotePosition = -1;
    int dotsCount = 0;
    char note[3] = {0};
    while (this->song[this->position] != 0) {
        if (this->song[this->position] == 0x20 || this->song[this->position] == '\t') {
            ++this->position;
            continue;
        }

        if (isdigit(this->song[this->position])) {
            if (notePosition == -1) {
                noteDuration *= 10;
                noteDuration += this->song[this->position] - '0';
            } else if (noteOctave == 0) {
                if (endNotePosition == -1) {
                    endNotePosition = this->position;
                }

                noteOctave += this->song[this->position] - '0';
            } else {
                noteOctave *= 10;
                noteOctave += this->song[this->position] - '0';
            }
        } else if (this->song[this->position] == '.' && notePosition != -1) {
            if (endNotePosition == -1) {
                endNotePosition = this->position;
            }

            ++dotsCount;
        } else if (this->song[this->position] != ',') {
            if (notePosition == -1) {
                notePosition = this->position;
            }
        } else if (notePosition != -1) {
            if (endNotePosition == -1) {
                endNotePosition = this->position;
            }

            memcpy(note, &this->song[notePosition], endNotePosition - notePosition);
            note[endNotePosition - notePosition] = 0;

            ++this->position;
            return obtainNote(noteDuration, note, noteOctave, dotsCount);
        } else {
            break;
        }

        ++this->position;
    }

    if (this->song[this->position] == 0 && notePosition != -1) {
        if (note[0] == 0) {
            if (endNotePosition == -1) {
                endNotePosition = this->position;
            }

            memcpy(note, &this->song[notePosition], endNotePosition - notePosition);
            note[endNotePosition - notePosition] = 0;
        }

        this->position = -1;
        return obtainNote(noteDuration, note, noteOctave, dotsCount);
    }

    return SimpleAudio::Note();
}
