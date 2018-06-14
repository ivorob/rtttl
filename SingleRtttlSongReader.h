#pragma once

#include "AbstractRtttlSongReader.h"

namespace SimpleAudio {

class SingleRtttlSongReader : public AbstractRtttlSongReader {
public:
    SingleRtttlSongReader(const char *song);

    const char *nextSong() override;
    const char *prevSong() override;
private:
    const char *song;
};

}
