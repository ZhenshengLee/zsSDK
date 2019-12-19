// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef MFCAMERAREADER_H
#define MFCAMERAREADER_H
// Platform
#include <windows.h>

// STL
#include <queue>

// Media Foundation
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <Mferror.h>
#include <ks.h>
#include <ksmedia.h>
#include <ksproxy.h>
#include <devpropdef.h>

// WRL
#include <wrl.h>

// zsa
#include <zsa/zsatypes.h>
#include <zsainternal/color.h>

#include <azure_c_shared_utility/envvariable.h>

#include "color_priv.h"

class CFrameContext
{
public:
    CFrameContext(_In_ IMFSample *pSample);
    ~CFrameContext();

    PBYTE GetBuffer()
    {
        return m_pBuffer;
    }
    size_t GetFrameSize()
    {
        return (size_t)m_bufferLength;
    }
    uint64_t GetExposureTime()
    {
        return m_exposureTime;
    }
    uint32_t GetWhiteBalance()
    {
        return m_whiteBalance;
    }
    uint32_t GetISOSpeed()
    {
        return m_isoSpeed;
    }
    ULONGLONG GetPTSTime()
    {
        return m_capturePTS;
    }

private:
    Microsoft::WRL::ComPtr<IMFSample> m_spSample;
    Microsoft::WRL::ComPtr<IMFMediaBuffer> m_spMediaBuffer;
    Microsoft::WRL::ComPtr<IMF2DBuffer2> m_sp2DBuffer;
    PBYTE m_pBuffer = nullptr;
    DWORD m_bufferLength = 0;

    uint64_t m_exposureTime = 0;
    uint32_t m_whiteBalance = 0;
    uint32_t m_isoSpeed = 0;
    ULONGLONG m_capturePTS = 0;
};

class CMFCameraReader
    : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
                                          IMFSourceReaderCallback>
{
public:
    CMFCameraReader();
    virtual ~CMFCameraReader();
    HRESULT RuntimeClassInitialize(GUID *containerId);

    zsa_result_t Start(const UINT32 width,
                       const UINT32 height,
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

    // IMFSourceReaderCallback
    STDMETHOD(OnReadSample)
    (HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample *pSample);

    STDMETHOD(OnFlush)(DWORD dwStreamIndex);

    STDMETHOD(OnEvent)(DWORD dwStreamIndex, IMFMediaEvent *pEvent);

private:
    HRESULT FindEdenColorCamera(GUID *containerId, Microsoft::WRL::ComPtr<IMFActivate> &spDevice);
    HRESULT GetDeviceProperty(LPCWSTR DeviceSymbolicName,
                              const DEVPROPKEY *pKey,
                              PBYTE pbBuffer,
                              ULONG cbBuffer,
                              ULONG *pcbData);

    // Camera controls
    HRESULT GetCameraControlCap(const GUID PropertySet,
                                const ULONG PropertyId,
                                bool *supportAuto,
                                LONG *minValue,
                                LONG *maxValue,
                                ULONG *stepValue,
                                LONG *defaultValue);

    HRESULT GetCameraControlValue(const GUID PropertySet,
                                  const ULONG PropertyId,
                                  LONG *pValue,
                                  ULONG *pFlags,
                                  ULONG *pCapability);

    HRESULT SetCameraControlValue(const GUID PropertySet, const ULONG PropertyId, LONG newValue, ULONG newFlags);

    zsa_result_t zsaResultFromHRESULT(const HRESULT hr)
    {
        return SUCCEEDED(hr) ? ZSA_RESULT_SUCCEEDED : ZSA_RESULT_FAILED;
    }

    int GetStride();
    zsa_result_t CreateImage(CFrameContext *pFrameContext, zsa_image_t *image);
    zsa_result_t CreateImageCopy(CFrameContext *pFrameContext, zsa_image_t *image);

    LONG CMFCameraReader::MapK4aExposureToMf(int K4aExposure);

    LONG CMFCameraReader::MapMfExponentToK4a(LONG MfExponent);

private:
    Microsoft::WRL::Wrappers::SRWLock m_lock;
    bool m_mfStarted = false;
    bool m_started = false;
    bool m_flushing = false;
    bool m_use_mf_buffer = true;
    bool m_using_60hz_power = true;
    HANDLE m_hStreamFlushed = NULL;

    Microsoft::WRL::ComPtr<IMFSourceReader> m_spSourceReader;
    Microsoft::WRL::ComPtr<IKsControl> m_spKsControl;

    UINT32 m_width_pixels;
    UINT32 m_height_pixels;
    zsa_image_format_t m_image_format;

    color_cb_stream_t *m_pCallback = nullptr;
    void *m_pCallbackContext = nullptr;
};

#endif // MFCAMERAREADER_H
