/** \file zsa.hpp
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 * Kinect For Azure SDK - C++ wrapper.
 */

#ifndef ZSA_HPP
#define ZSA_HPP

#include "zsa.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace zsa
{

/**
 * \defgroup cppsdk C++ Reference
 *
 * Functions part of the SDK.
 *
 * @{
 */

/** Exception type thrown when a ZSA API call fails
 */
class error : public std::runtime_error
{
public:
    using runtime_error::runtime_error;
};

// Helper functions not intended for use by client code
//
namespace internal
{
/// @cond FALSE
/** Casts an arithmetic value, clamping to the supported range of the type being cast to
 *
 * \returns input represented in output_type
 *
 * \remarks
 * Clamps to 0 for unsigned types
 */
template<typename output_type, typename input_type> output_type clamp_cast(input_type input)
{
    static_assert(std::is_arithmetic<input_type>::value, "clamp_cast only supports arithmetic types");
    static_assert(std::is_arithmetic<output_type>::value, "clamp_cast only supports arithmetic types");
    const input_type min_value = std::is_signed<input_type>() ?
                                     static_cast<input_type>(std::numeric_limits<output_type>::min()) :
                                     0;

    input_type max_value = static_cast<input_type>(std::numeric_limits<output_type>::max());
    if (max_value < 0)
    {
        // Output type is of greater or equal size to input type and we've overflowed.
        //
        max_value = std::numeric_limits<input_type>::max();
    }
    input = std::min(input, max_value);
    input = std::max(input, min_value);
    return static_cast<output_type>(input);
}
/// @endcond
} // namespace internal

/** \class image zsa.hpp <zsa/zsa.hpp>
 * Wrapper for \ref zsa_image_t
 *
 * Wraps a handle for an image.  Copying/moving is cheap, copies are shallow.
 *
 * \sa zsa_image_t
 */
class image
{
public:
    /** Creates an image from a zsa_image_t.
     * Takes ownership of the handle, i.e. assuming that handle has a refcount
     * of 1, you should not call zsa_image_release on the handle after giving
     * it to the image; the image will take care of that.
     */
    image(zsa_image_t handle = nullptr) noexcept : m_handle(handle) {}

    /** Creates a shallow copy of another image
     */
    image(const image &other) noexcept : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
            zsa_image_reference(m_handle);
        }
    }

    /** Moves another image into a new image
     */
    image(image &&other) noexcept : m_handle(other.m_handle)
    {
        other.m_handle = nullptr;
    }

    ~image()
    {
        reset();
    }

    /** Sets image to a shallow copy of other
     */
    image &operator=(const image &other) noexcept
    {
        if (this != &other)
        {
            reset();
            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
                zsa_image_reference(m_handle);
            }
        }
        return *this;
    }

    /** Moves another image into this image; other is set to invalid
     */
    image &operator=(image &&other) noexcept
    {
        if (this != &other)
        {
            reset();
            m_handle = other.m_handle;
            other.m_handle = nullptr;
        }
        return *this;
    }

    /** Invalidates this image
     */
    image &operator=(std::nullptr_t) noexcept
    {
        reset();
        return *this;
    }

    /** Returns true if two images refer to the same zsa_image_t, false otherwise
     */
    bool operator==(const image &other) const noexcept
    {
        return m_handle == other.m_handle;
    }

    /** Returns false if the image is valid, true otherwise
     */
    bool operator==(std::nullptr_t) const noexcept
    {
        return m_handle == nullptr;
    }

    /** Returns true if two images wrap different zsa_image_t instances, false otherwise
     */
    bool operator!=(const image &other) const noexcept
    {
        return m_handle != other.m_handle;
    }

    /** Returns true if the image is valid, false otherwise
     */
    bool operator!=(std::nullptr_t) const noexcept
    {
        return m_handle != nullptr;
    }

    /** Returns true if the image is valid, false otherwise
     */
    operator bool() const noexcept
    {
        return m_handle != nullptr;
    }

    /** Returns the underlying zsa_image_t handle
     *
     * Note that this function does not increment the reference count on the zsa_image_t.
     * The caller is responsible for incrementing the reference count on
     * the zsa_image_t if the caller needs the zsa_image_t to outlive this C++ object.
     * Otherwise, the zsa_image_t will be destroyed by this C++ object.
     */
    zsa_image_t handle() const noexcept
    {
        return m_handle;
    }

    /** Releases the underlying zsa_image_t; the image is set to invalid.
     */
    void reset() noexcept
    {
        if (m_handle != nullptr)
        {
            zsa_image_release(m_handle);
            m_handle = nullptr;
        }
    }

    /** Create a blank image
     * Throws error on failure
     *
     * \sa zsa_image_create
     */
    static image create(zsa_image_format_t format, int width_pixels, int height_pixels, int stride_bytes)
    {
        zsa_image_t handle = nullptr;
        zsa_result_t result = zsa_image_create(format, width_pixels, height_pixels, stride_bytes, &handle);
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to create image!");
        }
        return image(handle);
    }

    /** Create an image from a pre-allocated buffer
     * Throws error on failure
     *
     * \sa zsa_image_create_from_buffer
     */
    static image create_from_buffer(zsa_image_format_t format,
                                    int width_pixels,
                                    int height_pixels,
                                    int stride_bytes,
                                    uint8_t *buffer,
                                    size_t buffer_size,
                                    zsa_memory_destroy_cb_t *buffer_release_cb,
                                    void *buffer_release_cb_context)
    {
        zsa_image_t handle = nullptr;
        zsa_result_t result = zsa_image_create_from_buffer(format,
                                                           width_pixels,
                                                           height_pixels,
                                                           stride_bytes,
                                                           buffer,
                                                           buffer_size,
                                                           buffer_release_cb,
                                                           buffer_release_cb_context,
                                                           &handle);
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to create image from buffer");
        }
        return image(handle);
    }

    /** Get the image buffer
     *
     * \sa zsa_image_get_buffer
     */
    uint8_t *get_buffer() noexcept
    {
        return zsa_image_get_buffer(m_handle);
    }

    /** Get the image buffer
     *
     * \sa zsa_image_get_buffer
     */
    const uint8_t *get_buffer() const noexcept
    {
        return zsa_image_get_buffer(m_handle);
    }

    /** Get the image buffer size in bytes
     *
     * \sa zsa_image_get_size
     */
    size_t get_size() const noexcept
    {
        return zsa_image_get_size(m_handle);
    }

    /** Get the image format of the image
     *
     * \sa zsa_image_get_format
     */
    zsa_image_format_t get_format() const noexcept
    {
        return zsa_image_get_format(m_handle);
    }

    /** Get the image width in pixels
     *
     * \sa zsa_image_get_width_pixels
     */
    int get_width_pixels() const noexcept
    {
        return zsa_image_get_width_pixels(m_handle);
    }

    /** Get the image height in pixels
     *
     * \sa zsa_image_get_height_pixels
     */
    int get_height_pixels() const noexcept
    {
        return zsa_image_get_height_pixels(m_handle);
    }

    /** Get the image stride in bytes
     *
     * \sa zsa_image_get_stride_bytes
     */
    int get_stride_bytes() const noexcept
    {
        return zsa_image_get_stride_bytes(m_handle);
    }

    /** Get the image's device timestamp in microseconds
     *
     * \sa zsa_image_get_device_timestamp_usec
     */
    std::chrono::microseconds get_device_timestamp() const noexcept
    {
        return std::chrono::microseconds(zsa_image_get_device_timestamp_usec(m_handle));
    }

    /** Get the image's system timestamp in nanoseconds
     *
     * \sa zsa_image_get_system_timestamp_nsec
     */
    std::chrono::nanoseconds get_system_timestamp() const noexcept
    {
        return std::chrono::nanoseconds(zsa_image_get_system_timestamp_nsec(m_handle));
    }

    /** Get the image exposure time in microseconds
     *
     * \sa zsa_image_get_exposure_usec
     */
    std::chrono::microseconds get_exposure() const noexcept
    {
        return std::chrono::microseconds(zsa_image_get_exposure_usec(m_handle));
    }

    /** Get the image white balance in Kelvin (color images only)
     *
     * \sa zsa_image_get_white_balance
     */
    uint32_t get_white_balance() const noexcept
    {
        return zsa_image_get_white_balance(m_handle);
    }

    /** Get the image's ISO speed (color images only)
     *
     * \sa zsa_image_get_white_balance
     */
    uint32_t get_iso_speed() const noexcept
    {
        return zsa_image_get_iso_speed(m_handle);
    }

    /** Set the image's timestamp in microseconds
     *
     * \sa zsa_image_set_device_timestamp_usec
     */
    void set_timestamp(std::chrono::microseconds timestamp) noexcept
    {
        zsa_image_set_device_timestamp_usec(m_handle, internal::clamp_cast<uint64_t>(timestamp.count()));
    }

    /** Set the image's exposure time in microseconds (color images only)
     *
     * \sa zsa_image_set_exposure_time_usec
     */
    void set_exposure_time(std::chrono::microseconds exposure) noexcept
    {
        zsa_image_set_exposure_usec(m_handle, internal::clamp_cast<uint64_t>(exposure.count()));
    }

    /** Set the white balance of the image (color images only)
     *
     * \sa zsa_image_set_white_balance
     */
    void set_white_balance(uint32_t white_balance) noexcept
    {
        zsa_image_set_white_balance(m_handle, white_balance);
    }

    /** Set the ISO speed of the image (color images only)
     *
     * \sa zsa_image_set_iso_speed
     */
    void set_iso_speed(uint32_t iso_speed) noexcept
    {
        zsa_image_set_iso_speed(m_handle, iso_speed);
    }

