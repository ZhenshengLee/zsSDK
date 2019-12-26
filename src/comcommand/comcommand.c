// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

//************************ Includes *****************************

// This library
#include "com_cmd_priv.h"

// System dependencies
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

// Ensure we have LIBCOM_API_VERSION defined if not defined by libcom.h
#ifndef LIBCOM_API_VERSION
#define LIBCOM_API_VERSION 0
#endif

FORCEINLINE zsa_result_t
TraceLibComError(int err, const char *szCall, const char *szFile, int line, const char *szFunction)
{
    zsa_result_t result = ZSA_RESULT_SUCCEEDED;
    if (err < 0)
    {
        // Example print:
        //  depth.cpp (86): allocator_create(&depth->allocator) returned ERROR_NOT_FOUND in depth_create

        LOG_ERROR("%s (%d): %s returned %s in %s ", szFile, line, szCall, libcom_error_name(err), szFunction);
        result = ZSA_RESULT_FAILED;
    }

    return result;
}

#define ZSA_RESULT_FROM_LIBCOM(_call_) TraceLibComError((_call_), #_call_, __FILE__, __LINE__, __func__)

//**************Symbolic Constant Macros (defines)  *************

//************************ Typedefs *****************************
typedef struct _com_command_header_t
{
    uint32_t packet_type;
    uint32_t packet_transaction_id;
    uint32_t payload_size;
    uint32_t command;
    uint32_t reserved; // Must be zero
} com_command_header_t;

typedef struct _com_command_packet_t
{
    com_command_header_t header;
    uint8_t data[COM_MAX_TX_DATA];
} com_command_packet_t;

/////////////////////////////////////////////////////
// This is the response structure going to the host.
/////////////////////////////////////////////////////

typedef struct _com_command_response_t
{
    uint32_t packet_type;
    uint32_t packet_transaction_id;
    uint32_t status;
    uint32_t reserved; // Will be zero
} com_command_response_t;

typedef struct _descriptor_choice_t
{
    uint16_t pid;
    uint8_t interface;
    uint8_t cmd_tx_endpoint;
    uint8_t cmd_rx_endpoint;
    uint8_t stream_endpoint;
    comcmd_context_t **handle_list;
} descriptor_choice_t;

//************ Declarations (Statics and globals) ***************

//******************* Function Prototypes ***********************

//*********************** Functions *****************************

#define UUID_STR_LENGTH sizeof("{00000000-0000-0000-0000-000000000000}")
static void uuid_to_string(const guid_t *guid, char *string, size_t string_size)
{
    snprintf(string,
             string_size,
             "{%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
             guid->id[3],
             guid->id[2],
             guid->id[1],
             guid->id[0],
             guid->id[5],
             guid->id[4],
             guid->id[7],
             guid->id[6],
             guid->id[9],
             guid->id[8],
             guid->id[10],
             guid->id[11],
             guid->id[12],
             guid->id[13],
             guid->id[14],
             guid->id[15]);
}

// scale the libcom debug verbosity to match zsa
static zsa_result_t com_cmd_set_libcom_debug_verbosity(comcmd_context_t *comcmd)
{
    zsa_result_t result = ZSA_RESULT_SUCCEEDED;
    return result;
}

static zsa_result_t populate_container_id(comcmd_context_t *comcmd)
{
    int i = 0;
    zsa_result_t result;
    return result;
}

static zsa_result_t find_libcom_device(uint32_t device_index,
                                       const guid_t *container_id,
                                       comcmd_context_t *comcmd)
{
    zsa_result_t result = ZSA_RESULT_SUCCEEDED;

    LOG_ERROR("zs: find_libcom_device not implemented", 0);
    return result;
}

