/** \file COMMON.h
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 * Kinect For Azure SDK.
 */

#ifndef COMMON_H
#define COMMON_H

#include <zsa/zsatypes.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _guid_t
{
    uint8_t id[16];
} guid_t;

#define ZSA_IMU_SAMPLE_RATE 1666 // +/- 2%

#define MAX_FPS_IN_MS (33) // 30 FPS

#define COUNTOF(x) (sizeof(x) / sizeof(x[0]))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define STRINGIFY(string) #string

// Clock tick runs 90kHz and convert sec to micro sec
#define ZSA_90K_HZ_TICK_TO_USEC(x) ((uint64_t)(x)*100 / 9)
#define ZSA_USEC_TO_90K_HZ_TICK(x) ((x)*9 / 100)

#define MAX_SERIAL_NUMBER_LENGTH                                                                                       \
    (13 * 2) // Current schema is for 12 digits plus NULL, the extra size is in case that grows in the future.

inline static uint32_t zsa_convert_fps_to_uint(zsa_fps_t fps)
{
    uint32_t fps_int;
    switch (fps)
    {
    case ZSA_FRAMES_PER_SECOND_5:
        fps_int = 5;
        break;
    case ZSA_FRAMES_PER_SECOND_15:
        fps_int = 15;
        break;
    case ZSA_FRAMES_PER_SECOND_30:
        fps_int = 30;
        break;
    default:
        assert(0);
        fps_int = 0;
        break;
    }
    return fps_int;
}

inline static bool zsa_convert_resolution_to_width_height(zsa_color_resolution_t resolution,
                                                          uint32_t *width_out,
                                                          uint32_t *height_out)
{
    uint32_t width = 0;
    uint32_t height = 0;
    switch (resolution)
    {
    case ZSA_COLOR_RESOLUTION_720P:
        width = 1280;
        height = 720;
        break;
    case ZSA_COLOR_RESOLUTION_1080P:
        width = 1920;
        height = 1080;
        break;
    case ZSA_COLOR_RESOLUTION_1440P:
        width = 2560;
        height = 1440;
        break;
    case ZSA_COLOR_RESOLUTION_1536P:
        width = 2048;
        height = 1536;
        break;
    case ZSA_COLOR_RESOLUTION_2160P:
        width = 3840;
        height = 2160;
        break;
    case ZSA_COLOR_RESOLUTION_3072P:
        width = 4096;
        height = 3072;
        break;
    default:
        return false;
    }

    if (width_out != NULL)
        *width_out = width;
    if (height_out != NULL)
        *height_out = height;
    return true;
}

inline static bool zsa_convert_depth_mode_to_width_height(zsa_depth_mode_t mode,
                                                          uint32_t *width_out,
                                                          uint32_t *height_out)
{
    uint32_t width = 0;
    uint32_t height = 0;
    switch (mode)
    {
    case ZSA_DEPTH_MODE_NFOV_2X2BINNED:
        width = 320;
        height = 288;
        break;
    case ZSA_DEPTH_MODE_NFOV_UNBINNED:
        width = 640;
        height = 576;
        break;
    case ZSA_DEPTH_MODE_WFOV_2X2BINNED:
        width = 512;
        height = 512;
        break;
    case ZSA_DEPTH_MODE_WFOV_UNBINNED:
        width = 1024;
        height = 1024;
        break;
    case ZSA_DEPTH_MODE_PASSIVE_IR:
        width = 1024;
        height = 1024;
        break;
    default:
        return false;
    }

    if (width_out != NULL)
        *width_out = width;
    if (height_out != NULL)
        *height_out = height;
    return true;
}

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H */