private:
    zsa_image_t m_handle;
};

/** \class capture zsa.hpp <zsa/zsa.hpp>
 * Wrapper for \ref zsa_capture_t
 *
 * Wraps a handle for a capture.  Copying/moving is cheap, copies are shallow.
 */
class capture
{
public:
    /** Creates a capture from a zsa_capture_t
     * Takes ownership of the handle, i.e. assuming that handle has a refcount
     * of 1, you should not call zsa_capture_release on the handle after giving
     * it to the capture; the capture will take care of that.
     */
    capture(zsa_capture_t handle = nullptr) noexcept : m_handle(handle) {}

    /** Creates a shallow copy of another capture
     */
    capture(const capture &other) noexcept : m_handle(other.m_handle)
    {
        if (m_handle != nullptr)
        {
            zsa_capture_reference(m_handle);
        }
    }

    /** Moves another capture into a new capture
     */
    capture(capture &&other) noexcept : m_handle(other.m_handle)
    {
        other.m_handle = nullptr;
    }

    ~capture()
    {
        reset();
    }

    /** Sets capture to a shallow copy of other
     */
    capture &operator=(const capture &other) noexcept
    {
        if (this != &other)
        {
            reset();
            m_handle = other.m_handle;
            if (m_handle != nullptr)
            {
                zsa_capture_reference(m_handle);
            }
        }
        return *this;
    }

