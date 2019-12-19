/** \file transformation.h
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 * Kinect For Azure SDK.
 */

#ifndef ZSATRANSFORMATION_H
#define ZSATRANSFORMATION_H

#include <zsa/zsatypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _zsa_camera_calibration_mode_info_t
{
    unsigned int calibration_image_binned_resolution[2];
    int crop_offset[2];
    unsigned int output_image_resolution[2];
} zsa_camera_calibration_mode_info_t;

typedef struct _zsa_transformation_image_descriptor_t
{
    int width_pixels;          // image width in pixels
    int height_pixels;         // image height in pixels
    int stride_bytes;          // image stride in bytes
    zsa_image_format_t format; // image format
} zsa_transformation_image_descriptor_t;

typedef struct _zsa_transformation_xy_tables_t
{
    float *x_table; // table used to compute X coordinate
    float *y_table; // table used to compute Y coordinate
    int width;      // width of x and y tables
    int height;     // height of x and y tables
} zsa_transformation_xy_tables_t;

typedef struct _zsa_transformation_pinhole_t
{
    float px;
    float py;
    float fx;
    float fy;
    int width;
    int height;
} zsa_transformation_pinhole_t;

typedef struct _zsa_transform_engine_calibration_t
{
    zsa_calibration_camera_t depth_camera_calibration;                    // depth camera calibration
    zsa_calibration_camera_t color_camera_calibration;                    // color camera calibration
    zsa_calibration_extrinsics_t depth_camera_to_color_camera_extrinsics; // depth to color extrinsics
    zsa_calibration_extrinsics_t color_camera_to_depth_camera_extrinsics; // color to depth extrinsics
    zsa_transformation_xy_tables_t depth_camera_xy_tables;                // depth camera xy tables
} zsa_transform_engine_calibration_t;

zsa_result_t transformation_get_mode_specific_calibration(const zsa_calibration_camera_t *depth_camera_calibration,
                                                          const zsa_calibration_camera_t *color_camera_calibration,
                                                          const zsa_calibration_extrinsics_t *gyro_extrinsics,
                                                          const zsa_calibration_extrinsics_t *accel_extrinsics,
                                                          const zsa_depth_mode_t depth_mode,
                                                          const zsa_color_resolution_t color_resolution,
                                                          zsa_calibration_t *calibration);

zsa_result_t transformation_3d_to_3d(const zsa_calibration_t *calibration,
                                     const float source_point3d[3],
                                     const zsa_calibration_type_t source_camera,
                                     const zsa_calibration_type_t target_camera,
                                     float target_point3d[3]);

zsa_result_t transformation_2d_to_3d(const zsa_calibration_t *calibration,
                                     const float source_point2d[2],
                                     const float source_depth,
                                     const zsa_calibration_type_t source_camera,
                                     const zsa_calibration_type_t target_camera,
                                     float target_point3d[3],
                                     int *valid);

zsa_result_t transformation_3d_to_2d(const zsa_calibration_t *calibration,
                                     const float source_point3d[3],
                                     const zsa_calibration_type_t source_camera,
                                     const zsa_calibration_type_t target_camera,
                                     float target_point2d[2],
                                     int *valid);

zsa_result_t transformation_2d_to_2d(const zsa_calibration_t *calibration,
                                     const float source_point2d[2],
                                     const float source_depth,
                                     const zsa_calibration_type_t source_camera,
                                     const zsa_calibration_type_t target_camera,
                                     float target_point2d[2],
                                     int *valid);

zsa_result_t transformation_color_2d_to_depth_2d(const zsa_calibration_t *calibration,
                                                 const float source_point2d[2],
                                                 const zsa_image_t depth_image,
                                                 float target_point2d[2],
                                                 int *valid);

zsa_transformation_t transformation_create(const zsa_calibration_t *calibration, bool gpu_optimization);

void transformation_destroy(zsa_transformation_t transformation_handle);

zsa_buffer_result_t transformation_depth_image_to_color_camera_validate_parameters(
    const zsa_calibration_t *calibration,
    const zsa_transformation_xy_tables_t *xy_tables_depth_camera,
    const uint8_t *depth_image_data,
    const zsa_transformation_image_descriptor_t *depth_image_descriptor,
    const uint8_t *custom_image_data,
    const zsa_transformation_image_descriptor_t *custom_image_descriptor,
    uint8_t *transformed_depth_image_data,
    zsa_transformation_image_descriptor_t *transformed_depth_image_descriptor,
    uint8_t *transformed_custom_image_data,
    zsa_transformation_image_descriptor_t *transformed_custom_image_descriptor);

zsa_buffer_result_t transformation_depth_image_to_color_camera_internal(
    const zsa_calibration_t *calibration,
    const zsa_transformation_xy_tables_t *xy_tables_depth_camera,
    const uint8_t *depth_image_data,
    const zsa_transformation_image_descriptor_t *depth_image_descriptor,
    const uint8_t *custom_image_data,
    const zsa_transformation_image_descriptor_t *custom_image_descriptor,
    uint8_t *transformed_depth_image_data,
    zsa_transformation_image_descriptor_t *transformed_depth_image_descriptor,
    uint8_t *transformed_custom_image_data,
    zsa_transformation_image_descriptor_t *transformed_custom_image_descriptor,
    zsa_transformation_interpolation_type_t interpolation_type,
    uint32_t invalid_custom_value);

