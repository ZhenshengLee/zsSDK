// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This library
#include <zsa/zsa.h>

// Dependent libraries
#include <zsainternal/common.h>
#include <zsainternal/capture.h>
#include <zsainternal/color.h>
#include <zsainternal/color_mcu.h>
// #include <zsainternal/depth_mcu.h>
// #include <zsainternal/calibration.h>
#include <zsainternal/capturesync.h>
// #include <zsainternal/transformation.h>
#include <zsainternal/logging.h>
#include <azure_c_shared_utility/tickcounter.h>
#include <astra/capi/astra.h>

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
    // 所有传感器的配置信息
    // 以及数据传输,定时器,标定等需要的必要配置项
    TICK_COUNTER_HANDLE tick_handle;

    // calibration_t calibration;

    colormcu_t colormcu;

    capturesync_t capturesync;

    color_t color;

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
    LOG_ERROR("zs: zsa_device_get_installed_count not implemented", 0);
    return device_count;
}

color_cb_streaming_capture_t color_capture_ready;

void color_capture_ready(zsa_result_t result, zsa_capture_t capture_handle, void *callback_context)
{
    zsa_device_t device_handle = (zsa_device_t)callback_context;
    RETURN_VALUE_IF_HANDLE_INVALID(VOID_VALUE, zsa_device_t, device_handle);
    zsa_context_t *device = zsa_device_t_get_context(device_handle);
    capturesync_add_capture(device->capturesync, result, capture_handle, COLOR_CAPTURE);
}

zsa_result_t zsa_device_open(uint32_t index, zsa_device_t *device_handle)
{
    RETURN_VALUE_IF_ARG(ZSA_RESULT_FAILED, device_handle == NULL);

    // device是一台机器人所有视觉传感器的集合,支持同一相机的不同实例?
    // 此结构体包含视觉传感器的所有配置信息，定义在本文件
    zsa_context_t *device = NULL;
    zsa_result_t result = ZSA_RESULT_SUCCEEDED;
    zsa_device_t handle = NULL;
    const guid_t *container_id = NULL;
    char serial_number[MAX_SERIAL_NUMBER_LENGTH];
    size_t serial_number_size = sizeof(serial_number);

    allocator_initialize();

    // 此定义还没有找到
    device = zsa_device_t_create(&handle);
    result = ZSA_RESULT_FROM_BOOL(device != NULL);

    if (ZSA_SUCCEEDED(result))
    {
        result = ZSA_RESULT_FROM_BOOL((device->tick_handle = tickcounter_create()) != NULL);
    }

    if (ZSA_SUCCEEDED(result))
    {
        result = TRACE_CALL(colormcu_create(container_id, &device->colormcu));
    }

    if (ZSA_SUCCEEDED(result))
    {
        result = TRACE_CALL(capturesync_create(&device->capturesync));
    }

    // Create color Module
    if (ZSA_SUCCEEDED(result))
    {
        result = TRACE_CALL(color_create(
            device->tick_handle, container_id, serial_number, color_capture_ready, handle, &device->color));
    }

    if (ZSA_FAILED(result))
    {
        zsa_device_close(handle);
        handle = NULL;
    }
    else
    {
        *device_handle = handle;
    }

    return result;
}

void zsa_device_close(zsa_device_t device_handle)
{
    RETURN_VALUE_IF_HANDLE_INVALID(VOID_VALUE, zsa_device_t, device_handle);
    zsa_context_t *device = zsa_device_t_get_context(device_handle);

    if (device->capturesync)
    {
        // Stop capturesync first so that imu, depth, and color can destroy cleanly
        capturesync_stop(device->capturesync);
    }

    if (device->color)
    {
        color_destroy(device->color);
        device->color = NULL;
    }

    // depth & color call into capturesync, so they need to be destroyed first.
    if (device->capturesync)
    {
        capturesync_destroy(device->capturesync);
        device->capturesync = NULL;
    }

    if (device->colormcu)
    {
        colormcu_destroy(device->colormcu);
        device->colormcu = NULL;
    }

    if (device->tick_handle)
    {
        tickcounter_destroy(device->tick_handle);
        device->tick_handle = NULL;
    }

    zsa_device_t_destroy(device_handle);
    allocator_deinitialize();
}

// zsa_wait_result_t zsa_device_get_capture(zsa_device_t device_handle,
//                                          zsa_capture_t *capture_handle,
//                                          int32_t timeout_in_ms)
// {
//     RETURN_VALUE_IF_HANDLE_INVALID(ZSA_WAIT_RESULT_FAILED, zsa_device_t, device_handle);
//     RETURN_VALUE_IF_ARG(ZSA_WAIT_RESULT_FAILED, capture_handle == NULL);
//     zsa_context_t *device = zsa_device_t_get_context(device_handle);
//     return TRACE_WAIT_CALL(capturesync_get_capture(device->capturesync, capture_handle, timeout_in_ms));
// }

// zsa_image_t zsa_capture_get_color_image(zsa_capture_t capture_handle)
// {
//     return capture_get_color_image(capture_handle);
// }

