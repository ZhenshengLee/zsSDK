/** \file comcommand.h
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 * Kinect For Azure SDK.
 */

#ifndef COM_COMMAND_H
#define COM_COMMAND_H

//************************ Includes *****************************
#include <zsa/zsatypes.h>
#include <zsainternal/image.h>
#include <zsainternal/common.h>

#ifdef __cplusplus
extern "C" {
#endif

//**************Symbolic Constant Macros (defines)  *************

//************************ Typedefs *****************************
typedef enum
{
    COM_DEVICE_DEPTH_PROCESSOR = 0,
    COM_DEVICE_COLOR_PROCESSOR,

    COM_DEVICE_TYPE_COUNT
} com_command_device_type_t;

#define NULL_INDEX 0xFF

typedef enum
{
    CMD_STATUS_PASS = 0,
} com_cmd_responses_t;

ZSA_DECLARE_HANDLE(comcmd_t);

/** Delivers a sample to the registered callback function when a capture is ready for processing.
 *
 * \param result
 * indicates if the capture being passed in is complete
 *
 * \param image_handle
 * image read by hardware.
 *
 * \param context
 * Context for the callback function
 *
 * \remarks
 * Capture is only of one type. At this point it is not linked to other captures. Capture is safe to use durring this
 * callback as the caller ensures a ref is held. If the callback function wants the capture to exist beyond this
 * callback, a ref must be taken with capture_inc_ref().
 */
typedef void(com_cmd_stream_cb_t)(zsa_result_t result, zsa_image_t image_handle, void *context);

//************ Declarations (Statics and globals) ***************

//******************* Function Prototypes ***********************

/** Open a handle to a COM Function device.
 *
 * \param com_handle [OUT]
 *    A pointer to write the opened comcmd_t device handle to
 *
 * \return ZSA_RESULT_SUCCEEDED if the device was opened, otherwise an error code
 *
 * If successful, \ref com_cmd_create will return a COM device handle in the COM
 * parameter. This handle grants exclusive access to the device and may be used in
 * the other zsa API calls.
 *
 * When done with the device, close the handle with \ref com_cmd_shutdown and then
 * \ref com_cmd_destroy
 */
zsa_result_t com_cmd_create(com_command_device_type_t device_type,
                            uint32_t device_index,
                            const guid_t *container_id,
                            comcmd_t *com_handle);

void com_cmd_destroy(comcmd_t com_handle);

/** Get the serial number associated with the COM descriptor.
 *
 * \param com_handle [IN]
 *    A pointer to the opened comcmd_t device handle
 *
 * \param serial_number [OUT]
 *    A pointer to write the serial number to
 *
 * \param serial_number_size [IN OUT]
 *    IN: a pointer to the size of the serial number buffer passed in
      OUT: the size of the serial number written to the buffer including the NULL.
 *
 * \return ZSA_BUFFER_RESULT_SUCCEEDED if the serial number was successfully opened, ZSA_BUFFER_RESULT_TOO_SMALL is the
 memory passed in is insufficient, ZSA_BUFFER_RESULT_FAILED if an error occurs.
 *
 * If successful, \ref serial_number will contain a serial number and serial_number_size will be the null terminated
 string length. If ZSA_BUFFER_RESULT_TOO_SMALL is returned, then serial_number_size will contain the required size.
 */
zsa_buffer_result_t com_cmd_get_serial_number(comcmd_t com_handle, char *serial_number, size_t *serial_number_size);

// Read command
zsa_result_t com_cmd_read(comcmd_t com_handle,
                          uint32_t cmd,
                          uint8_t *p_cmd_data,
                          size_t cmd_data_size,
                          uint8_t *p_data,
                          size_t data_size,
                          size_t *bytes_read);

zsa_result_t com_cmd_read_with_status(comcmd_t comcmd_handle,
                                      uint32_t cmd,
                                      uint8_t *p_cmd_data,
                                      size_t cmd_data_size,
                                      uint8_t *p_data,
                                      size_t data_size,
                                      size_t *bytes_read,
                                      uint32_t *cmd_status);

// Write command
zsa_result_t com_cmd_write(comcmd_t com_handle,
                           uint32_t cmd,
                           uint8_t *p_cmd_data,
                           size_t cmd_data_size,
                           uint8_t *p_data,
                           size_t data_size);

zsa_result_t com_cmd_write_with_status(comcmd_t com_handle,
                                       uint32_t cmd,
                                       uint8_t *p_cmd_data,
                                       size_t cmd_data_size,
                                       uint8_t *p_data,
                                       size_t data_size,
                                       uint32_t *cmd_status);

// stream data callback
zsa_result_t com_cmd_stream_register_cb(comcmd_t comcmd, com_cmd_stream_cb_t *frame_ready_cb, void *context);

zsa_result_t com_cmd_stream_start(comcmd_t com_handle, size_t payload_size);

zsa_result_t com_cmd_stream_stop(comcmd_t com_handle);

// Get the number of connected devices
zsa_result_t com_cmd_get_device_count(uint32_t *p_device_count);

const guid_t *com_cmd_get_container_id(comcmd_t comcmd_handle);

#ifdef __cplusplus
} // namespace zsa
#endif

#endif /* COM_COMMAND_H */
