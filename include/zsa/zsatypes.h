/** \file zsatypes.h
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 * Kinect For Azure SDK Type definitions.
 */

#ifndef ZSATYPES_H
#define ZSATYPES_H

#ifdef __cplusplus
#include <cinttypes>
#include <cstddef>
#include <cstring>
#else
#include <inttypes.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Declare an opaque handle type.
 *
 * \param _handle_name_ The type name of the handle
 *
 * \remarks
 * This is used to define the public handle types for the Azure Kinect APIs.
 * The macro is not intended to be used outside of the Azure Kinect headers.
 */
#define ZSA_DECLARE_HANDLE(_handle_name_)                                                                              \
    typedef struct _##_handle_name_                                                                                    \
    {                                                                                                                  \
        size_t _rsvd; /**< Reserved, do not use. */                                                                    \
    } * _handle_name_;

/**
 * \defgroup csdk C Reference
 *
 */
/**
 * \defgroup Handles Handles
 * \ingroup csdk
 * Handles represent object instances.
 *
 * Handles are opaque pointers returned by the SDK which represent an object.
 *
 * @{
 */

/** \class zsa_device_t zsa.h <zsa/zsa.h>
 * Handle to an Azure Kinect device.
 *
 * \remarks
 * Handles are created with zsa_device_open() and closed with zsa_device_close(). Invalid handles are set to 0.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
ZSA_DECLARE_HANDLE(zsa_device_t);

/** \class zsa_capture_t zsa.h <zsa/zsa.h>
 * Handle to an Azure Kinect capture.
 *
 * \remarks
 * A capture represents a set of images that were captured by a device at approximately the same time. A capture may
 * have a color, IR, and depth image. A capture may have up to one image of each type. A capture may have no image for a
 * given type as well.
 *
 * \remarks
 * Captures also store a temperature value which represents the temperature of the device at the time of the capture.
 *
 * \remarks
 * While all the images associated with the capture were collected at approximately the same time, each image has an
 * individual timestamp which may differ from each other. If the device was configured to capture depth and color images
 * separated by a delay, zsa_device_get_capture() will return a capture containing both image types separated by the
 * configured delay.
 *
 * \remarks
 * The capture will add a reference on any \ref zsa_image_t that is added to it with one of the setters like
 * zsa_capture_set_color_image(). If an existing image is being replaced, the previous image will have the reference
 * released.
 *
 * \remarks
 * Once all references to the \ref zsa_capture_t are released, the capture will release the references it holds on any
 * \ref zsa_image_t that are associated with it.
 *
 * \remarks
 * When a capture returns an image to the caller with an accessor like zsa_capture_get_color_image(), a new reference is
 * added to the \ref zsa_image_t and the caller is responsible for releasing that reference with zsa_image_release().
 *
 * \remarks
 * Empty captures are created with zsa_capture_create().
 *
 * \remarks
 * Captures can be obtained from a device using zsa_device_get_capture().
 *
 * \remarks
 * The caller must call zsa_capture_release() to release its reference to any \ref zsa_capture_t that it receives from
 * an Azure Kinect API.
 *
 * \remarks
 * Additional references to the capture may be obtained with zsa_capture_reference().
 *
 * \remarks
 * An invalid zsa_capture_t will be set to 0.
 *
 * \see zsa_image_t
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
ZSA_DECLARE_HANDLE(zsa_capture_t);

/** \class zsa_image_t zsa.h <zsa/zsa.h>
 * Handle to an Azure Kinect image.
 *
 * \remarks
 * A \ref zsa_image_t manages an image buffer and associated metadata.
 *
 * \remarks
 * The memory associated with the image buffer in an \ref zsa_image_t may have been allocated by the Azure Kinect APIs
 * or by the application. If the \ref zsa_image_t was created by an Azure Kinect API, its memory will be freed when all
 * references to the \ref zsa_image_t are released. All images retrieved directly from a device will have been created
 * by the API. An application can create a \ref zsa_image_t using memory that it has allocated using
 * zsa_image_create_from_buffer(). In that case the memory is not freed when the references are released but instead the
 * application is provided with a callback. Applications need to create a zsa_image_t to provide as an output to the
 * zsa_transformation_t transformation functions.
 *
 * \remarks
 * An image has a number of metadata properties that can be set or retrieved using the accessor functions. Not all
 * properties are applicable to images of all types. See the documentation for the individual accessors for more
 * information on their applicability and meaning.
 *
 * \remarks
 * Images may be of one of the standard \ref zsa_image_format_t formats, or may be of format ::ZSA_IMAGE_FORMAT_CUSTOM.
 * The format defines how the underlying image buffer should be interpreted.
 *
 * \remarks
 * Images from a device are retrieved through a \ref zsa_capture_t object returned by zsa_device_get_capture().
 *
 * \remarks
 * Images stored in a zsa_capture_t are referenced by the zsa_capture_t until they are replaced or the zsa_capture_t is
 * destroyed.
 *
 * \remarks
 * The caller must call zsa_image_release() to release its reference to any \ref zsa_image_t that it receives from
 * an Azure Kinect API.
 *
 * \remarks
 * Additional references to the image may be obtained with zsa_image_reference().
 *
 * \remarks
 * An invalid zsa_image_t will be set to 0.
 *
 * \see zsa_capture_t
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
ZSA_DECLARE_HANDLE(zsa_image_t);

/** \class zsa_transformation_t zsa.h <zsa/zsa.h>
 * Handle to an Azure Kinect transformation context.
 *
 * \remarks
 * Handles are created with zsa_transformation_create() and closed with zsa_transformation_destroy().
 *
 * \remarks
 * The transformation handle is used to transform images from the coordinate system of one camera into the other. Each
 * transformation handle requires some pre-computed resources to be allocated, which are retained until the handle is
 * destroyed.
 *
 * \remarks
 * Invalid handles are set to 0.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
ZSA_DECLARE_HANDLE(zsa_transformation_t);

/**
 *
 * @}
 *
 * \defgroup Enumerations Enumerations
 * \ingroup csdk
 *
 * Enumeration types used by APIs.
 *
 * @{
 */

