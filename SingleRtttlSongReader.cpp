#include "SingleRtttlSongReader.h"

SimpleAudio::SingleRtttlSongReader::SingleRtttlSongReader(const char *song)
    : song(song)
{
}

const char *
SimpleAudio::SingleRtttlSongReader::nextSong()
{
    return this->song;
}

const char *
SimpleAudio::SingleRtttlSongReader::prevSong()
{
    return this->song;
}
