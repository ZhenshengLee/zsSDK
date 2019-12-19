# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(zsa_math STATIC
            math.c
            )

# Consumers should #include <zsainternal/math.h>
target_include_directories(zsa_math PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Dependencies of this library
target_link_libraries(zsa_math PUBLIC
    )

# Define alias for other targets to link against
add_library(zsainternal::math ALIAS zsa_math)
