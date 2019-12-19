# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(zsa_image STATIC
            image.c
            )

# Consumers should #include <zsainternal/image.h>
target_include_directories(zsa_image PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Dependencies of this library
target_link_libraries(zsa_image PUBLIC
    azure::aziotsharedutil
    zsainternal::allocator
    zsainternal::logging)

# Define alias for other targets to link against
add_library(zsainternal::image ALIAS zsa_image)
