#ifdef _MSC_VER

#include "isac_spatial_sound.h"

using namespace Microsoft::WRL;
using namespace winrt::Windows::Media::Devices;
using namespace sk;

constexpr uint32_t c_HrtfSampleRate = 48000;

#define RETURN_IF_NULL_ALLOC(x) \
if (x == nullptr) return E_OUTOFMEMORY

#define RETURN_IF_FAILED(x) \
{HRESULT __hr = x; \
if (FAILED(__hr)) return __hr;}

#define RETURN_HR_IF(x, y) \
if (y) return x

#define RETURN_HR_IF_NULL(x, y) \
if (y == nullptr) return x

IsacAdapter::IsacAdapter(int maxSources) : m_IsActivated(false), m_PumpEvent(nullptr)
{
    //TODO TODO TODO
    // Subscribe to device change notifications so we can reactivate later
    //auto deviceChangeHandler = [&](winrt::Windows::Foundation::IInspectable const&,
    //    DefaultAudioRenderDeviceChangedEventArgs const& args) {
    //        return HandleDeviceChange(args.Id());
    //};
    //m_DeviceChangeToken = MediaDevice::DefaultAudioRenderDeviceChanged(deviceChangeHandler);
    m_Sources = new ComPtr<ISpatialAudioObject>[maxSources];
    m_MaxSources = maxSources;
}

IsacAdapter::~IsacAdapter()
{
    // Unregister from device change notifications
    MediaDevice::DefaultAudioRenderDeviceChanged(m_DeviceChangeToken);
    delete[] m_Sources;
    CloseHandle(m_PumpEvent);
}

// Calls ActivateAudioInterfaceAsync and waits for it to complete
// On success, sets activatedDeviceId to the id of the device that ISAC is activated against
HRESULT IsacAdapter::ActivateIsacInterface(winrt::hstring* activatedDeviceId)
{
    // ActivateAudioInterfaceAsync takes a special activation object.
    // This is our implementation of that object.
    ComPtr<IsacActivator> spCompletionObject = Make<IsacActivator>();
    RETURN_IF_NULL_ALLOC(spCompletionObject);

    ComPtr<IActivateAudioInterfaceCompletionHandler> spCompletionHandler;
    RETURN_IF_FAILED(spCompletionObject.As(&spCompletionHandler));

    auto deviceId = MediaDevice::GetDefaultAudioRenderId(AudioDeviceRole::Default);

    // Activate the ISpatialAudioClient interface and wait for it to complete
    ComPtr<IActivateAudioInterfaceAsyncOperation> spOperation;
    RETURN_IF_FAILED(ActivateAudioInterfaceAsync(
        deviceId.c_str(), __uuidof(ISpatialAudioClient), nullptr, spCompletionHandler.Get(), &spOperation));
    RETURN_IF_FAILED(spCompletionObject->Wait(INFINITE));
    RETURN_IF_FAILED(spCompletionObject->GetActivateResult(&m_Isac));

    *activatedDeviceId = deviceId;
    return S_OK;
}

// Given an activated ISpatialAudioClient interface, finds the supported WAVEFORMATEX that matches our needs
// We only operate in 48kHz float. If that's supported, it is returned. Otherwise, this function fails
HRESULT FindAcceptableWaveformat(ISpatialAudioClient* isac, WAVEFORMATEX* objectFormat)
{
    // Initialize ISAC with its preferred format
    ComPtr<IAudioFormatEnumerator> audioObjectFormatEnumerator;
    RETURN_IF_FAILED(isac->GetSupportedAudioObjectFormatEnumerator(&audioObjectFormatEnumerator));
    UINT32 audioObjectFormatCount = 0;
    RETURN_IF_FAILED(audioObjectFormatEnumerator->GetCount(&audioObjectFormatCount));
    RETURN_HR_IF(E_FAIL, audioObjectFormatCount == 0);

    // Find the first format that's in 48kHz, float. That's what Unity uses
    for (auto i = 0u; i < audioObjectFormatCount; i++)
    {
        WAVEFORMATEX* format = nullptr;
        RETURN_IF_FAILED(audioObjectFormatEnumerator->GetFormat(0, &format));
        if (format->nSamplesPerSec == c_HrtfSampleRate && format->wFormatTag == WAVE_FORMAT_IEEE_FLOAT)
        {
            *objectFormat = *format;
            break;
        }
    }
    RETURN_HR_IF_NULL(E_NOT_VALID_STATE, objectFormat);

    return S_OK;
}

