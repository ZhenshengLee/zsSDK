# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(zsa_logging STATIC
            logging.cpp
            )

# Consumers should #include <zsainternal/logging.h>
target_include_directories(zsa_logging PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Dependencies of this library
target_link_libraries(zsa_logging PUBLIC
    azure::aziotsharedutil
    spdlog::spdlog
    zsainternal::rwlock
    zsainternal::global
    )

# Define alias for other targets to link against
add_library(zsainternal::logging ALIAS zsa_logging)