    /** Moves another capture into this capture; other is set to invalid
     */
    capture &operator=(capture &&other) noexcept
    {
        if (this != &other)
        {
            reset();
            m_handle = other.m_handle;
            other.m_handle = nullptr;
        }
        return *this;
    }

    /** Invalidates this capture
     */
    capture &operator=(std::nullptr_t) noexcept
    {
        reset();
        return *this;
    }

    /** Returns true if two captures refer to the same zsa_capture_t, false otherwise
     */
    bool operator==(const capture &other) const noexcept
    {
        return m_handle == other.m_handle;
    }

    /** Returns false if the capture is valid, true otherwise
     */
    bool operator==(std::nullptr_t) const noexcept
    {
        return m_handle == nullptr;
    }

    /** Returns true if two captures wrap different zsa_capture_t instances, false otherwise
     */
    bool operator!=(const capture &other) const noexcept
    {
        return m_handle != other.m_handle;
    }

    /** Returns true if the capture is valid, false otherwise
     */
    bool operator!=(std::nullptr_t) const noexcept
    {
        return m_handle != nullptr;
    }

    /** Returns true if the capture is valid, false otherwise
     */
    operator bool() const noexcept
    {
        return m_handle != nullptr;
    }

    /** Returns the underlying zsa_capture_t handle
     *
     * Note that this function does not increment the reference count on the zsa_capture_t.
     * The caller is responsible for incrementing the reference count on
     * the zsa_capture_t if the caller needs the zsa_capture_t to outlive this C++ object.
     * Otherwise, the zsa_capture_t will be destroyed by this C++ object.
     */
    zsa_capture_t handle() const noexcept
    {
        return m_handle;
    }

    /** Releases the underlying zsa_capture_t; the capture is set to invalid.
     */
    void reset() noexcept
    {
        if (m_handle != nullptr)
        {
            zsa_capture_release(m_handle);
            m_handle = nullptr;
        }
    }

    /** Get the color image associated with the capture
     *
     * \sa zsa_capture_get_color_image
     */
    image get_color_image() const noexcept
    {
        return image(zsa_capture_get_color_image(m_handle));
    }

    /** Get the depth image associated with the capture
     *
     * \sa zsa_capture_get_depth_image
     */
    image get_depth_image() const noexcept
    {
        return image(zsa_capture_get_depth_image(m_handle));
    }

    /** Get the IR image associated with the capture
     *
     * \sa zsa_capture_get_ir_image
     */
    image get_ir_image() const noexcept
    {
        return image(zsa_capture_get_ir_image(m_handle));
    }

    /** Set / add a color image to the capture
     *
     * \sa zsa_capture_set_color_image
     */
    void set_color_image(const image &color_image) noexcept
    {
        zsa_capture_set_color_image(m_handle, color_image.handle());
    }

    /** Set / add a depth image to the capture
     *
     * \sa zsa_capture_set_depth_image
     */
    void set_depth_image(const image &depth_image) noexcept
    {
        zsa_capture_set_depth_image(m_handle, depth_image.handle());
    }

    /** Set / add an IR image to the capture
     *
     * \sa zsa_capture_set_ir_image
     */
    void set_ir_image(const image &ir_image) noexcept
    {
        zsa_capture_set_ir_image(m_handle, ir_image.handle());
    }

    /** Set the temperature associated with the capture in Celsius.
     *
     * \sa zsa_capture_set_temperature_c
     */
    void set_temperature_c(float temperature_c) noexcept
    {
        zsa_capture_set_temperature_c(m_handle, temperature_c);
    }

    /** Get temperature (in Celsius) associated with the capture.
     *
     * \sa zsa_capture_get_temperature_c
     */
    float get_temperature_c() const noexcept
    {
        return zsa_capture_get_temperature_c(m_handle);
    }

    /** Create an empty capture object.
     * Throws error on failure.
     *
     * \sa zsa_capture_create
     */
    static capture create()
    {
        zsa_capture_t handle = nullptr;
        zsa_result_t result = zsa_capture_create(&handle);
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to create capture!");
        }
        return capture(handle);
    }