/** Result code returned by Azure Kinect APIs.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_RESULT_SUCCEEDED = 0, /**< The result was successful */
    ZSA_RESULT_FAILED,        /**< The result was a failure */
} zsa_result_t;

/** Result code returned by Azure Kinect APIs.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_BUFFER_RESULT_SUCCEEDED = 0, /**< The result was successful */
    ZSA_BUFFER_RESULT_FAILED,        /**< The result was a failure */
    ZSA_BUFFER_RESULT_TOO_SMALL,     /**< The input buffer was too small */
} zsa_buffer_result_t;

/** Result code returned by Azure Kinect APIs.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_WAIT_RESULT_SUCCEEDED = 0, /**< The result was successful */
    ZSA_WAIT_RESULT_FAILED,        /**< The result was a failure */
    ZSA_WAIT_RESULT_TIMEOUT,       /**< The operation timed out */
} zsa_wait_result_t;

/** Verbosity levels of debug messaging
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_LOG_LEVEL_CRITICAL = 0, /**< Most severe level of debug messaging. */
    ZSA_LOG_LEVEL_ERROR,        /**< 2nd most severe level of debug messaging. */
    ZSA_LOG_LEVEL_WARNING,      /**< 3nd most severe level of debug messaging. */
    ZSA_LOG_LEVEL_INFO,         /**< 2nd least severe level of debug messaging. */
    ZSA_LOG_LEVEL_TRACE,        /**< Least severe level of debug messaging. */
    ZSA_LOG_LEVEL_OFF,          /**< No logging is performed */
} zsa_log_level_t;

/** Depth sensor capture modes.
 *
 * \remarks
 * See the hardware specification for additional details on the field of view, and supported frame rates
 * for each mode.
 *
 * \remarks
 * NFOV and WFOV denote Narrow and Wide Field Of View configurations.
 *
 * \remarks
 * Binned modes reduce the captured camera resolution by combining adjacent sensor pixels into a bin.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
// Be sure to update zsa_depth_mode_to_string in zsa.c if enum values are added.
typedef enum
{
    ZSA_DEPTH_MODE_OFF = 0,        /**< Depth sensor will be turned off with this setting. */
    ZSA_DEPTH_MODE_NFOV_2X2BINNED, /**< Depth captured at 320x288. Passive IR is also captured at 320x288. */
    ZSA_DEPTH_MODE_NFOV_UNBINNED,  /**< Depth captured at 640x576. Passive IR is also captured at 640x576. */
    ZSA_DEPTH_MODE_WFOV_2X2BINNED, /**< Depth captured at 512x512. Passive IR is also captured at 512x512. */
    ZSA_DEPTH_MODE_WFOV_UNBINNED,  /**< Depth captured at 1024x1024. Passive IR is also captured at 1024x1024. */
    ZSA_DEPTH_MODE_PASSIVE_IR,     /**< Passive IR only, captured at 1024x1024. */
} zsa_depth_mode_t;

