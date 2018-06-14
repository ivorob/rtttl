#include <rtttl.h>

namespace {

class ArrayRtttlSongReader : public SimpleAudio::AbstractRtttlSongReader {
public:
    ArrayRtttlSongReader(const char *songs[], int count)
        : songs(songs),
          count(count),
          currentSong()
    {
    }

    const char *nextSong() override {
        if (this->currentSong >= count) {
            this->currentSong = 0;
        }

        return this->songs[this->currentSong++];
    }

    const char *prevSong() override {
        if (this->currentSong == 0) {
            this->currentSong = count;
        }

        return this->songs[--this->currentSong];
    }
private:
    const char **songs;
    int count;
    int currentSong;
};

}

#define TONE_PIN 13

const char *songs[] = {
    "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6",
    "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6",
    "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#",
    "MissionImp:d=16,o=8,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d",
};

ArrayRtttlSongReader reader(songs, sizeof(songs) / sizeof(char *));
SimpleAudio::RtttlPlayer player(TONE_PIN, reader);

void setup() {
    pinMode(TONE_PIN, OUTPUT);  
    Serial.begin(9600);
    Serial.print("Songs count: ");
    Serial.println(sizeof(songs) / sizeof(char *));
}

void loop() {  
    player.stepNext();              // Load song to player

    Serial.print("Playing ");
    Serial.println(player.getSongName());
    while (!player.eof()) {         // Do we have notes to play?
        player.play();              // Play note
    }  

    Serial.println("Pause.");
    delay(2000);
}