private:
    zsa_capture_t m_handle;
};

/** \class calibration zsa.hpp <zsa/zsa.hpp>
 * Wrapper for \ref zsa_calibration_t
 *
 * Provides member functions for zsa_calibration_t.
 */
struct calibration : public zsa_calibration_t
{
    /** Transform a 3d point of a source coordinate system into a 3d point of the target coordinate system.
     * Throws error on failure.
     *
     * \sa zsa_calibration_3d_to_3d
     */
    zsa_float3_t convert_3d_to_3d(const zsa_float3_t &source_point3d,
                                  zsa_calibration_type_t source_camera,
                                  zsa_calibration_type_t target_camera) const
    {
        zsa_float3_t target_point3d;
        zsa_result_t result =
            zsa_calibration_3d_to_3d(this, &source_point3d, source_camera, target_camera, &target_point3d);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Calibration contained invalid transformation parameters!");
        }
        return target_point3d;
    }

    /** Transform a 2d pixel coordinate with an associated depth value of the source camera into a 3d point of the
     * target coordinate system.
     * Returns false if the point is invalid in the target coordinate system (and therefore target_point3d should not be
     * used)
     * Throws error if calibration contains invalid data.
     *
     * \sa zsa_calibration_2d_to_3d
     */
    bool convert_2d_to_3d(const zsa_float2_t &source_point2d,
                          float source_depth,
                          zsa_calibration_type_t source_camera,
                          zsa_calibration_type_t target_camera,
                          zsa_float3_t *target_point3d) const
    {
        int valid = 0;
        zsa_result_t result = zsa_calibration_2d_to_3d(
            this, &source_point2d, source_depth, source_camera, target_camera, target_point3d, &valid);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Calibration contained invalid transformation parameters!");
        }
        return static_cast<bool>(valid);
    }

    /** Transform a 3d point of a source coordinate system into a 2d pixel coordinate of the target camera.
     * Returns false if the point is invalid in the target coordinate system (and therefore target_point2d should not be
     * used)
     * Throws error if calibration contains invalid data.
     *
     * \sa zsa_calibration_3d_to_2d
     */
    bool convert_3d_to_2d(const zsa_float3_t &source_point3d,
                          zsa_calibration_type_t source_camera,
                          zsa_calibration_type_t target_camera,
                          zsa_float2_t *target_point2d) const
    {
        int valid = 0;
        zsa_result_t result =
            zsa_calibration_3d_to_2d(this, &source_point3d, source_camera, target_camera, target_point2d, &valid);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Calibration contained invalid transformation parameters!");
        }
        return static_cast<bool>(valid);
    }

    /** Transform a 2d pixel coordinate with an associated depth value of the source camera into a 2d pixel coordinate
     * of the target camera
     * Returns false if the point is invalid in the target coordinate system (and therefore target_point2d should not be
     * used)
     * Throws error if calibration contains invalid data.
     *
     * \sa zsa_calibration_2d_to_2d
     */
    bool convert_2d_to_2d(const zsa_float2_t &source_point2d,
                          float source_depth,
                          zsa_calibration_type_t source_camera,
                          zsa_calibration_type_t target_camera,
                          zsa_float2_t *target_point2d) const
    {
        int valid = 0;
        zsa_result_t result = zsa_calibration_2d_to_2d(
            this, &source_point2d, source_depth, source_camera, target_camera, target_point2d, &valid);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Calibration contained invalid transformation parameters!");
        }
        return static_cast<bool>(valid);
    }

    /** Transform a 2D pixel coordinate from color camera into a 2D pixel coordinate of the depth camera. This function
     * searches along an epipolar line in the depth image to find the corresponding depth pixel.
     * Returns false if the point is invalid in the target coordinate system (and therefore target_point2d should not be
     * used) Throws error if calibration contains invalid data.
     *
     * \sa zsa_calibration_color_2d_to_depth_2d
     */
    bool convert_color_2d_to_depth_2d(const zsa_float2_t &source_point2d,
                                      const image &depth_image,
                                      zsa_float2_t *target_point2d) const
    {
        int valid = 0;
        zsa_result_t result =
            zsa_calibration_color_2d_to_depth_2d(this, &source_point2d, depth_image.handle(), target_point2d, &valid);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Calibration contained invalid transformation parameters!");
        }
        return static_cast<bool>(valid);
    }

    /** Get the camera calibration for a device from a raw calibration blob.
     * Throws error on failure.
     *
     * \sa zsa_calibration_get_from_raw
     */
    static calibration get_from_raw(char *raw_calibration,
                                    size_t raw_calibration_size,
                                    zsa_depth_mode_t target_depth_mode,
                                    zsa_color_resolution_t target_color_resolution)
    {
        calibration calib;
        zsa_result_t result = zsa_calibration_get_from_raw(raw_calibration,
                                                           raw_calibration_size,
                                                           target_depth_mode,
                                                           target_color_resolution,
                                                           &calib);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to load calibration from raw calibration blob!");
        }
        return calib;
    }

    /** Get the camera calibration for a device from a raw calibration blob.
     * Throws error on failure.
     *
     * \sa zsa_calibration_get_from_raw
     */
    static calibration get_from_raw(uint8_t *raw_calibration,
                                    size_t raw_calibration_size,
                                    zsa_depth_mode_t target_depth_mode,
                                    zsa_color_resolution_t target_color_resolution)
    {
        return get_from_raw(reinterpret_cast<char *>(raw_calibration),
                            raw_calibration_size,
                            target_depth_mode,
                            target_color_resolution);
    }

    /** Get the camera calibration for a device from a raw calibration blob.
     * Throws error on failure.
     *
     * \sa zsa_calibration_get_from_raw
     */
    static calibration get_from_raw(std::vector<uint8_t> &raw_calibration,
                                    zsa_depth_mode_t target_depth_mode,
                                    zsa_color_resolution_t target_color_resolution)
    {
        return get_from_raw(reinterpret_cast<char *>(raw_calibration.data()),
                            raw_calibration.size(),
                            target_depth_mode,
                            target_color_resolution);
    }
};