/** Color sensor resolutions.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
// Be sure to update zsa_color_resolution_to_string in zsa.c if enum values are added.
typedef enum
{
    ZSA_COLOR_RESOLUTION_OFF = 0, /**< Color camera will be turned off with this setting */
    ZSA_COLOR_RESOLUTION_720P,    /**< 1280 * 720  16:9 */
    ZSA_COLOR_RESOLUTION_1080P,   /**< 1920 * 1080 16:9 */
    ZSA_COLOR_RESOLUTION_1440P,   /**< 2560 * 1440 16:9 */
    ZSA_COLOR_RESOLUTION_1536P,   /**< 2048 * 1536 4:3  */
    ZSA_COLOR_RESOLUTION_2160P,   /**< 3840 * 2160 16:9 */
    ZSA_COLOR_RESOLUTION_3072P,   /**< 4096 * 3072 4:3  */
} zsa_color_resolution_t;

/** Image format type.
 *
 * \remarks
 * The image format indicates how the \ref zsa_image_t buffer data is interpreted.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
// Be sure to update zsa_image_format_to_string in zsa.c if enum values are added.
typedef enum
{
    /** Color image type MJPG.
     *
     * \details
     * The buffer for each image is encoded as a JPEG and can be decoded by a JPEG decoder.
     *
     * \details
     * Because the image is compressed, the stride parameter for the \ref zsa_image_t is not applicable.
     *
     * \details
     * Each MJPG encoded image in a stream may be of differing size depending on the compression efficiency.
     */
    ZSA_IMAGE_FORMAT_COLOR_MJPG = 0,

    /** Color image type NV12.
     *
     * \details
     * NV12 images separate the luminance and chroma data such that all the luminance is at the
     * beginning of the buffer, and the chroma lines follow immediately after.
     *
     * \details
     * Stride indicates the length of each line in bytes and should be used to determine the start location of each line
     * of the image in memory. Chroma has half as many lines of height and half the width in pixels of the luminance.
     * Each chroma line has the same width in bytes as a luminance line.
     */
    ZSA_IMAGE_FORMAT_COLOR_NV12,

    /** Color image type YUY2.
     *
     * \details
     * YUY2 stores chroma and luminance data in interleaved pixels.
     *
     * \details
     * Stride indicates the length of each line in bytes and should be used to determine the start location of each
     * line of the image in memory.
     */
    ZSA_IMAGE_FORMAT_COLOR_YUY2,

    /** Color image type BGRA32.
     *
     * \details
     * Each pixel of BGRA32 data is four bytes. The first three bytes represent Blue, Green,
     * and Red data. The fourth byte is the alpha channel and is unused in the Azure Kinect APIs.
     *
     * \details
     * Stride indicates the length of each line in bytes and should be used to determine the start location of each
     * line of the image in memory.
     *
     * \details
     * The Azure Kinect device does not natively capture in this format. Requesting images of this format
     * requires additional computation in the API.
     *
     */
    ZSA_IMAGE_FORMAT_COLOR_BGRA32,

    /** Depth image type DEPTH16.
     *
     * \details
     * Each pixel of DEPTH16 data is two bytes of little endian unsigned depth data. The unit of the data is in
     * millimeters from the origin of the camera.
     *
     * \details
     * Stride indicates the length of each line in bytes and should be used to determine the start location of each
     * line of the image in memory.
     */
    ZSA_IMAGE_FORMAT_DEPTH16,

    /** Image type IR16.
     *
     * \details
     * Each pixel of IR16 data is two bytes of little endian unsigned depth data. The value of the data represents
     * brightness.
     *
     * \details
     * This format represents infrared light and is captured by the depth camera.
     *
     * \details
     * Stride indicates the length of each line in bytes and should be used to determine the start location of each
     * line of the image in memory.
     */
    ZSA_IMAGE_FORMAT_IR16,

    /** Single channel image type CUSTOM8.
     *
     * \details
     * Each pixel of CUSTOM8 is a single channel one byte of unsigned data.
     *
     * \details
     * Stride indicates the length of each line in bytes and should be used to determine the start location of each
     * line of the image in memory.
     */
    ZSA_IMAGE_FORMAT_CUSTOM8,

    /** Single channel image type CUSTOM16.
     *
     * \details
     * Each pixel of CUSTOM16 is a single channel two bytes of little endian unsigned data.
     *
     * \details
     * Stride indicates the length of each line in bytes and should be used to determine the start location of each
     * line of the image in memory.
     */
    ZSA_IMAGE_FORMAT_CUSTOM16,

    /** Custom image format.
     *
     * \details
     * Used in conjunction with user created images or images packing non-standard data.
     *
     * \details
     * See the originator of the custom formatted image for information on how to interpret the data.
     */
    ZSA_IMAGE_FORMAT_CUSTOM,
} zsa_image_format_t;

/** Transformation interpolation type.
 *
 * \remarks
 * Interpolation type used with zsa_transformation_depth_image_to_color_camera_custom.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_TRANSFORMATION_INTERPOLATION_TYPE_NEAREST = 0, /**< Nearest neighbor interpolation */
    ZSA_TRANSFORMATION_INTERPOLATION_TYPE_LINEAR,      /**< Linear interpolation */
} zsa_transformation_interpolation_type_t;

