#ifndef ZSA_H
#define ZSA_H

#ifdef __cplusplus
#include <cinttypes>
#else
#include <inttypes.h>
#endif
#include <zsa/zsaversion.h>
#include <zsa/zsatypes.h>
#include <zsa/zsa_export.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup Functions Functions
 * \ingroup csdk
 *
 * Public functions of the API
 *
 * @{
 */

/** Gets the number of connected devices
 *
 * \returns Number of sensors connected to the PC.
 *
 * \relates zsa_device_t
 *
 * \remarks
 * This API counts the number of Azure Kinect devices connected to the host PC.
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsa.h (include zsa/zsa.h)</requirement>
 *   <requirement name="Library">zsa.lib</requirement>
 *   <requirement name="DLL">zsa.dll</requirement>
 * </requirements>
 * \endxmlonly
 */
ZSA_EXPORT uint32_t zsa_device_get_installed_count(void);

/** Open an Azure Kinect device.
 *
 * \param index
 * The index of the device to open, starting with 0. Optionally pass in #ZSA_DEVICE_DEFAULT.
 *
 * \param device_handle
 * Output parameter which on success will return a handle to the device.
 *
 * \relates zsa_device_t
 *
 * \return ::ZSA_RESULT_SUCCEEDED if the device was opened successfully.
 *
 * \remarks
 * If successful, zsa_device_open() will return a device handle in the device_handle parameter.
 * This handle grants exclusive access to the device and may be used in the other Azure Kinect API calls.
 *
 * \remarks
 * When done with the device, close the handle with zsa_device_close()
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsa.h (include zsa/zsa.h)</requirement>
 *   <requirement name="Library">zsa.lib</requirement>
 *   <requirement name="DLL">zsa.dll</requirement>
 * </requirements>
 * \endxmlonly
 */
ZSA_EXPORT zsa_result_t zsa_device_open(uint32_t index, zsa_device_t *device_handle);


/** Closes an Azure Kinect device.
 *
 * \param device_handle
 * Handle obtained by zsa_device_open().
 *
 * \relates zsa_device_t
 *
 * \remarks Once closed, the handle is no longer valid.
 *
 * \remarks Before closing the handle to the device, ensure that all \ref zsa_capture_t captures have been released with
 * zsa_capture_release().
 *
 * \xmlonly
 * <requirements>
 *   <requirement name="Header">zsa.h (include zsa/zsa.h)</requirement>
 *   <requirement name="Library">zsa.lib</requirement>
 *   <requirement name="DLL">zsa.dll</requirement>
 * </requirements>
 * \endxmlonly
 */
ZSA_EXPORT void zsa_device_close(zsa_device_t device_handle);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ZSA_H */
