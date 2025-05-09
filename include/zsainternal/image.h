/** \file image.h
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 * Kinect For Azure SDK.
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <zsa/zsatypes.h>
#include <zsainternal/handle.h>
#include <zsainternal/allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void(image_destroy_cb_t)(void *buffer, void *context);

/** Create a handle to an image object.
 *
 * \param format [IN]
 * format of the image being created.
 *
 * \param width_pixels [IN]
 * width of the image being created
 *
 * \param height_pixels [IN]
 * height of the image being created
 *
 * \param stride_bytes [IN]
 * stride of the image being created
 *
 * \param source
 * source of the image for allocation accounting
 *
 * \return NULL if failed, otherwise a zsa_image_t handle
 *
 * If successful, \ref image_create will return an image_t handle. This function will allocate a function of size
 * height_pixels * stride_bytes. Use /r image_create_from_buffer for advanced configurations.
 *
 * When done with the device, close the handle with \ref image_release
 */
zsa_result_t image_create(zsa_image_format_t format,
                          int width_pixels,
                          int height_pixels,
                          int stride_bytes,
                          allocation_source_t source,
                          zsa_image_t *image);

/** Create a handle to an image object.
 * internal function to allocate an image object and memory blob of 'size'. Used for USB layer where we need counted
 * objects and don't know anything about the image. Also wrapped by IMU but never exposed.
 */
zsa_result_t image_create_empty_internal(allocation_source_t source, size_t size, zsa_image_t *image);

/** Create a handle to an image object.
 * \param format [IN]
 * format of the image being created.
 *
 * \param width_pixels [IN]
 * width of the image being created
 *
 * \param height_pixels [IN]
 * height of the image being created
 *
 * \param stride_bytes [IN]
 * stride of the image being created
 *
 * \param buffer [IN]
 * pre allocated buffer to use in the image_t
 *
 * \param buffer_size [IN]
 * size in bytes of the pre allocated buffer
 *
 * \param buffer_destroy_cb [IN]
 * buffer destroy function to be called when the ref count on zsa_image_t reaches zero
 *
 * \param buffer_destroy_cb_context [IN]
 * buffer destroy function to be called when the ref count on zsa_image_t reaches zero
 *
 * \return NULL if failed, otherwise a zsa_image_t handle
 *
 * If successful, \ref image_create will return an image_t handle. If this function failes, the user must free the
 * buffer. If the function succeeds the buffer_destroy_cb function will be called to free the memory when all references
 * to it are released.
 *
 * When done with the device, close the handle with \ref image_release
 */
zsa_result_t image_create_from_buffer(zsa_image_format_t format,
                                      int width_pixels,
                                      int height_pixels,
                                      int stride_bytes,
                                      uint8_t *buffer,
                                      size_t buffer_size,
                                      image_destroy_cb_t *buffer_destroy_cb,
                                      void *buffer_destroy_cb_context,
                                      zsa_image_t *image_handle);

/** Removes one reference on image_t, free's when it hits zero
 *
 * \param image_handle [IN]
 * Handle to the image to remove the reference from
 * */
void image_dec_ref(zsa_image_t image_handle);

/** Removes one reference on image_t, free's when it hits zero
 *
 * \param image_handle [IN]
 * Handle to the image to remove the reference from
 * */
void image_inc_ref(zsa_image_t image_handle);

uint8_t *image_get_buffer(zsa_image_t image_handle);
size_t image_get_size(zsa_image_t image_handle);
void image_set_size(zsa_image_t image_handle, size_t size);
zsa_image_format_t image_get_format(zsa_image_t image_handle);
int image_get_width_pixels(zsa_image_t image_handle);
int image_get_height_pixels(zsa_image_t image_handle);
int image_get_stride_bytes(zsa_image_t image_handle);
uint64_t image_get_device_timestamp_usec(zsa_image_t image_handle);
uint64_t image_get_system_timestamp_nsec(zsa_image_t image_handle);
uint64_t image_get_exposure_usec(zsa_image_t image_handle);
uint32_t image_get_white_balance(zsa_image_t image_handle);
uint32_t image_get_iso_speed(zsa_image_t image_handle);
void image_set_device_timestamp_usec(zsa_image_t image_handle, uint64_t timestamp_usec);
void image_set_system_timestamp_nsec(zsa_image_t image_handle, uint64_t timestamp_nsec);
zsa_result_t image_apply_system_timestamp(zsa_image_t image_handle);
void image_set_exposure_usec(zsa_image_t image_handle, uint64_t exposure_usec);
void image_set_white_balance(zsa_image_t image_handle, uint32_t white_balance);
void image_set_iso_speed(zsa_image_t image_handle, uint32_t iso_speed);

#ifdef __cplusplus
}
#endif

#endif /* IMAGE_H */
