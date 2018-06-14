#include <gtest/gtest.h>
#include "SimpleAudioNote.h"

TEST(NoteTest, invalidNote)
{
    SimpleAudio::Note note;
    ASSERT_FALSE(note.isValid());
    ASSERT_EQ(240000, note.getDuration());
    ASSERT_EQ(0, note.getFrequency());
}

TEST(NoteTest, init)
{
    SimpleAudio::Note note1(0, 8, 5);
    ASSERT_EQ(523, note1.getFrequency());
    ASSERT_EQ(30000, note1.getDuration());

    SimpleAudio::Note note2(0, 8, 5, 1);
    ASSERT_EQ(523, note2.getFrequency());
    ASSERT_EQ(45000, note2.getDuration());

    SimpleAudio::Note note3(0, 8, 5, 2);
    ASSERT_EQ(523, note3.getFrequency());
    ASSERT_EQ(52500, note3.getDuration());
}