/** \class transformation zsa.hpp <zsa/zsa.hpp>
 * Wrapper for \ref zsa_transformation_t
 *
 * Wraps a handle for a transformation.
 */
class transformation
{
public:
    /** Creates a transformation associated with calibration
     *
     * \sa zsa_transformation_create
     */
    transformation(const zsa_calibration_t &calibration) noexcept :
        m_handle(zsa_transformation_create(&calibration)),
        m_color_resolution({ calibration.color_camera_calibration.resolution_width,
                             calibration.color_camera_calibration.resolution_height }),
        m_depth_resolution({ calibration.depth_camera_calibration.resolution_width,
                             calibration.depth_camera_calibration.resolution_height })
    {
    }

    /** Creates a transformation from a zsa_transformation_t
     * Takes ownership of the handle, i.e. you should not call
     * zsa_transformation_destroy on the handle after giving
     * it to the transformation; the transformation will take care of that.
     */
    transformation(zsa_transformation_t handle = nullptr) noexcept : m_handle(handle) {}

    /** Moves another tranformation into a new transformation
     */
    transformation(transformation &&other) noexcept :
        m_handle(other.m_handle),
        m_color_resolution(other.m_color_resolution),
        m_depth_resolution(other.m_depth_resolution)
    {
        other.m_handle = nullptr;
    }

    transformation(const transformation &) = delete;

    ~transformation()
    {
        destroy();
    }

    /** Moves another image into this image; other is set to invalid
     */
    transformation &operator=(transformation &&other) noexcept
    {
        if (this != &other)
        {
            destroy();
            m_handle = other.m_handle;
            m_color_resolution = other.m_color_resolution;
            m_depth_resolution = other.m_depth_resolution;
            other.m_handle = nullptr;
        }

        return *this;
    }

    /** Invalidates this transformation
     */
    transformation &operator=(std::nullptr_t) noexcept
    {
        destroy();
        return *this;
    }

    transformation &operator=(const transformation &) = delete;

    /** Invalidates this transformation
     */
    void destroy() noexcept
    {
        if (m_handle != nullptr)
        {
            zsa_transformation_destroy(m_handle);
            m_handle = nullptr;
        }
    }

