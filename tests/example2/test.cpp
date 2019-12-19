// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <zsa/zsa.h>
#include <gtest/gtest.h>

TEST(zsa, example)
{
    zsa_device_t device = nullptr;
    // ASSERT_EQ(K4A_RESULT_SUCCEEDED, zsa_device_open(K4A_DEVICE_DEFAULT, &device));
    // zsa_device_close(device);
    // device = nullptr;
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
