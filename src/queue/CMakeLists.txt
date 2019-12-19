# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(zsa_queue STATIC
            queue.c
            )

# Consumers should #include <zsainternal/queue.h>
target_include_directories(zsa_queue PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

target_link_libraries(zsa_queue PUBLIC
    azure::aziotsharedutil
    zsainternal::allocator
    zsainternal::logging
)

# Define alias for other targets to link against
add_library(zsainternal::queue ALIAS zsa_queue)
