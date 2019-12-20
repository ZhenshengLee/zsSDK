// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <astra/capi/astra.h>
#include <gtest/gtest.h>

TEST(astra, example)
{
    ASSERT_EQ(ASTRA_STATUS_SUCCESS, astra_initialize());
    astra_terminate();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
