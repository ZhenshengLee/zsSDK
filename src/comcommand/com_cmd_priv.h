// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

/**
 * Private header file for the command and streaming interface
 */

#pragma once

//************************ Includes *****************************
#include <inttypes.h>
#include <stdbool.h>
// This library
#include <zsainternal/comcommand.h>
#include <zsainternal/common.h>

// Dependent libraries
#include <zsainternal/allocator.h>
#include <azure_c_shared_utility/lock.h>
#include <azure_c_shared_utility/threadapi.h>

// Exteranl dependencis

// Ensure we have LIBCOM_API_VERSION defined if not defined by libcom.h
#ifndef LIBCOM_API_VERSION
#define LIBCOM_API_VERSION 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

//**************Symbolic Constant Macros (defines)  *************
#define COM_CMD_MAX_WAIT_TIME 2000
#define COM_CMD_MAX_XFR_COUNT 8 // Upper limit to the number of outstanding transfer
#ifdef _WIN32
#define COM_CMD_MAX_XFR_POOL 80000000 // Memory pool size for outstanding transfers (based on empirical testing)
#else
#define COM_CMD_MAX_XFR_POOL 10000000 // Memory pool size for outstanding transfers (based on empirical testing)
#endif
#define COM_CMD_PORT_DEPTH 8

#define COM_CMD_EVENT_WAIT_TIME 1
#define COM_MAX_TX_DATA 128
#define COM_CMD_PACKET_TYPE 0x06022009
#define COM_CMD_PACKET_TYPE_RESPONSE 0x0A6FE000
#define ZSA_MSFT_VID 0x045E
#define ZSA_RGB_PID 0x097D
#define ZSA_DEPTH_PID 0x097C
#define COM_CMD_DEFAULT_CONFIG 1

#define COM_CMD_DEPTH_INTERFACE 0
#define COM_CMD_DEPTH_IN_ENDPOINT 0x02
#define COM_CMD_DEPTH_OUT_ENDPOINT 0x81
#define COM_CMD_DEPTH_STREAM_ENDPOINT 0x83

#define COM_CMD_IMU_INTERFACE 2
#define COM_CMD_IMU_IN_ENDPOINT 0x04
#define COM_CMD_IMU_OUT_ENDPOINT 0x83
#define COM_CMD_IMU_STREAM_ENDPOINT 0x82

//************************ Typedefs *****************************
typedef struct _com_async_transfer_data_t
{
    struct _comcmd_context_t *comcmd;
    struct libcom_transfer *bulk_transfer;
    zsa_image_t image;
    uint32_t list_index;
} com_async_transfer_data_t;

typedef struct _comcmd_context_t
{
    allocation_source_t source;

    // LIBCOM properties

    uint8_t index;
    uint16_t pid;
    uint8_t interface;
    uint8_t cmd_tx_endpoint;
    uint8_t cmd_rx_endpoint;
    uint8_t stream_endpoint;
    uint32_t transaction_id;

    unsigned char serial_number[MAX_SERIAL_NUMBER_LENGTH];
    guid_t container_id;

    com_cmd_stream_cb_t *callback;
    void *stream_context;
    bool stream_going;
    com_async_transfer_data_t *transfer_list[COM_CMD_MAX_XFR_COUNT];
    size_t stream_size;
    LOCK_HANDLE lock;
    THREAD_HANDLE stream_handle;
} comcmd_context_t;

ZSA_DECLARE_CONTEXT(comcmd_t, comcmd_context_t);

//************ Declarations (Statics and globals) ***************

//******************* Function Prototypes ***********************
// void com_cmd_libcom_cb(struct libcom_transfer *bulk_transfer);

#ifdef __cplusplus
}
#endif
