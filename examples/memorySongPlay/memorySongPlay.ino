#include <RtttlParser.h>

#define TONE_PIN 13

SimpleAudio::RtttlParser parser;

char *song = "MissionImp:d=16,o=8,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";

void setup() {
  Serial.begin(9600);

  Serial.print("Parsing song...\t");
  if (parser.parseSong(song)) {
    Serial.println("OK");
  } else {
    Serial.println("ERROR"); 
  }
}

void loop() {
  if (parser.getNotesCount() != 0) {
    Serial.print("Play: ");
    Serial.println(parser.getName());
  
    Serial.print("Default duration: ");
    Serial.println(parser.getDefaultDuration());

    Serial.print("Default octave: ");
    Serial.println(parser.getDefaultOctave());
    
    Serial.print("Default tempo: ");
    Serial.println(parser.getDefaultTempo());

    Serial.print("Song has ");
    Serial.print(parser.getNotesCount());
    Serial.println(" notes");
    Serial.println("NOTES:");

    for (int i = 0; i < parser.getNotesCount(); ++i) {
      const SimpleAudio::Note& note = parser.getNote(i);

      int duration = (60 * 1000L / parser.getDefaultTempo()) * 4 / note.getDuration();
      Serial.print("Frequency: ");
      Serial.println(note.getFrequency());
      Serial.print("Note duration: ");
      Serial.println(duration);
      tone(TONE_PIN, note.getFrequency());
      delay(duration);
    }

    Serial.println(" ");
  }
    
  delay(10000);
}
