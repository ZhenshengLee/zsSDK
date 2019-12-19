# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
add_library(zsa_rwlock STATIC
            rwlock_linux.c
            )
else()
add_library(zsa_rwlock STATIC
            rwlock_win32.c
            )
endif()

# Consumers should #include <zsainternal/rwlock.h>
target_include_directories(zsa_rwlock PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Define alias for other targets to link against
add_library(zsainternal::rwlock ALIAS zsa_rwlock)