/** Color and depth sensor frame rate.
 *
 * \remarks
 * This enumeration is used to select the desired frame rate to operate the cameras. The actual
 * frame rate may vary slightly due to dropped data, synchronization variation between devices,
 * clock accuracy, or if the camera exposure priority mode causes reduced frame rate.
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
// Be sure to update zsa_fps_to_string in zsa.c if enum values are added.
typedef enum
{
    ZSA_FRAMES_PER_SECOND_5 = 0, /**< 5 FPS */
    ZSA_FRAMES_PER_SECOND_15,    /**< 15 FPS */
    ZSA_FRAMES_PER_SECOND_30,    /**< 30 FPS */
} zsa_fps_t;

/** Color sensor control commands
 *
 * \remarks
 * The current settings can be read with zsa_device_get_color_control(). The settings can be set with
 * zsa_device_set_color_control().
 *
 * \remarks
 * Control values set on a device are reset only when the device is power cycled. The device will retain the settings
 * even if the \ref zsa_device_t is closed or the application is restarted.
 *
 * \see zsa_color_control_mode_t
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    /** Exposure time setting.
     *
     * \details
     * May be set to ::ZSA_COLOR_CONTROL_MODE_AUTO or ::ZSA_COLOR_CONTROL_MODE_MANUAL.
     *
     * \details
     * The Azure Kinect supports a limited number of fixed expsore settings. When setting this, expect the exposure to
     * be rounded up to the nearest setting. Exceptions are 1) The last value in the table is the upper limit, so a
     * value larger than this will be overridden to the largest entry in the table. 2) The exposure time cannot be
     * larger than the equivelent FPS. So expect 100ms exposure time to be reduced to 30ms or 33.33ms when the camera is
     * started. The most recent copy of the table 'device_exposure_mapping' is in
     * https://github.com/microsoft/Azure-Kinect-Sensor-SDK/blob/develop/src/color/color_priv.h
     *
     * \details
     * Exposure time is measured in microseconds.
     */
    ZSA_COLOR_CONTROL_EXPOSURE_TIME_ABSOLUTE = 0,

    /** Exposure or Framerate priority setting.
     *
     * \details
     * May only be set to ::ZSA_COLOR_CONTROL_MODE_MANUAL.
     *
     * \details
     * Value of 0 means framerate priority. Value of 1 means exposure priority.
     *
     * \details
     * Using exposure priority may impact the framerate of both the color and depth cameras.
     *
     * \details
     * Deprecated starting in 1.2.0. Please discontinue usage, firmware does not support this.
     */
    ZSA_COLOR_CONTROL_AUTO_EXPOSURE_PRIORITY,

    /** Brightness setting.
     *
     * \details
     * May only be set to ::ZSA_COLOR_CONTROL_MODE_MANUAL.
     *
     * \details
     * The valid range is 0 to 255. The default value is 128.
     */
    ZSA_COLOR_CONTROL_BRIGHTNESS,

    /** Contrast setting.
     *
     * \details
     * May only be set to ::ZSA_COLOR_CONTROL_MODE_MANUAL.
     */
    ZSA_COLOR_CONTROL_CONTRAST,

    /** Saturation setting.
     *
     * \details
     * May only be set to ::ZSA_COLOR_CONTROL_MODE_MANUAL.
     */
    ZSA_COLOR_CONTROL_SATURATION,

    /** Sharpness setting.
     *
     * \details
     * May only be set to ::ZSA_COLOR_CONTROL_MODE_MANUAL.
     */
    ZSA_COLOR_CONTROL_SHARPNESS,

    /** White balance setting.
     *
     * \details
     * May be set to ::ZSA_COLOR_CONTROL_MODE_AUTO or ::ZSA_COLOR_CONTROL_MODE_MANUAL.
     *
     * \details
     * The unit is degrees Kelvin. The setting must be set to a value evenly divisible by 10 degrees.
     */
    ZSA_COLOR_CONTROL_WHITEBALANCE,

    /** Backlight compensation setting.
     *
     * \details
     * May only be set to ::ZSA_COLOR_CONTROL_MODE_MANUAL.
     *
     * \details
     * Value of 0 means backlight compensation is disabled. Value of 1 means backlight compensation is enabled.
     */
    ZSA_COLOR_CONTROL_BACKLIGHT_COMPENSATION,

    /** Gain setting.
     *
     * \details
     * May only be set to ::ZSA_COLOR_CONTROL_MODE_MANUAL.
     */
    ZSA_COLOR_CONTROL_GAIN,

    /** Powerline frequency setting.
     *
     * \details
     * May only be set to ::ZSA_COLOR_CONTROL_MODE_MANUAL.
     *
     * \details
     * Value of 1 sets the powerline compensation to 50 Hz. Value of 2 sets the powerline compensation to 60 Hz.
     */
    ZSA_COLOR_CONTROL_POWERLINE_FREQUENCY
} zsa_color_control_command_t;

