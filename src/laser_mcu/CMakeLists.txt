# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(zsa_color_mcu STATIC
            color_mcu.c
            )

# Consumers should #include <zsainternal/color_mcu.h>
target_include_directories(zsa_color_mcu PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Dependencies of this library
target_link_libraries(zsa_color_mcu PUBLIC
    zsainternal::logging
    zsainternal::usb_cmd)

# Define alias for other targets to link against
add_library(zsainternal::color_mcu ALIAS zsa_color_mcu)
