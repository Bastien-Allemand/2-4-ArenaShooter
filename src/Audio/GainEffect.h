/******************************************************************************/
/*  GainEffect.h                                                              */
/*  XAPO personnalisé : amplification / atténuation du signal audio.          */
/******************************************************************************/
#ifndef AUDIO_GAINEFFECT_H_INCLUDED
#define AUDIO_GAINEFFECT_H_INCLUDED

/* ===== Dépendances internes ================================================= */
#include "define.h"

/* ===== Dépendances système / STL =========================================== */
#include <xapo.h>
#include <initguid.h>     // DEFINE_GUID / __uuidof
#include <combaseapi.h>   // CoTaskMemAlloc
#include <strsafe.h>      // wcsncpy_s
#include <vector>
#include <Maths/MathsFunctions.hpp>

/* ========================================================================== */
/// @brief Paramètres exposés au runtime.
struct GainParameters
{
    float gain; ///< 1.0 = 0 dB ; <1 atténue ; >1 amplifie.
};

/* ========================================================================== */
__declspec(uuid("12345678-1234-1234-1234-1234567890ab"))
class GainEffect : public virtual IXAPO,
    public virtual IXAPOParameters
{
public:
    GainEffect();
    virtual ~GainEffect();

    /* ===== IUnknown ======================================================= */
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override;
    STDMETHOD_(ULONG, AddRef)()  override;
    STDMETHOD_(ULONG, Release)() override;
    /* ====================================================================== */

    /* ===== IXAPO ========================================================== */
    STDMETHOD(Initialize)(const void* pData, UINT32 size) override;
    STDMETHOD_(void, Reset)() override;
    STDMETHOD(LockForProcess)(
        UINT32                               inCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pIn,
        UINT32                               outCount,
        const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOut) override;
    STDMETHOD_(void, UnlockForProcess)() override;
    STDMETHOD(GetRegistrationProperties)(XAPO_REGISTRATION_PROPERTIES** ppProps) override;
    STDMETHOD_(void, Process)(UINT32 inCount,
        const XAPO_PROCESS_BUFFER_PARAMETERS* pIn,
        UINT32 outCount,
        XAPO_PROCESS_BUFFER_PARAMETERS* pOut,
        BOOL enabled) override;

    STDMETHOD_(const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS*, GetInputLockedType)();
    STDMETHOD_(const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS*, GetOutputLockedType)();

    STDMETHOD_(HRESULT, IsInputFormatSupported)(
        const WAVEFORMATEX* pOutputFormat,
        const WAVEFORMATEX* pRequestedInputFormat,
        WAVEFORMATEX** ppSupportedInputFormat)  override;
    STDMETHOD_(HRESULT, IsOutputFormatSupported)(
        const WAVEFORMATEX* pInputFormat,
        const WAVEFORMATEX* pRequestedOutputFormat,
        WAVEFORMATEX** ppSupportedOutputFormat) override;

    STDMETHOD_(UINT32, CalcInputFrames)(UINT32 outputFrameCount)  override;
    STDMETHOD_(UINT32, CalcOutputFrames)(UINT32 inputFrameCount)  override;
    /* ====================================================================== */

    /* ===== IXAPOParameters =============================================== */
    STDMETHOD_(void, SetParameters)(const void* pParameters, UINT32 size) override;
    STDMETHOD_(void, GetParameters)(void* pParameters, UINT32 size)       override;
    /* ====================================================================== */

private:
    LONG               m_refCount{ 1 };
    GainParameters     m_params{ 1.0f };
    WAVEFORMATEX       m_format{};

    XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS m_inputParams{};
    XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS m_outputParams{};
};

#endif // AUDIO_GAINEFFECT_H_INCLUDED