// zsa_result_t zsa_image_create(zsa_image_format_t format,
//                               int width_pixels,
//                               int height_pixels,
//                               int stride_bytes,
//                               zsa_image_t *image_handle)
// {
//     return image_create(format, width_pixels, height_pixels, stride_bytes, ALLOCATION_SOURCE_USER, image_handle);
// }

static zsa_result_t validate_configuration(zsa_context_t *device, const zsa_device_configuration_t *config)
{
    RETURN_VALUE_IF_ARG(ZSA_RESULT_FAILED, config == NULL);
    RETURN_VALUE_IF_ARG(ZSA_RESULT_FAILED, device == NULL);
    zsa_result_t result = ZSA_RESULT_SUCCEEDED;
    return result;
}

zsa_result_t zsa_device_start_cameras(zsa_device_t device_handle, const zsa_device_configuration_t *config)
{
    RETURN_VALUE_IF_ARG(ZSA_RESULT_FAILED, config == NULL);
    RETURN_VALUE_IF_HANDLE_INVALID(ZSA_RESULT_FAILED, zsa_device_t, device_handle);
    zsa_result_t result = ZSA_RESULT_SUCCEEDED;
    zsa_context_t *device = zsa_device_t_get_context(device_handle);

    LOG_TRACE("zsa_device_start_cameras starting", 0);
    if (device->color_started == true)
    {
        LOG_ERROR("zsa_device_start_cameras called while one of the sensors are running, color:%d",
                  device->color_started);
        result = ZSA_RESULT_FAILED;
    }

    if (ZSA_SUCCEEDED(result))
    {
        LOG_INFO("Starting camera's with the following config.", 0);
        LOG_INFO("    color_format:%d", config->color_format);
        LOG_INFO("    color_resolution:%d", config->color_resolution);
        LOG_INFO("    camera_fps:%d", config->camera_fps);
        LOG_INFO("    synchronized_images_only:%d", config->synchronized_images_only);
        LOG_INFO("    wired_sync_mode:%d", config->wired_sync_mode);
        LOG_INFO("    subordinate_delay_off_master_usec:%d", config->subordinate_delay_off_master_usec);
        LOG_INFO("    disable_streaming_indicator:%d", config->disable_streaming_indicator);
        result = TRACE_CALL(validate_configuration(device, config));
    }

    if (ZSA_SUCCEEDED(result))
    {
        result = TRACE_CALL(colormcu_set_multi_device_mode(device->colormcu, config));
    }

    if (ZSA_SUCCEEDED(result))
    {
        result = TRACE_CALL(capturesync_start(device->capturesync, config));
    }


    if (ZSA_SUCCEEDED(result))
    {
        if (config->color_resolution != ZSA_COLOR_RESOLUTION_OFF)
        {
            // NOTE: Color must be started before depth and IMU as it triggers the sync of PTS. If it starts after
            // depth or IMU, the user will see timestamps reset back to zero when the color camera is started.
            result = TRACE_CALL(color_start(device->color, config));
        }
        if (ZSA_SUCCEEDED(result))
        {
            device->color_started = true;
        }
    }
    LOG_INFO("zsa_device_start_cameras started", 0);

    if (ZSA_FAILED(result))
    {
        zsa_device_stop_cameras(device_handle);
    }

    return result;
}

void zsa_device_stop_cameras(zsa_device_t device_handle)
{
    RETURN_VALUE_IF_HANDLE_INVALID(VOID_VALUE, zsa_device_t, device_handle);
    zsa_context_t *device = zsa_device_t_get_context(device_handle);

    LOG_INFO("zsa_device_stop_cameras stopping", 0);

    // Capturesync needs to stop before color so that all queues will purged
    if (device->capturesync)
    {
        capturesync_stop(device->capturesync);
    }

    if (device->color)
    {
        // This call will block waiting for all outstanding allocations to be released
        color_stop(device->color);
        device->color_started = false;
    }

    LOG_INFO("zsa_device_stop_cameras stopped", 0);
}

// zsa_buffer_result_t zsa_device_get_serialnum(zsa_device_t device_handle,
//                                              char *serial_number,
//                                              size_t *serial_number_size)
// {
//     RETURN_VALUE_IF_HANDLE_INVALID(ZSA_BUFFER_RESULT_FAILED, zsa_device_t, device_handle);
//     zsa_context_t *device = zsa_device_t_get_context(device_handle);

//     return TRACE_BUFFER_CALL(depth_get_device_serialnum(device->depth, serial_number, serial_number_size));
// }

// zsa_result_t zsa_device_get_version(zsa_device_t device_handle, zsa_hardware_version_t *version)
// {
//     RETURN_VALUE_IF_HANDLE_INVALID(ZSA_RESULT_FAILED, zsa_device_t, device_handle);
//     zsa_context_t *device = zsa_device_t_get_context(device_handle);

//     return TRACE_CALL(depth_get_device_version(device->depth, version));
// }


#ifdef __cplusplus
}
#endif