/** Color sensor control mode
 *
 * \remarks
 * The current settings can be read with zsa_device_get_color_control(). The settings can be set with
 * zsa_device_set_color_control().
 *
 * \see zsa_color_control_command_t
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_COLOR_CONTROL_MODE_AUTO = 0, /**< set the associated zsa_color_control_command_t to auto*/
    ZSA_COLOR_CONTROL_MODE_MANUAL,   /**< set the associated zsa_color_control_command_t to manual*/
} zsa_color_control_mode_t;

/** Synchronization mode when connecting two or more devices together.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_WIRED_SYNC_MODE_STANDALONE, /**< Neither 'Sync In' or 'Sync Out' connections are used. */
    ZSA_WIRED_SYNC_MODE_MASTER,     /**< The 'Sync Out' jack is enabled and synchronization data it driven out the
                                       connected wire. While in master mode the color camera must be enabled as part of the
                                       multi device sync signalling logic. Even if the color image is not needed, the color
                                       camera must be running.*/
    ZSA_WIRED_SYNC_MODE_SUBORDINATE /**< The 'Sync In' jack is used for synchronization and 'Sync Out' is driven for the
                                       next device in the chain. 'Sync Out' is a mirror of 'Sync In' for this mode.
                                     */
} zsa_wired_sync_mode_t;

/** Calibration types.
 *
 * Specifies a type of calibration.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_CALIBRATION_TYPE_UNKNOWN = -1, /**< Calibration type is unknown */
    ZSA_CALIBRATION_TYPE_DEPTH,        /**< Depth sensor */
    ZSA_CALIBRATION_TYPE_COLOR,        /**< Color sensor */
    ZSA_CALIBRATION_TYPE_GYRO,         /**< Gyroscope sensor */
    ZSA_CALIBRATION_TYPE_ACCEL,        /**< Accelerometer sensor */
    ZSA_CALIBRATION_TYPE_NUM,          /**< Number of types excluding unknown type*/
} zsa_calibration_type_t;

/** Calibration model type.
 *
 * The model used interpret the calibration parameters.
 *
 * \remarks
 * Azure Kinect devices are calibrated with Brown Conrady.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_CALIBRATION_LENS_DISTORTION_MODEL_UNKNOWN = 0, /**< Calibration model is unknown */
    ZSA_CALIBRATION_LENS_DISTORTION_MODEL_THETA, /**< Deprecated (not supported). Calibration model is Theta (arctan).
                                                  */
    ZSA_CALIBRATION_LENS_DISTORTION_MODEL_POLYNOMIAL_3K, /**< Deprecated (not supported). Calibration model is
                                                            Polynomial 3K. */
    ZSA_CALIBRATION_LENS_DISTORTION_MODEL_RATIONAL_6KT,  /**< Deprecated (only supported early internal devices).
                                                            Calibration model is Rational 6KT. */
    ZSA_CALIBRATION_LENS_DISTORTION_MODEL_BROWN_CONRADY, /**< Calibration model is Brown Conrady (compatible with
                                                          * OpenCV) */
} zsa_calibration_model_type_t;

/** Firmware build type.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_FIRMWARE_BUILD_RELEASE, /**< Production firmware. */
    ZSA_FIRMWARE_BUILD_DEBUG    /**< Pre-production firmware. */
} zsa_firmware_build_t;

/** Firmware signature type.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef enum
{
    ZSA_FIRMWARE_SIGNATURE_MSFT,    /**< Microsoft signed firmware. */
    ZSA_FIRMWARE_SIGNATURE_TEST,    /**< Test signed firmware. */
    ZSA_FIRMWARE_SIGNATURE_UNSIGNED /**< Unsigned firmware. */
} zsa_firmware_signature_t;

/**
 *
 * @}
 *
 * \defgroup Macros Macros
 * \ingroup csdk
 *
 * Public macros for API consumers.
 *
 * @{
 */