zsa_result_t com_cmd_create(com_command_device_type_t device_type,
                            uint32_t device_index,
                            const guid_t *container_id,
                            comcmd_t *comcmd_handle)
{
    RETURN_VALUE_IF_ARG(ZSA_RESULT_FAILED, device_type >= COM_DEVICE_TYPE_COUNT);
    zsa_result_t result = ZSA_RESULT_FAILED;
    ssize_t count = 0; // holding number of devices in list
    int32_t activeConfig = 0;
    comcmd_context_t *comcmd;

    result = ZSA_RESULT_FROM_BOOL((comcmd = comcmd_t_create(comcmd_handle)) != NULL);

    if (ZSA_SUCCEEDED(result))
    {
        comcmd->stream_going = false;
        result = ZSA_RESULT_FROM_BOOL((comcmd->lock = Lock_Init()) != NULL);
    }

    if (ZSA_SUCCEEDED(result))
    {
        if (device_type == COM_DEVICE_DEPTH_PROCESSOR)
        {
            comcmd->pid = ZSA_DEPTH_PID;
            comcmd->interface = COM_CMD_DEPTH_INTERFACE;
            comcmd->cmd_tx_endpoint = COM_CMD_DEPTH_IN_ENDPOINT;
            comcmd->cmd_rx_endpoint = COM_CMD_DEPTH_OUT_ENDPOINT;
            comcmd->stream_endpoint = COM_CMD_DEPTH_STREAM_ENDPOINT;
            comcmd->source = ALLOCATION_SOURCE_COM_DEPTH;
        }
        else
        {
            comcmd->pid = ZSA_RGB_PID;
            comcmd->interface = COM_CMD_IMU_INTERFACE;
            comcmd->cmd_tx_endpoint = COM_CMD_IMU_IN_ENDPOINT;
            comcmd->cmd_rx_endpoint = COM_CMD_IMU_OUT_ENDPOINT;
            comcmd->stream_endpoint = COM_CMD_IMU_STREAM_ENDPOINT;
            comcmd->source = ALLOCATION_SOURCE_COM_IMU;
        }
        result = find_libcom_device(device_index, container_id, comcmd);
    }

    if (ZSA_SUCCEEDED(result))
    {
        result = populate_serialnumber(comcmd);
    }

    // close and free resources if error
    if (ZSA_FAILED(result))
    {
        com_cmd_destroy(*comcmd_handle);
        *comcmd_handle = NULL;
    }

    // free the list, unref the devices in it

    return result;
}

/**
 *  Function to destroy a previous device creation and
 *  releases the associated resources.
 *
 *  @param comcmd_handle
 *   Handle to the device that is being destroyed.
 *
 */
void com_cmd_destroy(comcmd_t comcmd_handle)
{
    RETURN_VALUE_IF_HANDLE_INVALID(VOID_VALUE, comcmd_t, comcmd_handle);
    comcmd_context_t *comcmd = comcmd_t_get_context(comcmd_handle);

    // Implicit stop (Must be called prior to releasing any entry resources)
    com_cmd_stream_stop(comcmd_handle);

    // Destroy the allocator
    comcmd_t_destroy(comcmd_handle);
}

zsa_buffer_result_t com_cmd_get_serial_number(comcmd_t comcmd_handle, char *serial_number, size_t *serial_number_size)
{
    RETURN_VALUE_IF_HANDLE_INVALID(ZSA_BUFFER_RESULT_FAILED, comcmd_t, comcmd_handle);
    RETURN_VALUE_IF_ARG(ZSA_BUFFER_RESULT_FAILED, serial_number_size == NULL);
    zsa_buffer_result_t result_b = ZSA_BUFFER_RESULT_SUCCEEDED;
    return result_b;
}

/**
 *  Function to handle a command transaction with a sensor module
 *
 *  @param comcmd_handle
 *   Handle to the device that is being read from.
 *
 *  @param cmd
 *   Which command to send
 *
 *  @param p_cmd_data
 *   Pointer to information needed for the command operation
 *
 *  @param cmd_data_size
 *   Size of the associated information
 *
 *  @param p_rx_data
 *   Pointer to where the data will be placed during a read operation
 *
 *  @param rx_data_size
 *   Size of the buffer provided to receive  data
 *
 *  @param p_tx_data
 *   Pointer to where the data will be placed during a write operation
 *
 *  @param tx_data_size
 *   Number of bytes to send
 *
 *  @param transfer_count
 *   Pointer to hold the number of bytes actually sent or read from the device.  Use NULL if not needed
 *
 *  @param cmd_status
 *   Pointer to hold the returned status of the command operation
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED   Operation successful
 *   ZSA_RESULT_FAILED      Operation failed
 *
 */
static zsa_result_t com_cmd_io(comcmd_t comcmd_handle,
                               uint32_t cmd,
                               void *p_cmd_data,
                               size_t cmd_data_size,
                               void *p_rx_data,
                               size_t rx_data_size,
                               void *p_tx_data,
                               size_t tx_data_size,
                               size_t *transfer_count,
                               uint32_t *cmd_status)
{
    RETURN_VALUE_IF_HANDLE_INVALID(ZSA_RESULT_FAILED, comcmd_t, comcmd_handle);
    RETURN_VALUE_IF_ARG(ZSA_RESULT_FAILED, cmd_status == NULL);
    RETURN_VALUE_IF_ARG(ZSA_RESULT_FAILED, p_rx_data != NULL && p_tx_data != NULL);


    zsa_result_t result = ZSA_RESULT_SUCCEEDED;
    return result;
}

