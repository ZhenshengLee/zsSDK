// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This library
#include <zsa/zsa.h>

// Dependent libraries
#include <zsainternal/common.h>
#include <zsainternal/capture.h>
// #include <zsainternal/color.h>
// #include <zsainternal/color_mcu.h>
// #include <zsainternal/depth_mcu.h>
// #include <zsainternal/calibration.h>
// #include <zsainternal/capturesync.h>
// #include <zsainternal/transformation.h>
#include <zsainternal/logging.h>
// #include <azure_c_shared_utility/tickcounter.h>

// System dependencies
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

char ZSA_ENV_VAR_LOG_TO_A_FILE[] = ZSA_ENABLE_LOG_TO_A_FILE;

typedef struct _zsa_context_t
{
    // TICK_COUNTER_HANDLE tick_handle;

    // calibration_t calibration;

    // colormcu_t colormcu;

    // capturesync_t capturesync;

    // color_t color;

    bool color_started;
} zsa_context_t;

ZSA_DECLARE_CONTEXT(zsa_device_t, zsa_context_t);

#define DEPTH_CAPTURE (false)
#define COLOR_CAPTURE (true)
#define TRANSFORM_ENABLE_GPU_OPTIMIZATION (true)
#define ZSA_DEPTH_MODE_TO_STRING_CASE(depth_mode)                                                                      \
    case depth_mode:                                                                                                   \
        return #depth_mode
#define ZSA_COLOR_RESOLUTION_TO_STRING_CASE(color_resolution)                                                          \
    case color_resolution:                                                                                             \
        return #color_resolution
#define ZSA_IMAGE_FORMAT_TO_STRING_CASE(image_format)                                                                  \
    case image_format:                                                                                                 \
        return #image_format
#define ZSA_FPS_TO_STRING_CASE(fps)                                                                                    \
    case fps:                                                                                                          \
        return #fps

uint32_t zsa_device_get_installed_count(void)
{
    uint32_t device_count = 9;
    // usb_cmd_get_device_count(&device_count);
    return device_count;
}


#ifdef __cplusplus
}
#endif