// Assuming ISAC has been activated, uses that ISAC in conjunction with the passed-in parameters to activate the
// spatial audio stream and associated static bed objects
HRESULT IsacAdapter::ActivateSpatialAudioStream(const WAVEFORMATEX& objectFormat, UINT32 maxObjects)
{
    RETURN_HR_IF_NULL(E_NOT_VALID_STATE, m_Isac);
    if (m_PumpEvent == nullptr)
    {
        m_PumpEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    }

    // Setup Static Bed Object mask
    AudioObjectType objectMask = AudioObjectType_None;
    SpatialAudioObjectRenderStreamActivationParams activationParams = {
        &objectFormat, objectMask, 0, maxObjects, AudioCategory_GameEffects, m_PumpEvent, nullptr };

    PROPVARIANT activateParams;
    PropVariantInit(&activateParams);
    activateParams.vt = VT_BLOB;
    activateParams.blob.cbSize = sizeof(activationParams);
    activateParams.blob.pBlobData = reinterpret_cast<BYTE*>(&activationParams);
    RETURN_IF_FAILED(m_Isac->ActivateSpatialAudioStream(&activateParams, IID_PPV_ARGS(&m_SpatialAudioStream)));

    return S_OK;
}

// Fully activates and initializes the ISAC interfaces and all related member variables
HRESULT IsacAdapter::Activate(isac_callback callback)
{
    if (callback == nullptr)
    {
        return E_INVALIDARG;
    }
    m_AppCallback = callback;
    // Nothing to do if already activated
    RETURN_HR_IF(S_OK, m_IsActivated);

    winrt::hstring deviceId;
    RETURN_IF_FAILED(ActivateIsacInterface(&deviceId));

    WAVEFORMATEX objectFormat;
    RETURN_IF_FAILED(FindAcceptableWaveformat(m_Isac.Get(), &objectFormat));

    // Determine how many dynamic objects this platform supports
    // If none, bail out. This will force Unity to fallback to its own panner
    UINT32 maxObjects = 0;
    RETURN_IF_FAILED(m_Isac->GetMaxDynamicObjectCount(&maxObjects));
    RETURN_HR_IF(E_NOT_VALID_STATE, maxObjects == 0 || maxObjects < m_MaxSources);
    RETURN_IF_FAILED(ActivateSpatialAudioStream(objectFormat, maxObjects));

    // It's important to only set the deviceId after successfully initializing everything
    // Otherwise, we may fail a retry on new device arrival notifications
    m_DeviceIdInUse = deviceId;
    m_IsActivated = true;

    // Get the dynamic sources
    for (int i = 0; i < m_MaxSources; i++)
    {
        RETURN_IF_FAILED(m_SpatialAudioStream->ActivateSpatialAudioObject(AudioObjectType_Dynamic, &m_Sources[i]));
    }

    RETURN_IF_FAILED(m_SpatialAudioStream->Start());

    if (m_PumpThread != nullptr)
    {
        CloseHandle(m_PumpThread);
    }
    m_PumpThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&IsacAdapter::SpatialAudioClientWorker, (void*)this, 0, NULL);
    SetThreadPriority(m_PumpThread, THREAD_PRIORITY_ABOVE_NORMAL);
            
    return S_OK;
}

