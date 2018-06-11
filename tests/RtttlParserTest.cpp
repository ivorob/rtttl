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

    ASSERT_EQ(0, parser.getNotesCount());
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

    ASSERT_EQ(5, parser.getNotesCount());

    ASSERT_EQ(30000, parser.getNote(0).getDuration());
    ASSERT_EQ(0, parser.getNote(0).getFrequency());

    ASSERT_EQ(30000, parser.getNote(1).getDuration());
    ASSERT_EQ(784, parser.getNote(1).getFrequency());

    ASSERT_EQ(30000, parser.getNote(2).getDuration());
    ASSERT_EQ(784, parser.getNote(2).getFrequency());

    ASSERT_EQ(30000, parser.getNote(3).getDuration());
    ASSERT_EQ(784, parser.getNote(3).getFrequency());

    ASSERT_EQ(120000, parser.getNote(4).getDuration());
    ASSERT_EQ(622, parser.getNote(4).getFrequency());

    ASSERT_FALSE(parser.parseSong("fifth:d=4,o=5,b=63:8k,8a1"));
    ASSERT_TRUE(!strcmp(parser.getName(), ""));
}

TEST(RtttlParserTest, LowCaseSongParse)
{
    SimpleAudio::RtttlParser parser;
    ASSERT_TRUE(parser.parseSong("MissionImp:d=16,o=8,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d"));

    ASSERT_TRUE(parser.getName() != nullptr);
    ASSERT_TRUE(!strcmp("MissionImp", parser.getName()));

    ASSERT_EQ(SimpleAudio::Duration::SIXTEENTH, parser.getDefaultDuration());
    ASSERT_EQ(SimpleAudio::Octave::OCTAVE_8, parser.getDefaultOctave());
    ASSERT_EQ(95, parser.getDefaultTempo());

    ASSERT_TRUE(parser.getNotesCount() != 0);

    ASSERT_EQ(7459, parser.getNote(19).getFrequency());

    ASSERT_TRUE(parser.parseSong("StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6"));

    ASSERT_TRUE(parser.getName() != nullptr);
    ASSERT_TRUE(!strcmp("StarWars", parser.getName()));

    ASSERT_EQ(SimpleAudio::Duration::QUARTER, parser.getDefaultDuration());
    ASSERT_EQ(SimpleAudio::Octave::OCTAVE_5, parser.getDefaultOctave());
    ASSERT_EQ(45, parser.getDefaultTempo());

    ASSERT_TRUE(parser.getNotesCount() != 0);

    ASSERT_EQ(0, parser.getNote(0).getFrequency());
    ASSERT_EQ(740, parser.getNote(1).getFrequency());
    ASSERT_EQ(988, parser.getNote(4).getFrequency());
}
