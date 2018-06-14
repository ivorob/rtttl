#include <rtttl.h>

#define TONE_PIN 13

char *song = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";

SimpleAudio::SingleRtttlSongReader reader(song);
SimpleAudio::RtttlPlayer player(6, reader);

void setup() {
    pinMode(TONE_PIN, OUTPUT);  
}

void loop() {  
    player.stepNext();              // Load song to player
    while (!player.eof()) {         // Do we have notes to play?
        player.play();              // Play note
    }  
}