zsa_result_t transformation_depth_image_to_color_camera_custom(
    zsa_transformation_t transformation_handle,
    const uint8_t *depth_image_data,
    const zsa_transformation_image_descriptor_t *depth_image_descriptor,
    const uint8_t *custom_image_data,
    const zsa_transformation_image_descriptor_t *custom_image_descriptor,
    uint8_t *transformed_depth_image_data,
    zsa_transformation_image_descriptor_t *transformed_depth_image_descriptor,
    uint8_t *transformed_custom_image_data,
    zsa_transformation_image_descriptor_t *transformed_custom_image_descriptor,
    zsa_transformation_interpolation_type_t interpolation_type,
    uint32_t invalid_custom_value);

zsa_buffer_result_t transformation_color_image_to_depth_camera_validate_parameters(
    const zsa_calibration_t *calibration,
    const zsa_transformation_xy_tables_t *xy_tables_depth_camera,
    const uint8_t *depth_image_data,
    const zsa_transformation_image_descriptor_t *depth_image_descriptor,
    const uint8_t *color_image_data,
    const zsa_transformation_image_descriptor_t *color_image_descriptor,
    uint8_t *transformed_color_image_data,
    zsa_transformation_image_descriptor_t *transformed_color_image_descriptor);

zsa_buffer_result_t transformation_color_image_to_depth_camera_internal(
    const zsa_calibration_t *calibration,
    const zsa_transformation_xy_tables_t *xy_tables_depth_camera,
    const uint8_t *depth_image_data,
    const zsa_transformation_image_descriptor_t *depth_image_descriptor,
    const uint8_t *color_image_data,
    const zsa_transformation_image_descriptor_t *color_image_descriptor,
    uint8_t *transformed_color_image_data,
    zsa_transformation_image_descriptor_t *transformed_color_image_descriptor);

zsa_result_t
transformation_color_image_to_depth_camera(zsa_transformation_t transformation_handle,
                                           const uint8_t *depth_image_data,
                                           const zsa_transformation_image_descriptor_t *depth_image_descriptor,
                                           const uint8_t *color_image_data,
                                           const zsa_transformation_image_descriptor_t *color_image_descriptor,
                                           uint8_t *transformed_color_image_data,
                                           zsa_transformation_image_descriptor_t *transformed_color_image_descriptor);

zsa_buffer_result_t
transformation_depth_image_to_point_cloud_internal(zsa_transformation_xy_tables_t *xy_tables,
                                                   const uint8_t *depth_image_data,
                                                   const zsa_transformation_image_descriptor_t *depth_image_descriptor,
                                                   uint8_t *xyz_image_data,
                                                   zsa_transformation_image_descriptor_t *xyz_image_descriptor);

zsa_result_t
transformation_depth_image_to_point_cloud(zsa_transformation_t transformation_handle,
                                          const uint8_t *depth_image_data,
                                          const zsa_transformation_image_descriptor_t *depth_image_descriptor,
                                          const zsa_calibration_type_t camera,
                                          uint8_t *xyz_image_data,
                                          zsa_transformation_image_descriptor_t *xyz_image_descriptor);

// Mode specific calibration
zsa_result_t
transformation_get_mode_specific_depth_camera_calibration(const zsa_calibration_camera_t *raw_camera_calibration,
                                                          const zsa_depth_mode_t depth_mode,
                                                          zsa_calibration_camera_t *mode_specific_camera_calibration);

zsa_result_t
transformation_get_mode_specific_color_camera_calibration(const zsa_calibration_camera_t *raw_camera_calibration,
                                                          const zsa_color_resolution_t color_resolution,
                                                          zsa_calibration_camera_t *mode_specific_camera_calibration);

zsa_result_t
transformation_get_mode_specific_camera_calibration(const zsa_calibration_camera_t *raw_camera_calibration,
                                                    const zsa_camera_calibration_mode_info_t *mode_info,
                                                    zsa_calibration_camera_t *mode_specific_camera_calibration,
                                                    bool pixelized_zero_centered_output);

// Intrinsic transformations
zsa_result_t transformation_unproject(const zsa_calibration_camera_t *camera_calibration,
                                      const float point2d[2],
                                      const float depth,
                                      float point3d[3],
                                      int *valid);

zsa_result_t transformation_project(const zsa_calibration_camera_t *camera_calibration,
                                    const float point3d[3],
                                    float point2d[2],
                                    int *valid);

// Extrinsic transformations
zsa_result_t transformation_get_extrinsic_transformation(const zsa_calibration_extrinsics_t *source_camera_calibration,
                                                         const zsa_calibration_extrinsics_t *target_camera_calibration,
                                                         zsa_calibration_extrinsics_t *source_to_target);

zsa_result_t transformation_apply_extrinsic_transformation(const zsa_calibration_extrinsics_t *source_to_target,
                                                           const float source_point3d[3],
                                                           float target_point3d[3]);

#ifdef __cplusplus
}
#endif

#endif /* ZSATRANSFORMATION_H */