    /** Transforms the depth map into the geometry of the color camera.
     * Throws error on failure
     *
     * \sa zsa_transformation_depth_image_to_color_camera
     * Transforms the output in to the existing caller provided \p transformed_depth_image.
     */
    void depth_image_to_color_camera(const image &depth_image, image *transformed_depth_image) const
    {
        zsa_result_t result = zsa_transformation_depth_image_to_color_camera(m_handle,
                                                                             depth_image.handle(),
                                                                             transformed_depth_image->handle());
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to convert depth map to color camera geometry!");
        }
    }

    /** Transforms the depth map into the geometry of the color camera.
     * Throws error on failure
     *
     * \sa zsa_transformation_depth_image_to_color_camera
     * Creates a new image with the output.
     */
    image depth_image_to_color_camera(const image &depth_image) const
    {
        image transformed_depth_image = image::create(ZSA_IMAGE_FORMAT_DEPTH16,
                                                      m_color_resolution.width,
                                                      m_color_resolution.height,
                                                      m_color_resolution.width *
                                                          static_cast<int32_t>(sizeof(uint16_t)));
        depth_image_to_color_camera(depth_image, &transformed_depth_image);
        return transformed_depth_image;
    }

    /** Transforms depth map and a custom image into the geometry of the color camera.
     * Throws error on failure
     *
     * \sa zsa_transformation_depth_image_to_color_camera_custom
     * Transforms the output in to the existing caller provided \p transformed_depth_image \p transformed_custom_image.
     */
    void depth_image_to_color_camera_custom(const image &depth_image,
                                            const image &custom_image,
                                            image *transformed_depth_image,
                                            image *transformed_custom_image,
                                            zsa_transformation_interpolation_type_t interpolation_type,
                                            uint32_t invalid_custom_value) const
    {
        zsa_result_t result = zsa_transformation_depth_image_to_color_camera_custom(m_handle,
                                                                                    depth_image.handle(),
                                                                                    custom_image.handle(),
                                                                                    transformed_depth_image->handle(),
                                                                                    transformed_custom_image->handle(),
                                                                                    interpolation_type,
                                                                                    invalid_custom_value);
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to convert depth map and custom image to color camera geometry!");
        }
    }

    /** Transforms depth map and a custom image into the geometry of the color camera.
     * Throws error on failure
     *
     * \sa zsa_transformation_depth_image_to_color_camera_custom
     * Creates a new image with the output.
     */
    std::pair<image, image>
    depth_image_to_color_camera_custom(const image &depth_image,
                                       const image &custom_image,
                                       zsa_transformation_interpolation_type_t interpolation_type,
                                       uint32_t invalid_custom_value) const
    {
        image transformed_depth_image = image::create(ZSA_IMAGE_FORMAT_DEPTH16,
                                                      m_color_resolution.width,
                                                      m_color_resolution.height,
                                                      m_color_resolution.width *
                                                          static_cast<int32_t>(sizeof(uint16_t)));
        int32_t bytes_per_pixel;
        switch (custom_image.get_format())
        {
        case ZSA_IMAGE_FORMAT_CUSTOM8:
            bytes_per_pixel = static_cast<int32_t>(sizeof(int8_t));
            break;
        case ZSA_IMAGE_FORMAT_CUSTOM16:
            bytes_per_pixel = static_cast<int32_t>(sizeof(int16_t));
            break;
        default:
            throw error("Failed to support this format of custom image!");
        }
        image transformed_custom_image = image::create(custom_image.get_format(),
                                                       m_color_resolution.width,
                                                       m_color_resolution.height,
                                                       m_color_resolution.width * bytes_per_pixel);
        depth_image_to_color_camera_custom(depth_image,
                                           custom_image,
                                           &transformed_depth_image,
                                           &transformed_custom_image,
                                           interpolation_type,
                                           invalid_custom_value);
        return { std::move(transformed_depth_image), std::move(transformed_custom_image) };
    }

    /** Transforms the color image into the geometry of the depth camera.
     * Throws error on failure
     *
     * \sa zsa_transformation_color_image_to_depth_camera
     * Transforms the output in to the existing caller provided \p transformed_color_image.
     */
    void color_image_to_depth_camera(const image &depth_image,
                                     const image &color_image,
                                     image *transformed_color_image) const
    {
        zsa_result_t result = zsa_transformation_color_image_to_depth_camera(m_handle,
                                                                             depth_image.handle(),
                                                                             color_image.handle(),
                                                                             transformed_color_image->handle());
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to convert color image to depth camera geometry!");
        }
    }

    /** Transforms the color image into the geometry of the depth camera.
     * Throws error on failure
     *
     * \sa zsa_transformation_color_image_to_depth_camera
     * Creates a new image with the output.
     */
    image color_image_to_depth_camera(const image &depth_image, const image &color_image) const
    {
        image transformed_color_image = image::create(ZSA_IMAGE_FORMAT_COLOR_BGRA32,
                                                      m_depth_resolution.width,
                                                      m_depth_resolution.height,
                                                      m_depth_resolution.width * 4 *
                                                          static_cast<int32_t>(sizeof(uint8_t)));
        color_image_to_depth_camera(depth_image, color_image, &transformed_color_image);
        return transformed_color_image;
    }

    /** Transforms the depth image into 3 planar images representing X, Y and Z-coordinates of corresponding 3d points.
     * Throws error on failure.
     *
     * \sa zsa_transformation_depth_image_to_point_cloud
     * Transforms the output in to the existing caller provided \p xyz_image.
     */
    void depth_image_to_point_cloud(const image &depth_image, zsa_calibration_type_t camera, image *xyz_image) const
    {
        zsa_result_t result =
            zsa_transformation_depth_image_to_point_cloud(m_handle, depth_image.handle(), camera, xyz_image->handle());
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to transform depth image to point cloud!");
        }
    }

    /** Transforms the depth image into 3 planar images representing X, Y and Z-coordinates of corresponding 3d points.
     * Throws error on failure.
     *
     * \sa zsa_transformation_depth_image_to_point_cloud
     * Creates a new image with the output.
     */
    image depth_image_to_point_cloud(const image &depth_image, zsa_calibration_type_t camera) const
    {
        image xyz_image = image::create(ZSA_IMAGE_FORMAT_CUSTOM,
                                        depth_image.get_width_pixels(),
                                        depth_image.get_height_pixels(),
                                        depth_image.get_width_pixels() * 3 * static_cast<int32_t>(sizeof(int16_t)));
        depth_image_to_point_cloud(depth_image, camera, &xyz_image);
        return xyz_image;
    }

private:
    zsa_transformation_t m_handle;
    struct resolution
    {
        int32_t width;
        int32_t height;
    };
    resolution m_color_resolution;
    resolution m_depth_resolution;
};

