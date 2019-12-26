// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

//************************ Includes *****************************
// This library
#include <zsainternal/comcommand.h>
#include "com_cmd_priv.h"

// System dependencies
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <azure_c_shared_utility/envvariable.h>

//**************Symbolic Constant Macros (defines)  *************
#define COM_CMD_LIBCOM_EVENT_TIMEOUT 1

//************************ Typedefs *****************************

//************ Declarations (Statics and globals) ***************

//******************* Function Prototypes ***********************

//*********************** Functions *****************************
/**
 *  Utility function for releasing the transfer resources
 *
 *  @param bulk_transfer
 *   Pointer to the resources allocated for doing the com transfer
 *
 */
static void com_cmd_release_xfr()
{
}

/**
 *  Function for handling the callback from the libcom library as a result of a transfer request
 *
 *  @param bulk_transfer
 *   Pointer to the resources allocated for doing the com transfer
 *
 */
void com_cmd_libcom_cb()
{
}

/**
 *  LibUsb context thread for monitoring events in the com lib
 *
 *  @param var
 *   context variable.  In this case, this points to a command handle
 *   associated with the streaming.
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED   Operation successful
 *   ZSA_RESULT_FAILED      Operation failed
 *
 */
static int com_cmd_lib_com_thread(void *var)
{
    zsa_result_t result = ZSA_RESULT_SUCCEEDED;
    comcmd_context_t *comcmd = (comcmd_context_t *)var;

    LOG_ERROR("zs: com_cmd_lib_com_thread entering but not implemented", 0);

    ThreadAPI_Exit((int)result);
    return 0;
}

/**
 *  Function to queue up the stream transfer.  This function will allocation
 *  COM_CMD_MAX_XFR_COUNT number of transfers on the stream pipe and
 *  start the transfers.
 *
 *  @param comcmd_handle
 *   Handle to the entry that will be passed into the com library as a context
 *
 *  @param payload_size
 *   Max size of the payload that will be streamed
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED   Operation successful
 *   ZSA_RESULT_FAILED      Operation failed or stream already started
 *
 */
zsa_result_t com_cmd_stream_start(comcmd_t comcmd_handle, size_t payload_size)
{
    RETURN_VALUE_IF_HANDLE_INVALID(ZSA_RESULT_FAILED, comcmd_t, comcmd_handle);
    RETURN_VALUE_IF_ARG(ZSA_RESULT_FAILED, payload_size == 0);

    comcmd_context_t *comcmd;
    zsa_result_t result;

    result = ZSA_RESULT_FROM_BOOL((comcmd = comcmd_t_get_context(comcmd_handle)) != NULL);

    // start stream handler
    if (ZSA_SUCCEEDED(result))
    {
        result = ZSA_RESULT_FAILED;

        // Sync operation with commands going to device
        Lock(comcmd->lock);
        if (comcmd->stream_going)
        {
            // Steam already going (Error?)
            LOG_INFO("Stream already in progress", 0);
        }
        else
        {
            comcmd->stream_size = payload_size;
            comcmd->stream_going = true;
            if (ThreadAPI_Create(&(comcmd->stream_handle), com_cmd_lib_com_thread, comcmd) != THREADAPI_OK)
            {
                comcmd->stream_going = false;
                LOG_ERROR("Could not start stream thread", 0);
            }
            else
            {
                result = ZSA_RESULT_SUCCEEDED;
            }
        }
        Unlock(comcmd->lock);
    }

    return result;
}

/**
 *  Function for stopping the streaming on a handle. This function
 *  will block until the stream is stopped.  It is called implicitly
 *  by the com_cmd_destroy() function
 *
 *  @param comcmd_handle
 *   Handle that contains the transfer resources used.
 *
 *  @return
 *   ZSA_RESULT_SUCCEEDED   Operation successful
 *   ZSA_RESULT_FAILED      Operation failed
 *
 */
zsa_result_t com_cmd_stream_stop(comcmd_t comcmd_handle)
{
    zsa_result_t result;
    comcmd_context_t *comcmd;

    RETURN_VALUE_IF_HANDLE_INVALID(ZSA_RESULT_FAILED, comcmd_t, comcmd_handle)

    result = ZSA_RESULT_FROM_BOOL((comcmd = comcmd_t_get_context(comcmd_handle)) != NULL);

    if (ZSA_SUCCEEDED(result))
    {
        result = ZSA_RESULT_FAILED;

        // Sync operation with commands going to device
        Lock(comcmd->lock);
        comcmd->stream_going = false;

        // This function is the only place that kills the thread so this should be safe
        if (comcmd->stream_handle != NULL) // check if the thread has already stopped
        {
            ThreadAPI_Join(comcmd->stream_handle, NULL);
            comcmd->stream_handle = NULL;
        }
        Unlock(comcmd->lock);

        result = ZSA_RESULT_SUCCEEDED;
    }

    return result;
}