/** Validate that a zsa_result_t is successful.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
#define ZSA_SUCCEEDED(_result_) (_result_ == ZSA_RESULT_SUCCEEDED)

/** Validate that a zsa_result_t is failed.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
#define ZSA_FAILED(_result_) (!ZSA_SUCCEEDED(_result_))

/**
 *
 * @}
 *
 * \defgroup Logging Error and event logging
 *
 * The SDK can log data to the console, files, or to a custom handler.
 *
 * Environment Variables:
 *
 * ZSA_ENABLE_LOG_TO_A_FILE / ZSA_RECORD_ENABLE_LOG_TO_A_FILE
 * Specifies the log file to save the log to. K4a.dll and zsarecord.dll can not log to the same file.
 *
 * ZSA_ENABLE_LOG_TO_A_FILE =
 * ZSA_RECORD_ENABLE_LOG_TO_A_FILE =
 *    0    - completely disable logging to a file
 *    log\custom.log - log all messages to the path and file specified - must end in '.log' to
 *                     be considered a valid entry
 *    NOTE 1: When enabled this takes precedence over the value of ZSA_ENABLE_LOG_TO_STDOUT.
 *    NOTE 2: This can not be set to the same value as ZSA_RECORD_ENABLE_LOG_TO_A_FILE as they represent separate
 *            logger instance that do not allowed shared access to the file being written to.
 *
 * ZSA_ENABLE_LOG_TO_STDOUT =
 *    0    - disable logging to stdout
 *    all else  - log all messages to stdout
 *
 * ZSA_LOG_LEVEL =
 *    'c'  - log all messages of level 'critical' criticality
 *    'e'  - log all messages of level 'error' or higher criticality
 *    'w'  - log all messages of level 'warning' or higher criticality
 *    'i'  - log all messages of level 'info' or higher criticality
 *    't'  - log all messages of level 'trace' or higher criticality
 *    DEFAULT - log all message of level 'error' or higher criticality
 *
 * See remarks section of \p zsa_set_debug_message_handler
 *
 * @{
 *
 *
 */

/**
 *
 * @}
 *
 * \defgroup Prototypes Callback Function prototypes
 * \ingroup csdk
 *
 * Prototypes used in callbacks.
 *
 * @{
 */

/** Callback function for debug messages being generated by the Azure Kinect SDK.
 *
 * \param context
 * The context of the callback function. This is the context that was supplied by the caller to \p
 * zsa_set_debug_message_handler.
 *
 * \param level
 * The level of the message that has been created.
 *
 * \param file
 * The file name of the source file that generated the message.
 *
 * \param line
 * The line number of the source file that generated the message.
 *
 * \param message
 * The messaged generated by the Azure Kinect SDK.
 *
 * \remarks
 * The callback is called asynchronously when the Azure Kinext SDK generates a message at a \p level that is equal to
 * or more critical than the level specified when calling \ref zsa_set_debug_message_handler() to register the callback.
 *
 * \remarks
 * This callback can occur from any thread and blocks the calling thread. The zsa_logging_message_cb_t function user
 * must protect it's logging resources from concurrent calls. All care should be made to minimize the amount of time
 * locks are held.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 *
 */
typedef void(zsa_logging_message_cb_t)(void *context,
                                       zsa_log_level_t level,
                                       const char *file,
                                       const int line,
                                       const char *message);

/** Callback function for a memory object being destroyed.
 *
 * \param buffer
 * The buffer pointer that was supplied by the caller as \p buffer_release_cb to \ref zsa_image_create_from_buffer().
 *
 * \param context
 * The context for the memory object that needs to be destroyed that was supplied by the caller as \p
 * buffer_release_cb_context to \ref zsa_image_create_from_buffer().
 *
 * \remarks
 * When all references for the memory object are released, this callback will be invoked as the final destroy for the
 * given memory.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 *
 */
typedef void(zsa_memory_destroy_cb_t)(void *buffer, void *context);

/** Callback function for a memory allocation.
 *
 * \param size
 * Minimum size in bytes needed for the buffer.
 *
 * \param context
 * Output parameter for a context that will be provided in the subsequent call to the \ref zsa_memory_destroy_cb_t
 * callback.
 *
 * \return
 * A pointer to the newly allocated memory.
 *
 * \remarks
 * A callback of this type is provided when there is an application defined allocator.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 *
 */
typedef uint8_t *(zsa_memory_allocate_cb_t)(int size, void **context);

/**
 *
 * @}
 *
 * \defgroup Structures Structures
 * \ingroup csdk
 *
 * Structures used in the API.
 *
 * @{
 */

