#include <gtest/gtest.h>

TEST(zsa, example)
{
    ASSERT_EQ(3, 1 + 2);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}