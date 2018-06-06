#include <gtest/gtest.h>
#include "SimpleAudioNote.h"

TEST(NoteTest, init)
{
    SimpleAudio::Note note;
    ASSERT_EQ(SimpleAudio::Duration::WHOLE, note.getDuration());
    ASSERT_EQ(0, note.getFrequency());

    SimpleAudio::Note note1(0, 8, 5);
    ASSERT_EQ(SimpleAudio::Duration::EIGHTH, note1.getDuration());
    ASSERT_EQ(523, note1.getFrequency());
}