/** Configuration parameters for an Azure Kinect device.
 *
 * \remarks
 * Used by zsa_device_start_cameras() to specify the configuration of the data capture.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef struct _zsa_device_configuration_t
{
    /** Image format to capture with the color camera.
     *
     * The color camera does not natively produce BGRA32 images.
     * Setting ::ZSA_IMAGE_FORMAT_COLOR_BGRA32 for color_format will result in higher CPU utilization. */
    zsa_image_format_t color_format;

    /** Image resolution to capture with the color camera. */
    zsa_color_resolution_t color_resolution;

    /** Capture mode for the depth camera. */
    zsa_depth_mode_t depth_mode;

    /** Desired frame rate for the color and depth camera. */
    zsa_fps_t camera_fps;

    /** Only produce zsa_capture_t objects if they contain synchronized color and depth images.
     *
     * \details
     * This setting controls the behavior in which images are dropped when images are produced faster than they can be
     * read, or if there are errors in reading images from the device.
     *
     * \details
     * If set to true, \ref zsa_capture_t objects will only be produced with both color and depth images.
     * If set to false, \ref zsa_capture_t objects may be produced only a single image when the corresponding image is
     * dropped.
     *
     * \details
     * Setting this to false ensures that the caller receives all of the images received from the camera, regardless of
     * whether the corresponding images expected in the capture are available.
     *
     * \details
     * If either the color or depth camera are disabled, this setting has no effect.
     */
    bool synchronized_images_only;

    /**
     * Desired delay between the capture of the color image and the capture of the depth image.
     *
     * \details
     * A negative value indicates that the depth image should be captured before the color image.
     *
     * \details
     * Any value between negative and positive one capture period is valid.
     */
    int32_t depth_delay_off_color_usec;

    /** The external synchronization mode. */
    zsa_wired_sync_mode_t wired_sync_mode;

    /**
     * The external synchronization timing.
     *
     * If this camera is a subordinate, this sets the capture delay between the color camera capture and the external
     * input pulse. A setting of zero indicates that the master and subordinate color images should be aligned.
     *
     * This setting does not effect the 'Sync out' connection.
     *
     * This value must be positive and range from zero to one capture period.
     *
     * If this is not a subordinate, then this value is ignored. */
    uint32_t subordinate_delay_off_master_usec;

    /**
     * Streaming indicator automatically turns on when the color or depth camera's are in use.
     *
     * This setting disables that behavior and keeps the LED in an off state. */
    bool disable_streaming_indicator;
} zsa_device_configuration_t;

/** Extrinsic calibration data.
 *
 * \remarks
 * Extrinsic calibration defines the physical relationship between two separate devices.
 *
 * \relates zsa_calibration_camera_t
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef struct _zsa_calibration_extrinsics_t
{
    float rotation[9];    /**< 3x3 Rotation matrix stored in row major order */
    float translation[3]; /**< Translation vector, x,y,z (in millimeters) */
} zsa_calibration_extrinsics_t;

/** Camera intrinsic calibration data.
 *
 * \remarks
 * Intrinsic calibration represents the internal optical properties of the camera.
 *
 * \remarks
 * Azure Kinect devices are calibrated with Brown Conrady which is compatible
 * with OpenCV.
 *
 * \relates zsa_calibration_camera_t
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef union
{
    /** individual parameter or array representation of intrinsic model. */
    struct _param
    {
        float cx;            /**< Principal point in image, x */
        float cy;            /**< Principal point in image, y */
        float fx;            /**< Focal length x */
        float fy;            /**< Focal length y */
        float k1;            /**< k1 radial distortion coefficient */
        float k2;            /**< k2 radial distortion coefficient */
        float k3;            /**< k3 radial distortion coefficient */
        float k4;            /**< k4 radial distortion coefficient */
        float k5;            /**< k5 radial distortion coefficient */
        float k6;            /**< k6 radial distortion coefficient */
        float codx;          /**< Center of distortion in Z=1 plane, x (only used for Rational6KT) */
        float cody;          /**< Center of distortion in Z=1 plane, y (only used for Rational6KT) */
        float p2;            /**< Tangential distortion coefficient 2 */
        float p1;            /**< Tangential distortion coefficient 1 */
        float metric_radius; /**< Metric radius */
    } param;                 /**< Individual parameter representation of intrinsic model */
    float v[15];             /**< Array representation of intrinsic model parameters */
} zsa_calibration_intrinsic_parameters_t;

/** Camera sensor intrinsic calibration data.
 *
 * \remarks
 * Intrinsic calibration represents the internal optical properties of the camera.
 *
 * \remarks
 * Azure Kinect devices are calibrated with Brown Conrady which is compatible
 * with OpenCV.
 *
 * \relates zsa_calibration_camera_t
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef struct _zsa_calibration_intrinsics_t
{
    zsa_calibration_model_type_t type;                 /**< Type of calibration model used*/
    unsigned int parameter_count;                      /**< Number of valid entries in parameters*/
    zsa_calibration_intrinsic_parameters_t parameters; /**< Calibration parameters*/
} zsa_calibration_intrinsics_t;

