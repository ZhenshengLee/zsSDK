# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_executable(zsa_example2_test test.cpp)
target_link_libraries(zsa_example2_test PRIVATE zsa::zsa)
target_link_libraries(zsa_example2_test PRIVATE gtest::gtest)

zsa_add_tests(TARGET zsa_example2_test HARDWARE_REQUIRED TEST_TYPE FUNCTIONAL)
