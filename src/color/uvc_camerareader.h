#ifndef UVC_CAMERAREADER_H
#define UVC_CAMERAREADER_H
// zsa
#include <zsa/zsatypes.h>
#include <zsainternal/color.h>

#include "color_priv.h"

// STL
#include <mutex>

// external
#include <libuvc/libuvc.h>
#include "turbojpeg.h"

class UVCCameraReader
{
public:
    UVCCameraReader();
    virtual ~UVCCameraReader();

    zsa_result_t Init(const char *serialNumber);

    zsa_result_t Start(const uint32_t width,
                       const uint32_t height,
                       const float fps,
                       const zsa_image_format_t imageFormat,
                       color_cb_stream_t *pCallback,
                       void *pCallbackContext);

    void Stop();

    void Shutdown();

    zsa_result_t GetCameraControlCapabilities(const zsa_color_control_command_t command,
                                              color_control_cap_t *capabilities);

    zsa_result_t GetCameraControl(const zsa_color_control_command_t command,
                                  zsa_color_control_mode_t *mode,
                                  int32_t *pValue);

    zsa_result_t SetCameraControl(const zsa_color_control_command_t command,
                                  const zsa_color_control_mode_t mode,
                                  int32_t newValue);

    void Callback(uvc_frame_t *frame);

private:
    bool IsInitialized()
    {
        return m_pContext && m_pDevice && m_pDeviceHandle;
    }

    zsa_result_t DecodeMJPEGtoBGRA32(uint8_t *in_buf, const size_t in_size, uint8_t *out_buf, const size_t out_size);

    int32_t MapK4aExposureToLinux(int32_t K4aExposure);
    int32_t MapLinuxExposureToK4a(int32_t LinuxExposure);

private:
    // Lock
    std::mutex m_mutex;

    // libUVC device and context
    uvc_context_t *m_pContext = nullptr;
    uvc_device_t *m_pDevice = nullptr;
    uvc_device_handle_t *m_pDeviceHandle = nullptr;
    bool m_streaming = false;
    bool m_using_60hz_power = true;

    // Image format cache
    uint32_t m_width_pixels;
    uint32_t m_height_pixels;
    zsa_image_format_t m_input_image_format;
    zsa_image_format_t m_output_image_format;

    // ZSA stream callback
    color_cb_stream_t *m_pCallback = nullptr;
    void *m_pCallbackContext = nullptr;

    // MJPEG decoder
    tjhandle m_decoder = nullptr;
};

#endif // UVC_CAMERAREADER_H
