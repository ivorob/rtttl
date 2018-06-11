#pragma once

namespace SimpleAudio {

typedef enum {
    WHOLE           = 1,
    HALF            = 2,
    QUARTER         = 4,
    EIGHTH          = 8,
    SIXTEENTH       = 16,
    THIRTY_SECOND   = 32,
} Duration;

typedef enum {
    OCTAVE_4 = 4,
    OCTAVE_5 = 5,
    OCTAVE_6 = 6,
    OCTAVE_7 = 7,
    OCTAVE_8 = 8,
    OCTAVE_MAX,
} Octave;

}
