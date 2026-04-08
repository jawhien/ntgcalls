//
// Created by Laky64 on 20/09/2024.
//

#pragma once
#include <queue>

#ifdef IS_WINDOWS
#include <ntgcalls/io/audio_mixer.hpp>
#include <ntgcalls/io/base_reader.hpp>
#include <ntgcalls/devices/base_device_module.hpp>
#include <modules/audio_device/win/core_audio_utility_win.h>
#include <rtc_base/win/scoped_com_initializer.h>
#include <ntgcalls/devices/device_info.hpp>
#include <rtc_base/platform_thread.h>

namespace ntgcalls {
    using Microsoft::WRL::ComPtr;
    using namespace webrtc::webrtc_win;

    class WinCoreDeviceModule final: public BaseDeviceModule, public BaseReader, public AudioMixer, public IAudioSessionEvents {
        ScopedHandle audioSamplesEvent, restartEvent, stopEvent;
        WAVEFORMATEXTENSIBLE format = {};
        uint32_t endpointBufferSizeFrames = 0;
        ComPtr<IAudioClient> audioClient;
        ComPtr<IAudioSessionControl> audioSessionControl;
        LONG refCount = 1;
        std::atomic_bool isRestarting = false;
        webrtc::ScopedCOMInitializer comInitializer;
        ScopedMMCSSRegistration mmcssRegistration;
        webrtc::PlatformThread thread;
        bool automaticRestart = false;
        int deviceIndex = -1;
        std::string deviceUID;
        ComPtr<IAudioCaptureClient> audioCaptureClient;
        ComPtr<IAudioRenderClient> audioRenderClient;
        std::mutex queueMutex;
        std::queue<bytes::unique_binary> queue;

        void init();

        void releaseCOMObjects();

        bool handleRestartEvent();

        void runDataListener();

        void stop();

        void switchDevice();

        bool handleDataRecord() const;

        bool handleDataPlayback();

        HRESULT STDMETHODCALLTYPE QueryInterface(const IID& riid, void** ppvObject) override;

        ULONG STDMETHODCALLTYPE AddRef() override;

        ULONG STDMETHODCALLTYPE Release() override;

        HRESULT STDMETHODCALLTYPE OnDisplayNameChanged(LPCWSTR NewDisplayName, LPCGUID EventContext) override;

        HRESULT STDMETHODCALLTYPE OnIconPathChanged(LPCWSTR NewIconPath, LPCGUID EventContext) override;

        HRESULT STDMETHODCALLTYPE OnSimpleVolumeChanged(float NewVolume, BOOL NewMute, LPCGUID EventContext) override;

        HRESULT STDMETHODCALLTYPE OnChannelVolumeChanged(DWORD ChannelCount, float NewChannelVolumeArray[], DWORD ChangedChannel, LPCGUID EventContext) override;

        HRESULT STDMETHODCALLTYPE OnGroupingParamChanged(LPCGUID NewGroupingParam, LPCGUID EventContext) override;

        HRESULT STDMETHODCALLTYPE OnStateChanged(AudioSessionState NewState) override;

        HRESULT STDMETHODCALLTYPE OnSessionDisconnected(AudioSessionDisconnectReason DisconnectReason) override;

    public:
        explicit WinCoreDeviceModule(const AudioDescription* desc, bool isCapture, BaseSink *sink);

        ~WinCoreDeviceModule() override;

        static bool isSupported();

        static std::vector<DeviceInfo> getDevices();

        void open() override;

        void onData(bytes::unique_binary data) override;
    };

} // ntgcalls

#endif