/** \class device zsa.hpp <zsa/zsa.hpp>
 * Wrapper for \ref zsa_device_t
 *
 * Wraps a handle for a device.
 */
class device
{
public:
    /** Creates a device from a zsa_device_t
     * Takes ownership of the handle, i.e. you should not call
     * zsa_device_close on the handle after giving it to the
     * device; the device will take care of that.
     */
    device(zsa_device_t handle = nullptr) noexcept : m_handle(handle) {}

    /** Moves another device into a new device
     */
    device(device &&dev) noexcept : m_handle(dev.m_handle)
    {
        dev.m_handle = nullptr;
    }

    device(const device &) = delete;

    ~device()
    {
        close();
    }

    device &operator=(const device &) = delete;

    /** Moves another device into this device; other is set to invalid
     */
    device &operator=(device &&dev) noexcept
    {
        if (this != &dev)
        {
            close();
            m_handle = dev.m_handle;
            dev.m_handle = nullptr;
        }
        return *this;
    }

    /** Returns true if the device is valid, false otherwise
     */
    operator bool() const noexcept
    {
        return m_handle != nullptr;
    }

    /** Returns the underlying zsa_device_t handle
     *
     * Note the zsa_device_t handle does not have a reference count will be destroyed when the C++ object is destroyed.
     * The caller is responsible for ensuring the C++ object outlives this handle.
     */
    zsa_device_t handle() const noexcept
    {
        return m_handle;
    }

    /** Closes a zsa device.
     *
     * \sa zsa_device_close
     */
    void close() noexcept
    {
        if (m_handle != nullptr)
        {
            zsa_device_close(m_handle);
            m_handle = nullptr;
        }
    }

    /** Reads a sensor capture into cap.  Returns true if a capture was read, false if the read timed out.
     * Throws error on failure.
     *
     * \sa zsa_device_get_capture
     */
    bool get_capture(capture *cap, std::chrono::milliseconds timeout)
    {
        zsa_capture_t capture_handle = nullptr;
        int32_t timeout_ms = internal::clamp_cast<int32_t>(timeout.count());
        zsa_wait_result_t result = zsa_device_get_capture(m_handle, &capture_handle, timeout_ms);
        if (result == ZSA_WAIT_RESULT_FAILED)
        {
            throw error("Failed to get capture from device!");
        }
        else if (result == ZSA_WAIT_RESULT_TIMEOUT)
        {
            return false;
        }

        *cap = capture(capture_handle);
        return true;
    }

    /** Reads a sensor capture into cap.  Returns true if a capture was read, false if the read timed out.
     * Throws error on failure. This API assumes an inifinate timeout.
     *
     * \sa zsa_device_get_capture
     */
    bool get_capture(capture *cap)
    {
        return get_capture(cap, std::chrono::milliseconds(ZSA_WAIT_INFINITE));
    }

    /** Reads an IMU sample.  Returns true if a sample was read, false if the read timed out.
     * Throws error on failure.
     *
     * \sa zsa_device_get_imu_sample
     */
    bool get_imu_sample(zsa_imu_sample_t *imu_sample, std::chrono::milliseconds timeout)
    {
        int32_t timeout_ms = internal::clamp_cast<int32_t>(timeout.count());
        zsa_wait_result_t result = zsa_device_get_imu_sample(m_handle, imu_sample, timeout_ms);
        if (result == ZSA_WAIT_RESULT_FAILED)
        {
            throw error("Failed to get IMU sample from device!");
        }
        else if (result == ZSA_WAIT_RESULT_TIMEOUT)
        {
            return false;
        }

        return true;
    }

    /** Reads an IMU sample.  Returns true if a sample was read, false if the read timed out.
     * Throws error on failure. This API assumes an infinate timeout.
     *
     * \sa zsa_device_get_imu_sample
     */
    bool get_imu_sample(zsa_imu_sample_t *imu_sample)
    {
        return get_imu_sample(imu_sample, std::chrono::milliseconds(ZSA_WAIT_INFINITE));
    }

