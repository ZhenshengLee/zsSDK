/** \file capture.h
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 * Kinect For Azure SDK.
 */

#ifndef CAPTURE_H
#define CAPTURE_H

#include <zsa/zsatypes.h>
#include <zsainternal/image.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Create a reference counted zsa_capture_t for tracking synchronized images.
 *
 * \param capture_handle
 * The capture handle
 *
 * /ref zsa_capture_t object this API creates is with a base ref count of 1. It will automatically be deleted when
 * its ref count reaches zero. Any zsa_image_t's associated with this capture will also have a reference removed when
 * this object is destroyed.
 */
zsa_result_t capture_create(zsa_capture_t *capture_handle);

/** Increase the ref count on \ref zsa_capture_t blob
 *
 * \param capture_handle
 * The zsa_capture_t blob
 *
 * /ref zsa_capture_t object ref count is increased by one
 */
void capture_inc_ref(zsa_capture_t capture_handle);

/** Decrease the ref count on \ref zsa_capture_t blob
 *
 * \param capture_handle
 * The zsa_capture_t blob
 *
 * /ref zsa_capture_t object ref count is increased by one
 */
void capture_dec_ref(zsa_capture_t capture_handle);

zsa_image_t capture_get_color_image(zsa_capture_t capture_handle);
zsa_image_t capture_get_depth_image(zsa_capture_t capture_handle);
zsa_image_t capture_get_imu_image(zsa_capture_t capture_handle);
zsa_image_t capture_get_ir_image(zsa_capture_t capture_handle);
void capture_set_color_image(zsa_capture_t capture_handle, zsa_image_t image_handle);
void capture_set_depth_image(zsa_capture_t capture_handle, zsa_image_t image_handle);
void capture_set_imu_image(zsa_capture_t capture_handle, zsa_image_t image_handle);
void capture_set_ir_image(zsa_capture_t capture_handle, zsa_image_t image_handle);
void capture_set_temperature_c(zsa_capture_t capture_handle, float temperature_c);
float capture_get_temperature_c(zsa_capture_t capture_handle);

#ifdef __cplusplus
}
#endif

#endif /* CAPTURE_H */
