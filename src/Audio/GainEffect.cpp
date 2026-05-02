/******************************************************************************/
/*  GainEffect.cpp                                                            */
/*  Implémentation du XAPO « GainEffect ».                                    */
/******************************************************************************/
#include "GainEffect.h"

/* ===== Dépendances système / STL =========================================== */
#include <cstring>   // memcpy / memcmp
#include <windows.h> // Interlocked*
#include <iostream>  // std::cout / std::cerr

/* ========================================================================== */
/*  GUID (déjà déclaré dans le header via __uuidof, mais redéfini ici pour    */
/*  l’enregistrement COM éventuel).                                           */
DEFINE_GUID(CLSID_GainEffect,
    0x12345678, 0x1234, 0x1234,
    0x12, 0x34, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab);

/* ========================================================================== */
GainEffect::GainEffect()
{
    ZeroMemory(&m_format, sizeof m_format);
}

GainEffect::~GainEffect() = default;

/* ===== IUnknown =========================================================== */
STDMETHODIMP GainEffect::QueryInterface(REFIID riid, void** ppv)
{
    if (!ppv) return E_POINTER;

    if (riid == __uuidof(IUnknown) || riid == __uuidof(IXAPO))
        *ppv = static_cast<IXAPO*>(this);
    else if (riid == __uuidof(IXAPOParameters))
        *ppv = static_cast<IXAPOParameters*>(this);
    else
        return (*ppv = nullptr), E_NOINTERFACE;

    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG) GainEffect::AddRef() { return InterlockedIncrement(&m_refCount); }
STDMETHODIMP_(ULONG) GainEffect::Release()
{
    ULONG count = InterlockedDecrement(&m_refCount);
    if (!count) delete this;
    return count;
}
/* ========================================================================== */

/* ===== IXAPO ============================================================== */
STDMETHODIMP GainEffect::Initialize(const void*, UINT32)
{
    std::cout << "[GainEffect] Initialized\n";
    return S_OK;
}

STDMETHODIMP_(void) GainEffect::Reset() { /* rien : conserve le gain courant */ }

STDMETHODIMP GainEffect::LockForProcess(
    UINT32, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pIn,
    UINT32, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOut)
{
    if (!pIn || !pOut) return E_INVALIDARG;

    m_inputParams = pIn[0];
    m_outputParams = pOut[0];

    if (m_inputParams.pFormat)
    {
        memcpy(&m_format, m_inputParams.pFormat, sizeof m_format);

        /* Vérif IEEE_FLOAT (obligatoire) */
        if (m_format.wFormatTag == WAVE_FORMAT_EXTENSIBLE)
        {
            auto* ext = reinterpret_cast<const WAVEFORMATEXTENSIBLE*>(m_inputParams.pFormat);
            if (ext->SubFormat != KSDATAFORMAT_SUBTYPE_IEEE_FLOAT)
            {
                std::cerr << "[GainEffect] Format non supporté (≠ IEEE_FLOAT)\n";
                return E_FAIL;
            }
        }

        std::cout << "[GainEffect] Locked: "
            << m_format.nChannels << " ch, "
            << m_format.nSamplesPerSec << " Hz, "
            << m_format.wBitsPerSample << " bits\n";
    }
    return S_OK;
}

STDMETHODIMP_(void) GainEffect::UnlockForProcess() { /* no‑op */ }

STDMETHODIMP GainEffect::GetRegistrationProperties(XAPO_REGISTRATION_PROPERTIES** ppProps)
{
    if (!ppProps) return E_POINTER;

    auto* p = static_cast<XAPO_REGISTRATION_PROPERTIES*>(
        CoTaskMemAlloc(sizeof(XAPO_REGISTRATION_PROPERTIES)));
    if (!p) return E_OUTOFMEMORY;

    p->clsid = CLSID_GainEffect;
    wcsncpy_s(p->FriendlyName, L"Custom Gain Effect", _TRUNCATE);
    wcsncpy_s(p->CopyrightInfo, L"(c) 2025 YourCompany", _TRUNCATE);
    p->MajorVersion = 1;
    p->MinorVersion = 0;
    p->Flags = XAPO_FLAG_FRAMERATE_MUST_MATCH;
    p->MinInputBufferCount = p->MaxInputBufferCount = 1;
    p->MinOutputBufferCount = p->MaxOutputBufferCount = 1;

    *ppProps = p;
    return S_OK;
}

STDMETHODIMP_(void) GainEffect::Process(
    UINT32, const XAPO_PROCESS_BUFFER_PARAMETERS* pIn,
    UINT32, XAPO_PROCESS_BUFFER_PARAMETERS* pOut,
    BOOL enabled)
{
    if (!enabled || pIn->BufferFlags == XAPO_BUFFER_SILENT)
    {
        pOut->BufferFlags = XAPO_BUFFER_SILENT;
        pOut->ValidFrameCount = pIn->ValidFrameCount;
        return;
    }

    const float* in = static_cast<const float*>(pIn->pBuffer);
    float* out = static_cast<float*>      (pOut->pBuffer);
    UINT32 frames = pIn->ValidFrameCount;
    UINT32 channels = m_format.nChannels;

    GainParameters params; GetParameters(&params, sizeof params);

    for (UINT32 i = 0; i < frames * channels; ++i)
    {
        float s = in[i] * params.gain;
        out[i] = gce::Clamp(s, -0.999f, 0.999f); // soft‑clip
    }

    pOut->ValidFrameCount = frames;
    pOut->BufferFlags = pIn->BufferFlags;
}
/* ========================================================================== */

STDMETHODIMP_(const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS*) GainEffect::GetInputLockedType()
{
    return &m_inputParams;
}
STDMETHODIMP_(const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS*) GainEffect::GetOutputLockedType()
{
    return &m_outputParams;
}

STDMETHODIMP_(HRESULT) GainEffect::IsInputFormatSupported(
    const WAVEFORMATEX* outFmt, const WAVEFORMATEX* reqInFmt, WAVEFORMATEX** ppSup)
{
    if (!outFmt || !reqInFmt || !ppSup) return E_POINTER;
    return (memcmp(outFmt, reqInFmt, sizeof(WAVEFORMATEX)) == 0) ? (*ppSup = nullptr, S_OK) : S_FALSE;
}

STDMETHODIMP_(HRESULT) GainEffect::IsOutputFormatSupported(
    const WAVEFORMATEX* inFmt, const WAVEFORMATEX* reqOutFmt, WAVEFORMATEX** ppSup)
{
    if (!inFmt || !reqOutFmt || !ppSup) return E_POINTER;
    return (memcmp(inFmt, reqOutFmt, sizeof(WAVEFORMATEX)) == 0) ? (*ppSup = nullptr, S_OK) : S_FALSE;
}

STDMETHODIMP_(UINT32) GainEffect::CalcInputFrames(UINT32 outFrames) { return outFrames; }
STDMETHODIMP_(UINT32) GainEffect::CalcOutputFrames(UINT32 inFrames) { return inFrames; }

/* ===== IXAPOParameters ===================================================== */
STDMETHODIMP_(void) GainEffect::SetParameters(const void* p, UINT32 size)
{
    if (size != sizeof(GainParameters) || !p) return;
    auto gp = *static_cast<const GainParameters*>(p);

    /* Validation & clamp */
    gp.gain = gce::Clamp(gp.gain, 0.0f, 10.0f);
    m_params = gp;
}

STDMETHODIMP_(void) GainEffect::GetParameters(void* p, UINT32 size)
{
    if (size == sizeof(GainParameters) && p)
        memcpy(p, &m_params, size);
}
/* ========================================================================== */
