# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(zsa_capturesync STATIC
            capturesync.c
            )

# Consumers should #include <zsainternal/capturesync.h>
target_include_directories(zsa_capturesync PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Dependencies of this library
target_link_libraries(zsa_capturesync PUBLIC
    azure::aziotsharedutil
    zsainternal::logging)

# Define alias for other targets to link against
add_library(zsainternal::capturesync ALIAS zsa_capturesync)
