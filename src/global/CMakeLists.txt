# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(zsa_global STATIC
            global.c
            )

# Consumers should #include <zsainternal/global.h>
target_include_directories(zsa_global PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Define alias for other targets to link against
add_library(zsainternal::global ALIAS zsa_global)
