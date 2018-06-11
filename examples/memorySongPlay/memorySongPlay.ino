#include <RtttlParser.h>

#define TONE_PIN 13

SimpleAudio::RtttlParser parser;

char *song = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";

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
    Serial.println("Plaing...");

    for (int i = 0; i < parser.getNotesCount(); ++i) {
      const SimpleAudio::Note& note = parser.getNote(i);

      int duration = (60 * 1000L / parser.getDefaultTempo()) * 4 / note.getDuration();
      Serial.print("Frequency: ");
      Serial.print(note.getFrequency());
      Serial.print(" Duration: ");
      Serial.println(duration);
      tone(TONE_PIN, note.getFrequency());
      delay(duration);
    }
    noTone(TONE_PIN);

    Serial.println("Stop.");
  }
    
  while (1) {
      delay(1000);
  }
}
