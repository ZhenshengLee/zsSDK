# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(ZSA_COLOR_SYSTEM_SOURCES mfcamerareader.cpp)
    set(ZSA_COLOR_SYSTEM_DEPENDENCIES
        mf.lib
        mfplat.lib
        mfreadwrite.lib
        mfuuid.lib
        cfgmgr32.lib)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(ZSA_COLOR_SYSTEM_SOURCES uvc_camerareader.cpp)
    set(ZSA_COLOR_SYSTEM_DEPENDENCIES libuvc::libuvc libjpeg-turbo::libjpeg-turbo)
endif()

add_library(zsa_color STATIC
            color.cpp
            ${ZSA_COLOR_SYSTEM_SOURCES})

# Consumers should #include <zsainternal/color.h>
target_include_directories(zsa_color PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Dependencies of this library
target_link_libraries(zsa_color PUBLIC
                      zsainternal::logging
                      ${ZSA_COLOR_SYSTEM_DEPENDENCIES})

# Define alias for other targets to link against
add_library(zsainternal::color ALIAS zsa_color)
