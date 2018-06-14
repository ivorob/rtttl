#pragma once

namespace SimpleAudio {

class AbstractRtttlSongReader {
public:
    virtual ~AbstractRtttlSongReader() {}

    virtual const char *nextSong() = 0;
    virtual const char *prevSong() = 0;
};

}
