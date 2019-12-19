# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_library(zsa_usb_cmd STATIC
            usbcommand.c
            usbstreaming.c
            )

# Consumers should #include <zsainternal/usbcommand.h>
target_include_directories(zsa_usb_cmd PUBLIC
    ${ZSA_PRIV_INCLUDE_DIR})

# Dependencies of this library
target_link_libraries(zsa_usb_cmd PUBLIC
    azure::aziotsharedutil
    LibUSB::LibUSB
    zsainternal::allocator
    zsainternal::image
    zsainternal::logging)

# Define alias for other targets to link against
add_library(zsainternal::usb_cmd ALIAS zsa_usb_cmd)
