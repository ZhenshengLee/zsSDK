# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(zsa_allocator STATIC
            allocator.c
            )

# Consumers should #include <zsainternal/allocator.h>
target_include_directories(zsa_allocator PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Dependencies of this library
target_link_libraries(zsa_allocator PUBLIC
    azure::aziotsharedutil
    zsainternal::image
    zsainternal::logging)

# Define alias for other targets to link against
add_library(zsainternal::allocator ALIAS zsa_allocator)
