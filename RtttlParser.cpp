#include <string.h>
#include <ctype.h>
#include <new.h>
#include "RtttlParser.h"

SimpleAudio::RtttlParser::RtttlParser()
    : name(),
      duration(Duration::WHOLE),
      octave(Octave::OCTAVE_4),
      tempo(100),
      notesCount(),
      notes()
{
}

SimpleAudio::RtttlParser::~RtttlParser()
{
    delete[] this->name;
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

int
SimpleAudio::RtttlParser::getNotesCount() const
{
    return this->notesCount;
}

const SimpleAudio::Note&
SimpleAudio::RtttlParser::getNote(int index) const
{
    if (index < 0 || index > getNotesCount() || this->notes == nullptr) {
        return this->pause;
    }

    return *this->notes[index];
}

bool
SimpleAudio::RtttlParser::parseSong(const char *song)
{
    int position = 0;
    bool result = parseName(song, position) &&
        parseSettings(song, position) && 
        parseNotes(song, position) &&
        position != -1;
    if (!result) {
        //TODO: free data
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
           octave <= SimpleAudio::Octave::OCTAVE_7;
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

bool
SimpleAudio::RtttlParser::addNote(int index, int noteDuration, const char *note, int noteOctave)
{
    if (this->notes == nullptr || index >= this->notesCount) {
        return true;
    }

    int toneIndex = 0;
    switch ((note[0] | 0x20)) {
        case 'c':
            /* passthru */
        case 'd':
            /* passthru */
        case 'e':
            /* passthru */
        case 'f':
            /* passthru */
        case 'g':
            toneIndex = ((note[0] | 0x20) - 'c') * 2;
            break;
        case 'a':
            /* passthru */
        case 'b':
            toneIndex = (note[0] | 0x20) - 'a' + 5 /* note before */ * 2 /* +# */;
            break;
        case 'p':
            toneIndex = -1;
            break;
        default:
            return false;
    }

    if (note[0] != 'P' && note[1] == '#') {
        ++toneIndex;
    }

    this->notes[index] = new SimpleAudio::Note(toneIndex,
            isDurationValid(noteDuration) ? noteDuration : getDefaultDuration(),
            isOctaveValid(noteOctave) ? noteOctave : getDefaultOctave());

    return true;
}

int
SimpleAudio::RtttlParser::fillNotes(const char *song, int& position)
{
    int count = 0;

    int noteDuration = 0;
    int noteOctave = 0;
    int notePosition = -1;
    char note[3];
    while (song[position] != 0) {
        if (song[position] == 0x20 || song[position] == '\t') {
            continue;
        }

        if (isdigit(song[position])) {
            if (notePosition == -1) {
                noteDuration *= 10;
                noteDuration += song[position] - '0';
            } else if (noteOctave == 0) {
                memcpy(note, &song[notePosition], position - notePosition);
                note[position - notePosition] = 0;

                noteOctave += song[position] - '0';
            } else {
                noteOctave *= 10;
                noteOctave += song[position] - '0';
            }
        } else if (song[position] != ',') {
            if (notePosition == -1) {
                notePosition = position;
            }
        } else if (notePosition != -1 && position - notePosition < 3) {
            if (noteOctave == 0) {
                memcpy(note, &song[notePosition], position - notePosition);
                note[position - notePosition] = 0;
            }

            if (!addNote(count, noteDuration, note, noteOctave)) {
                break;
            }

            ++count;

            noteDuration = 0;
            noteOctave = 0;
            notePosition = -1;
            note[0] = 0;
        } else {
            break;
        }

        ++position;
    }

    if (song[position] == 0 && notePosition != -1) {
        if (note[0] == 0) {
            memcpy(note, &song[notePosition], position - notePosition - 1);
            note[position - notePosition - 1] = 0;
        }

        addNote(count, noteDuration, note, noteOctave);

        ++count;
    }

    return count;
}

bool
SimpleAudio::RtttlParser::parseNotes(const char *song, int& position)
{
    if (position != -1) {
        int currentPosition = position;
        this->notesCount = fillNotes(song, currentPosition);
        if (this->notesCount != 0) {
            this->notes = new Note*[this->notesCount];
            this->notesCount = fillNotes(song, position);
            return this->notesCount != 0;
        }
    }

    return false;
}
