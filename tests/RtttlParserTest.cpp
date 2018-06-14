#include <gtest/gtest.h>
#include "RtttlParser.h"

TEST(RtttlParserTest, EmptyInit)
{
    SimpleAudio::RtttlParser parser;

    ASSERT_TRUE(parser.getName() != nullptr);
    ASSERT_TRUE(!strcmp("", parser.getName()));

    ASSERT_EQ(SimpleAudio::Duration::WHOLE, parser.getDefaultDuration());
    ASSERT_EQ(SimpleAudio::Octave::OCTAVE_4, parser.getDefaultOctave());
    ASSERT_EQ(100, parser.getDefaultTempo());
}

TEST(RtttlParserTest, Parse)
{
    SimpleAudio::RtttlParser parser;
    ASSERT_TRUE(parser.parseSong("fifth: d=4,o=5,b=63:8P,8G5,8G5,8G5,2D#5"));

    ASSERT_TRUE(parser.getName() != nullptr);
    ASSERT_TRUE(!strcmp("fifth", parser.getName()));

    ASSERT_EQ(SimpleAudio::Duration::QUARTER, parser.getDefaultDuration());
    ASSERT_EQ(SimpleAudio::Octave::OCTAVE_5, parser.getDefaultOctave());
    ASSERT_EQ(63, parser.getDefaultTempo());

    const SimpleAudio::Note& note1 = parser.parseNextNote();
    ASSERT_TRUE(note1.isValid());
    ASSERT_EQ(30000, note1.getDuration());
    ASSERT_EQ(0, note1.getFrequency());

    const SimpleAudio::Note& note2 = parser.parseNextNote();
    ASSERT_TRUE(note2.isValid());
    ASSERT_EQ(30000, note2.getDuration());
    ASSERT_EQ(784, note2.getFrequency());

    const SimpleAudio::Note& note3 = parser.parseNextNote();
    ASSERT_TRUE(note3.isValid());
    ASSERT_EQ(30000, note3.getDuration());
    ASSERT_EQ(784, note3.getFrequency());

    const SimpleAudio::Note& note4 = parser.parseNextNote();
    ASSERT_TRUE(note4.isValid());
    ASSERT_EQ(30000, note4.getDuration());
    ASSERT_EQ(784, note4.getFrequency());

    const SimpleAudio::Note& note5 = parser.parseNextNote();
    ASSERT_TRUE(note5.isValid());
    ASSERT_EQ(120000, note5.getDuration());
    ASSERT_EQ(622, note5.getFrequency());

    const SimpleAudio::Note& note6 = parser.parseNextNote();
    ASSERT_FALSE(note6.isValid());

    ASSERT_TRUE(parser.parseSong("fifth:d=4,o=5,b=63:8k,8a1"));
    const SimpleAudio::Note& note = parser.parseNextNote();
    ASSERT_FALSE(note.isValid());
}

TEST(RtttlParserTest, StreamParse)
{
    SimpleAudio::RtttlParser parser;
    char *song = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
    
    ASSERT_TRUE(parser.parseSong(song));
    const SimpleAudio::Note& note1 = parser.parseNextNote();
    ASSERT_TRUE(note1.isValid());
    ASSERT_EQ(7500, note1.getDuration());
    ASSERT_EQ(0, note1.getFrequency());

    ASSERT_TRUE(parser.parseSong(song));
    const SimpleAudio::Note& note2 = parser.parseNextNote();
    ASSERT_TRUE(note2.isValid());
    ASSERT_EQ(7500, note2.getDuration());
    ASSERT_EQ(0, note2.getFrequency());

    ASSERT_TRUE(parser.parseSong(song));
    const SimpleAudio::Note& note3 = parser.parseNextNote();
    ASSERT_TRUE(note3.isValid());
    ASSERT_EQ(7500, note3.getDuration());
    ASSERT_EQ(0, note3.getFrequency());
}