/**
 *  Function to read data from the device.
 *
 *  @param comcmd_handle
 *   Handle to the device that is being read from.
 *
 *  @param cmd
 *   Which read to perform
 *
 *  @param p_cmd_data
 *   Pointer to information needed for the read operation
 *
 *  @param cmd_data_size
 *   Size of the associated information
 *
 *  @param p_data
 *   Pointer to where the data will be placed during the read operation
 *
 *  @param data_size
 *   Size of the buffer provided to receive the data
 *
 *  @param bytes_read
 *   Pointer to hold the number of bytes actually read from the device.  Use NULL if not needed
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED   Operation successful
 *   ZSA_RESULT_FAILED      Operation failed
 *
 */
zsa_result_t com_cmd_read(comcmd_t comcmd_handle,
                          uint32_t cmd,
                          uint8_t *p_cmd_data,
                          size_t cmd_data_size,
                          uint8_t *p_data,
                          size_t data_size,
                          size_t *bytes_read)
{
    uint32_t cmd_status;
    zsa_result_t result = ZSA_RESULT_FAILED;

    result = TRACE_CALL(
        com_cmd_io(comcmd_handle, cmd, p_cmd_data, cmd_data_size, p_data, data_size, NULL, 0, bytes_read, &cmd_status));

    if (ZSA_SUCCEEDED(result) && cmd_status != 0)
    {
        LOG_ERROR("Read command(%08X) ended in failure, Command status 0x%08x", cmd, cmd_status);
        result = ZSA_RESULT_FAILED;
    }

    return result;
}

/**
 *  Function to read data from the device.
 *
 *  @param comcmd_handle
 *   Handle to the device that is being read from.
 *
 *  @param cmd
 *   Which read to perform
 *
 *  @param p_cmd_data
 *   Pointer to information needed for the read operation
 *
 *  @param cmd_data_size
 *   Size of the associated information
 *
 *  @param p_data
 *   Pointer to where the data will be placed during the read operation
 *
 *  @param data_size
 *   Size of the buffer provided to receive the data
 *
 *  @param bytes_read
 *   Pointer to hold the number of bytes actually read from the device.  Use NULL if not needed
 *
 *  @param cmd_status
 *   Pointer to hold the returned status of the command operation
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED   Operation successful
 *   ZSA_RESULT_FAILED      Operation failed
 *
 */
zsa_result_t com_cmd_read_with_status(comcmd_t comcmd_handle,
                                      uint32_t cmd,
                                      uint8_t *p_cmd_data,
                                      size_t cmd_data_size,
                                      uint8_t *p_data,
                                      size_t data_size,
                                      size_t *bytes_read,
                                      uint32_t *cmd_status)
{
    RETURN_VALUE_IF_ARG(ZSA_RESULT_FAILED, cmd_status == NULL);

    return com_cmd_io(
        comcmd_handle, cmd, p_cmd_data, cmd_data_size, p_data, data_size, NULL, 0, bytes_read, cmd_status);
}

/**
 *  Function to write data to the device.
 *
 *  @param comcmd_handle
 *   Handle to the device that is being written to.
 *
 *  @param cmd
 *   Which write to perform
 *
 *  @param p_cmd_data
 *   Pointer to information needed for the write operation
 *
 *  @param cmd_data_size
 *   Size of the associated information
 *
 *  @param p_data
 *   Pointer to data being sent to the device
 *
 *  @param data_size
 *   Size of the data buffer being sent
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED   Operation successful
 *   ZSA_RESULT_FAILED      Operation failed
 *
 */
zsa_result_t com_cmd_write(comcmd_t comcmd_handle,
                           uint32_t cmd,
                           uint8_t *p_cmd_data,
                           size_t cmd_data_size,
                           uint8_t *p_data,
                           size_t data_size)
{
    uint32_t cmd_status;
    zsa_result_t result = ZSA_RESULT_FAILED;

    result = TRACE_CALL(
        com_cmd_io(comcmd_handle, cmd, p_cmd_data, cmd_data_size, NULL, 0, p_data, data_size, NULL, &cmd_status));

    if (ZSA_SUCCEEDED(result) && cmd_status != 0)
    {
        LOG_ERROR("Write command(%08X) ended in failure, Command status 0x%08x", cmd, cmd_status);
        result = ZSA_RESULT_FAILED;
    }

    return result;
}

