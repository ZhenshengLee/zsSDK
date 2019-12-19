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

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* ZSA_H */