/** Camera calibration contains intrinsic and extrinsic calibration information for a camera.
 *
 * \relates zsa_calibration_camera_t
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef struct _zsa_calibration_camera_t
{
    zsa_calibration_extrinsics_t extrinsics; /**< Extrinsic calibration data. */
    zsa_calibration_intrinsics_t intrinsics; /**< Intrinsic calibration data. */
    int resolution_width;                    /**< Resolution width of the calibration sensor. */
    int resolution_height;                   /**< Resolution height of the calibration sensor. */
    float metric_radius;                     /**< Max FOV of the camera. */
} zsa_calibration_camera_t;

/** Calibration type representing device calibration.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef struct _zsa_calibration_t
{
    zsa_calibration_camera_t depth_camera_calibration; /**< Depth camera calibration. */

    zsa_calibration_camera_t color_camera_calibration; /**< Color camera calibration. */

    /** Extrinsic transformation parameters.
     *
     * The extrinsic parameters allow 3D coordinate conversions between depth camera, color camera, the IMU's gyroscope
     * and accelerometer. To transform from a source to a target 3D coordinate system, use the parameters stored
     * under extrinsics[source][target].
     */
    zsa_calibration_extrinsics_t extrinsics[ZSA_CALIBRATION_TYPE_NUM][ZSA_CALIBRATION_TYPE_NUM];

    zsa_depth_mode_t depth_mode;             /**< Depth camera mode for which calibration was obtained. */
    zsa_color_resolution_t color_resolution; /**< Color camera resolution for which calibration was obtained. */
} zsa_calibration_t;

/** Version information.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef struct _zsa_version_t
{
    uint32_t major;     /**< Major version; represents a breaking change. */
    uint32_t minor;     /**< Minor version; represents additional features, no regression from lower versions with same
                           major version. */
    uint32_t iteration; /**< Reserved. */
} zsa_version_t;

/** Structure to define hardware version.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef struct _zsa_hardware_version_t
{
    zsa_version_t rgb;          /**< Color camera firmware version. */
    zsa_version_t depth;        /**< Depth camera firmware version. */
    zsa_version_t audio;        /**< Audio device firmware version. */
    zsa_version_t depth_sensor; /**< Depth sensor firmware version. */

    zsa_firmware_build_t firmware_build;         /**< Build type reported by the firmware. */
    zsa_firmware_signature_t firmware_signature; /**< Signature type of the firmware. */
} zsa_hardware_version_t;

/** Two dimensional floating point vector.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef union
{
    /** XY or array representation of vector */
    struct _xy
    {
        float x; /**< X component of a vector */
        float y; /**< Y component of a vector */
    } xy;        /**< X, Y representation of a vector */
    float v[2];  /**< Array representation of a vector */
} zsa_float2_t;

/** Three dimensional floating point vector.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef union
{
    /** XYZ or array representation of vector. */
    struct _xyz
    {
        float x; /**< X component of a vector. */
        float y; /**< Y component of a vector. */
        float z; /**< Z component of a vector. */
    } xyz;       /**< X, Y, Z representation of a vector. */
    float v[3];  /**< Array representation of a vector. */
} zsa_float3_t;

/** IMU sample.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
typedef struct _zsa_imu_sample_t
{
    float temperature;            /**< Temperature reading of this sample (Celsius). */
    zsa_float3_t acc_sample;      /**< Accelerometer sample in meters per second squared. */
    uint64_t acc_timestamp_usec;  /**< Timestamp of the accelerometer in microseconds. */
    zsa_float3_t gyro_sample;     /**< Gyro sample in radians per second. */
    uint64_t gyro_timestamp_usec; /**< Timestamp of the gyroscope in microseconds */
} zsa_imu_sample_t;

/**
 *
 * @}
 *
 * \defgroup Definitions Definitions
 * \ingroup csdk
 *
 * Definition of constant values.
 *
 * @{
 */

/** Default device index.
 *
 * Passed as an argument to \ref zsa_device_open() to open the default sensor
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
#define ZSA_DEVICE_DEFAULT (0)

/** An infinite wait time for functions that take a timeout parameter.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
#define ZSA_WAIT_INFINITE (-1)

/** Initial configuration setting for disabling all sensors.
 *
 * \remarks
 * Use this setting to initialize a \ref zsa_device_configuration_t to a disabled state.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsatypes.h (include zsa/zsa.h)</requirement>
 * </requirements>
 * \endxmlonly
 */
static const zsa_device_configuration_t ZSA_DEVICE_CONFIG_INIT_DISABLE_ALL = { ZSA_IMAGE_FORMAT_COLOR_MJPG,
                                                                               ZSA_COLOR_RESOLUTION_OFF,
                                                                               ZSA_DEPTH_MODE_OFF,
                                                                               ZSA_FRAMES_PER_SECOND_30,
                                                                               false,
                                                                               0,
                                                                               ZSA_WIRED_SYNC_MODE_STANDALONE,
                                                                               0,
                                                                               false };

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ZSATYPES_H */
