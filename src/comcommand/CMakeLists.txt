# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(zsa_com_cmd STATIC
            comcommand.c
            comstreaming.c
            )

# Consumers should #include <zsainternal/comcommand.h>
target_include_directories(zsa_com_cmd PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Dependencies of this library
target_link_libraries(zsa_com_cmd PUBLIC
    azure::aziotsharedutil
    zsainternal::allocator
    zsainternal::image
    zsainternal::logging)

# Define alias for other targets to link against
add_library(zsainternal::com_cmd ALIAS zsa_com_cmd)
