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
    ASSERT_TRUE(parser.parseSong("fifth:d=4,o=5,b=63:8P,8G5,8G5,8G5,2D#5"));

    ASSERT_TRUE(parser.getName() != nullptr);
    ASSERT_TRUE(!strcmp("fifth", parser.getName()));

    ASSERT_EQ(SimpleAudio::Duration::QUARTER, parser.getDefaultDuration());
    ASSERT_EQ(SimpleAudio::Octave::OCTAVE_5, parser.getDefaultOctave());
    ASSERT_EQ(63, parser.getDefaultTempo());

    ASSERT_EQ(5, parser.getNotesCount());

    ASSERT_EQ(SimpleAudio::Duration::EIGHTH, parser.getNote(0).getDuration());
    ASSERT_EQ(0, parser.getNote(0).getFrequency());

    ASSERT_EQ(SimpleAudio::Duration::EIGHTH, parser.getNote(1).getDuration());
    ASSERT_EQ(831, parser.getNote(1).getFrequency());

    ASSERT_EQ(SimpleAudio::Duration::EIGHTH, parser.getNote(2).getDuration());
    ASSERT_EQ(831, parser.getNote(2).getFrequency());

    ASSERT_EQ(SimpleAudio::Duration::EIGHTH, parser.getNote(3).getDuration());
    ASSERT_EQ(831, parser.getNote(3).getFrequency());

    ASSERT_EQ(SimpleAudio::Duration::HALF, parser.getNote(4).getDuration());
    ASSERT_EQ(622, parser.getNote(4).getFrequency());
}