// Reacts to an audio endpoint change
HRESULT IsacAdapter::HandleDeviceChange(winrt::hstring newDeviceId)
{
    // Don't process this device change request if we're already playing on the new device
    RETURN_HR_IF(S_OK, newDeviceId == m_DeviceIdInUse);

    m_IsActivated = false;
    m_SpatialAudioStream->Stop();

    RETURN_IF_FAILED(Activate(m_AppCallback));

    return S_OK;
}

// This must be called on a separate thread. It will run every ISAC pump pass (roughly 10ms)
DWORD WINAPI IsacAdapter::SpatialAudioClientWorker(LPVOID lpParam)
{
    IsacAdapter* pThis = (IsacAdapter*)lpParam;
    BYTE** objectBuffers = new BYTE*[pThis->m_MaxSources];
    vec3* positions = new vec3[pThis->m_MaxSources];
    float* volumes = new float[pThis->m_MaxSources];
    while (true)
    {
        DWORD waitResult = WaitForSingleObject(pThis->m_PumpEvent, INFINITE);
        if (waitResult == WAIT_OBJECT_0)
        {
            UINT32 objects = 0;
            UINT32 frameCount = 0;
            if (FAILED(pThis->m_SpatialAudioStream->BeginUpdatingAudioObjects(&objects, &frameCount)))
            {
                break;
            }

            // Fill the buffers for the call into the app code
            UINT32 byteCount;
            for (int i = 0; i < pThis->m_MaxSources; i++)
            {
                pThis->m_Sources[i]->GetBuffer(&objectBuffers[i], &byteCount);
                // Fill the buffers with 0s
                memset(objectBuffers[i], 0, byteCount);
            }

            pThis->m_AppCallback((float**)objectBuffers, pThis->m_MaxSources, byteCount / sizeof(float), positions, volumes);

            for (int i = 0; i < pThis->m_MaxSources; i++)
            {
                // Intentionally ignore any per-object failures and continue to the next object
                pThis->m_Sources[i]->SetPosition(positions[i].x, positions[i].y, positions[i].z);
                pThis->m_Sources[i]->SetVolume(volumes[i]);
            }

            pThis->m_SpatialAudioStream->EndUpdatingAudioObjects();
        }
    }

    delete[] objectBuffers;
    delete[] positions;
    delete[] volumes;
    return 0;
}

IsacActivator::IsacActivator() : m_ActivateResult(E_ILLEGAL_METHOD_CALL)
{
    m_CompletedEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

IsacActivator::~IsacActivator()
{
    CloseHandle(m_CompletedEvent);
}

STDMETHODIMP IsacActivator::ActivateCompleted(IActivateAudioInterfaceAsyncOperation* operation)
{
    SetEvent(m_CompletedEvent);
    ComPtr<IUnknown> spAudioAsUnknown;
    RETURN_IF_FAILED(operation->GetActivateResult(&m_ActivateResult, &spAudioAsUnknown));
    RETURN_IF_FAILED(m_ActivateResult);
    m_ActivateResult = spAudioAsUnknown.As(&m_Isac);

    return S_OK;
}

STDMETHODIMP IsacActivator::Wait(DWORD Timeout)
{
    DWORD waitResult = WaitForSingleObject(m_CompletedEvent, Timeout);
    if (waitResult == WAIT_OBJECT_0)
    {
        return S_OK;
    }
    else if (waitResult == WAIT_TIMEOUT)
    {
        return HRESULT_FROM_WIN32(ERROR_TIMEOUT);
    }
    else if (waitResult == WAIT_FAILED)
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return E_FAIL;
}

STDMETHODIMP IsacActivator::GetActivateResult(ISpatialAudioClient** deviceAccess)
{
    *deviceAccess = nullptr;
    RETURN_IF_FAILED(m_ActivateResult);
    RETURN_IF_FAILED(m_Isac.CopyTo(deviceAccess));
    return S_OK;
}

#endif // _MSC_VER