    /** Starts the ZSA device's cameras
     * Throws error on failure.
     *
     * \sa zsa_device_start_cameras
     */
    void start_cameras(const zsa_device_configuration_t *configuration)
    {
        zsa_result_t result = zsa_device_start_cameras(m_handle, configuration);
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to start cameras!");
        }
    }

    /** Stops the ZSA device's cameras
     *
     * \sa zsa_device_stop_cameras
     */
    void stop_cameras() noexcept
    {
        zsa_device_stop_cameras(m_handle);
    }

    /** Starts the ZSA IMU
     * Throws error on failure
     *
     * \sa zsa_device_start_imu
     */
    void start_imu()
    {
        zsa_result_t result = zsa_device_start_imu(m_handle);
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to start IMU!");
        }
    }

    /** Stops the ZSA IMU
     *
     * \sa zsa_device_stop_imu
     */
    void stop_imu() noexcept
    {
        zsa_device_stop_imu(m_handle);
    }

    /** Get the ZSA device serial number
     * Throws error on failure.
     *
     * \sa zsa_device_get_serialnum
     */
    std::string get_serialnum() const
    {
        std::string serialnum;
        size_t buffer = 0;
        zsa_buffer_result_t result = zsa_device_get_serialnum(m_handle, &serialnum[0], &buffer);

        if (result == ZSA_BUFFER_RESULT_TOO_SMALL && buffer > 1)
        {
            serialnum.resize(buffer);
            result = zsa_device_get_serialnum(m_handle, &serialnum[0], &buffer);
            if (result == ZSA_BUFFER_RESULT_SUCCEEDED && serialnum[buffer - 1] == 0)
            {
                // std::string expects there to not be as null terminator at the end of its data but
                // zsa_device_get_serialnum adds a null terminator, so we drop the last character of the string after we
                // get the result back.
                serialnum.resize(buffer - 1);
            }
        }

        if (result != ZSA_BUFFER_RESULT_SUCCEEDED)
        {
            throw error("Failed to read device serial number!");
        }

        return serialnum;
    }

    /** Get the ZSA color sensor control value
     * Throws error on failure.
     *
     * \sa zsa_device_get_color_control
     */
    void get_color_control(zsa_color_control_command_t command, zsa_color_control_mode_t *mode, int32_t *value) const
    {
        zsa_result_t result = zsa_device_get_color_control(m_handle, command, mode, value);
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to read color control!");
        }
    }

    /** Set the ZSA color sensor control value
     * Throws error on failure.
     *
     * \sa zsa_device_set_color_control
     */
    void set_color_control(zsa_color_control_command_t command, zsa_color_control_mode_t mode, int32_t value)
    {
        zsa_result_t result = zsa_device_set_color_control(m_handle, command, mode, value);
        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to set color control!");
        }
    }

    /** Get the raw calibration blob for the entire ZSA device.
     * Throws error on failure.
     *
     * \sa zsa_device_get_raw_calibration
     */
    std::vector<uint8_t> get_raw_calibration() const
    {
        std::vector<uint8_t> calibration;
        size_t buffer = 0;
        zsa_buffer_result_t result = zsa_device_get_raw_calibration(m_handle, &calibration[0], &buffer);

        if (result == ZSA_BUFFER_RESULT_TOO_SMALL && buffer > 1)
        {
            calibration.resize(buffer);
            result = zsa_device_get_raw_calibration(m_handle, &calibration[0], &buffer);
        }

        if (result != ZSA_BUFFER_RESULT_SUCCEEDED)
        {
            throw error("Failed to read raw device calibration!");
        }

        return calibration;
    }

    /** Get the camera calibration for the entire ZSA device, which is used for all transformation functions.
     * Throws error on failure.
     *
     * \sa zsa_device_get_calibration
     */
    calibration get_calibration(zsa_depth_mode_t depth_mode, zsa_color_resolution_t color_resolution) const
    {
        calibration calib;
        zsa_result_t result = zsa_device_get_calibration(m_handle, depth_mode, color_resolution, &calib);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to read device calibration!");
        }
        return calib;
    }

    /** Get the device jack status for the synchronization in connector
     * Throws error on failure.
     *
     * \sa zsa_device_get_sync_jack
     */
    bool is_sync_in_connected() const
    {
        bool sync_in_jack_connected, sync_out_jack_connected;
        zsa_result_t result = zsa_device_get_sync_jack(m_handle, &sync_in_jack_connected, &sync_out_jack_connected);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to read sync jack status!");
        }
        return sync_in_jack_connected;
    }

    /** Get the device jack status for the synchronization out connector
     * Throws error on failure.
     *
     * \sa zsa_device_get_sync_jack
     */
    bool is_sync_out_connected() const
    {
        bool sync_in_jack_connected, sync_out_jack_connected;
        zsa_result_t result = zsa_device_get_sync_jack(m_handle, &sync_in_jack_connected, &sync_out_jack_connected);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to read sync jack status!");
        }
        return sync_out_jack_connected;
    }

    /** Get the version numbers of the ZSA subsystems' firmware
     * Throws error on failure.
     *
     * \sa zsa_device_get_version
     */
    zsa_hardware_version_t get_version() const
    {
        zsa_hardware_version_t version;
        zsa_result_t result = zsa_device_get_version(m_handle, &version);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to read device firmware information!");
        }
        return version;
    }

    /** Open a zsa device.
     * Throws error on failure.
     *
     * \sa zsa_device_open
     */
    static device open(uint32_t index)
    {
        zsa_device_t handle = nullptr;
        zsa_result_t result = zsa_device_open(index, &handle);

        if (ZSA_RESULT_SUCCEEDED != result)
        {
            throw error("Failed to open device!");
        }
        return device(handle);
    }

    /** Gets the number of connected devices
     *
     * \sa zsa_device_get_installed_count
     */
    static uint32_t get_installed_count() noexcept
    {
        return zsa_device_get_installed_count();
    }

private:
    zsa_device_t m_handle;
};

/**
 * @}
 */

} // namespace zsa

#endif