/**
 *  Function to write data to the device.
 *
 *  @param comcmd_handle
 *   Handle to the device that is being written to.
 *
 *  @param cmd
 *   Which write to perform
 *
 *  @param p_cmd_data
 *   Pointer to information needed for the write operation
 *
 *  @param cmd_data_size
 *   Size of the associated information
 *
 *  @param p_data
 *   Pointer to data being sent to the device
 *
 *  @param data_size
 *   Size of the data buffer being sent
 *
 *  @param cmd_status
 *   Pointer to hold the returned status of the command operation
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED   Operation successful
 *   ZSA_RESULT_FAILED      Operation failed
 *
 */
zsa_result_t com_cmd_write_with_status(comcmd_t comcmd_handle,
                                       uint32_t cmd,
                                       uint8_t *p_cmd_data,
                                       size_t cmd_data_size,
                                       uint8_t *p_data,
                                       size_t data_size,
                                       uint32_t *cmd_status)
{
    zsa_result_t result = ZSA_RESULT_FAILED;

    result = TRACE_CALL(
        com_cmd_io(comcmd_handle, cmd, p_cmd_data, cmd_data_size, NULL, 0, p_data, data_size, NULL, cmd_status));

    return result;
}

/**
 *  Function registering the callback function associated with
 *  streaming data.
 *
 *  @param comcmd_handle
 *   Handle to the context where the callback will be stored
 *
 *  @param context
 *   Data that will be handed back with the callback
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED   Operation successful
 *   ZSA_RESULT_FAILED      Operation failed
 *
 */
zsa_result_t com_cmd_stream_register_cb(comcmd_t comcmd_handle, com_cmd_stream_cb_t *capture_ready_cb, void *context)
{
    zsa_result_t result;
    comcmd_context_t *comcmd;

    RETURN_VALUE_IF_HANDLE_INVALID(ZSA_RESULT_FAILED, comcmd_t, comcmd_handle)

    result = ZSA_RESULT_FROM_BOOL((comcmd = comcmd_t_get_context(comcmd_handle)) != NULL);

    // check if handle is valid
    if (ZSA_SUCCEEDED(result))
    {
        comcmd->callback = capture_ready_cb;
        comcmd->stream_context = context;
    }
    return result;
}

/**
 *  Function to get the number of sensor modules attached
 *
 *  @param p_device_count
 *   Pointer to where the device count will be placed
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED   Operation successful
 *   ZSA_RESULT_FAILED      Operation failed
 *
 */
zsa_result_t com_cmd_get_device_count(uint32_t *p_device_count)
{
    zsa_result_t result = ZSA_RESULT_SUCCEEDED;
    ssize_t count;            // holding number of devices in list
    int err;
    uint32_t color_device_count = 0;
    uint32_t depth_device_count = 0;

    return result;
}

// Waiting on hot-plugging support
#if 0
/**
 *  Function to get the attachment bus number and port path for a particular handle
 *
 *  @param comcmd
 *   Handle that will be used to get the bus and port path
 *
 *  @param p_bus
 *   Pointer to where the bus number will be placed
 *
 *  @param p_path
 *   Pointer to where the port numbers will be placed
 *
 *  @param path_size
 *   number of port numbers supported in the port path array
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED           Operation successful
 *   ZSA_RESULT_FAILED              Operation failed
 *
 */
zsa_result_t com_cmd_path_get(comcmd_t comcmd_handle, uint8_t *p_bus, uint8_t *p_path, uint8_t path_size)
{
    zsa_result_t result = ZSA_RESULT_SUCCEEDED;
    comcmd_t *comcmd = (comcmd_t *)comcmd_handle;
    if (comcmd != NULL)
    {
        *p_bus = comcmd->bus;
        // clear path
        memset(p_path, 0, path_size);
        // copy over the path
        if (path_size > COM_CMD_PORT_DEPTH)
        {
            path_size = COM_CMD_PORT_DEPTH;
        }
        memcpy(p_path, comcmd->port_path, path_size);
    }
    else
    {
        LOG_ERROR( "Error comcmd is NULL",0);
        result = ZSA_RESULT_FAILED;
    }
    return result;
}
#endif

const guid_t *com_cmd_get_container_id(comcmd_t comcmd_handle)
{
    RETURN_VALUE_IF_HANDLE_INVALID(NULL, comcmd_t, comcmd_handle);

    comcmd_context_t *comcmd = comcmd_t_get_context(comcmd_handle);

    return &comcmd->container_id;
}
