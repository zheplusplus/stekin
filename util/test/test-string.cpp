#include <gtest/gtest.h>

#include "../string.h"

TEST(String, ReplaceAll)
{
    ASSERT_EQ("aa", util::replace_all("aa", "aaa", "bbb"));
    ASSERT_EQ("@@", util::replace_all("@@", "@", "@"));
    ASSERT_EQ("$$", util::replace_all("$", "$", "$$"));
    ASSERT_EQ("$#$#", util::replace_all("$$", "$", "$#"));
    ASSERT_EQ("$#$#", util::replace_all("##", "#", "$#"));
}

TEST(String, FromInt)
{
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(std::string(1, char('0' + i)), util::str(i));
    }
    ASSERT_EQ("1024", util::str(1024));

    for (int i = -9; i < 0; ++i) {
        ASSERT_EQ("-" + std::string(1, char('0' - i)), util::str(i));
    }
    ASSERT_EQ("-4096", util::str(-4096));
}

TEST(String, FromDouble)
{
    ASSERT_EQ("1", util::str(1.0));
    ASSERT_EQ("1.5", util::str(1.5));
    ASSERT_EQ("2.5", util::str(2.5));
    ASSERT_EQ("-2.5", util::str(-2.5));
    ASSERT_EQ("-1.5", util::str(-1.5));
}
