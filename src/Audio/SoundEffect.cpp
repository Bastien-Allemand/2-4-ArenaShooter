/******************************************************************************/
/*  SoundEffect.cpp                                                           */
/*  Implémentation de SoundEffect.                                            */
/******************************************************************************/
#include "SoundEffect.h"

/* ===== Dépendances système / STL =========================================== */
#include <iostream>   // std::cerr / std::cout

/* ========================================================================== */
namespace gce
{
    /////////////////////////////////////////
    /// @brief Constructeur : rien à faire
    ///        hormis l’initialisation.
    /////////////////////////////////////////
    SoundEffect::SoundEffect() = default;

    /////////////////////////////////////////
    /// @brief Libère proprement toutes les
    ///        instances d’effets créées.
    /////////////////////////////////////////
    SoundEffect::~SoundEffect()
    {
        for (auto& effect : m_effectInstances)
        {
            if (effect) { effect->Release(); }
        }
        m_effectInstances.clear();
        m_effectDescriptors.clear();
    }

    /* ===== IUnknown ======================================================= */
    STDMETHODIMP SoundEffect::QueryInterface(REFIID riid, void** ppvObject)
    {
        if (!ppvObject) return E_POINTER;

        if (riid == __uuidof(IUnknown))
        {
            *ppvObject = static_cast<IUnknown*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    STDMETHODIMP_(ULONG) SoundEffect::AddRef()
    {
        return InterlockedIncrement(&m_refCount);
    }

    STDMETHODIMP_(ULONG) SoundEffect::Release()
    {
        ULONG newCount = InterlockedDecrement(&m_refCount);
        if (newCount == 0) delete this;
        return newCount;
    }
    /* ====================================================================== */

    /////////////////////////////////////////
    /// @brief  Enregistre un XAPO dans la
    ///         chaîne d’effets interne.
    /////////////////////////////////////////
    void SoundEffect::AddEffect(IUnknown* pEffect,
        bool      initialState,
        UINT32    outputChannels)
    {
        this->AddRef(); // sécurité

        XAUDIO2_EFFECT_DESCRIPTOR desc{};
        desc.pEffect = pEffect;
        desc.InitialState = initialState;
        desc.OutputChannels = outputChannels;

        m_effectDescriptors.push_back(desc);
    }

    /////////////////////////////////////////
    /// @brief  Applique la chaîne construite
    ///         à la source voice donnée.
    /////////////////////////////////////////
    void SoundEffect::ApplyToSourceVoice(IXAudio2SourceVoice* pSourceVoice)
    {
        if (!pSourceVoice) return;

        XAUDIO2_EFFECT_CHAIN chain{};
        chain.EffectCount = static_cast<UINT32>(m_effectDescriptors.size());
        chain.pEffectDescriptors = m_effectDescriptors.data();

        if (FAILED(pSourceVoice->SetEffectChain(&chain)))
        {
            std::cerr << "[SoundEffect] SetEffectChain failed\n";
            return;
        }

        /* Active et mixe chaque effet à 100 % */
        for (UINT32 i = 0; i < chain.EffectCount; ++i)
        {
            pSourceVoice->SetEffectParameters(i, nullptr, 0);
            pSourceVoice->EnableEffect(i);
        }
    }

    /////////////////////////////////////////
    /// @brief Ajoute un effet de gain simple.
    /// @param gainValue Facteur de gain (1.0 = 0 dB).
    /////////////////////////////////////////
    void SoundEffect::AddGain(float gainValue)
    {
        auto* pGain = new GainEffect();
        IXAPO* pXAPO = static_cast<IXAPO*>(pGain);
        pXAPO->Initialize(nullptr, 0);

        IXAPOParameters* pParams = nullptr;
        if (SUCCEEDED(pXAPO->QueryInterface(__uuidof(IXAPOParameters),
            reinterpret_cast<void**>(&pParams))))
        {
            GainParameters gp{ gainValue };
            pParams->SetParameters(&gp, sizeof gp);
            pParams->Release();
        }

        m_effectInstances.push_back(pXAPO);
        AddEffect(pXAPO, /*initialState=*/true, /*stéréo*/2);

        std::cout << "[SoundEffect] Gain ajouté (gain=" << gainValue << ")\n";
    }

    /* ---------------------------------------------------------------------- */
    /*  ↓↓↓  AddReverb : corps commenté — à déverrouiller quand prêt  ↓↓↓      */
    /* ---------------------------------------------------------------------- */

    void SoundEffect::AddReverb(
        UINT32 ReflectionsDelay, BYTE ReverbDelay, BYTE RearDelay,
        BYTE PositionLeft, BYTE PositionRight,
        BYTE PositionMatrixLeft, BYTE PositionMatrixRight,
        BYTE EarlyDiffusion, BYTE LateDiffusion,
        BYTE LowEQGain, BYTE LowEQCutoff,
        BYTE HighEQGain, BYTE HighEQCutoff,
        float RoomFilterFreq, float RoomFilterMain, float RoomFilterHF,
        float ReflectionsGain, float ReverbGain,
        float DecayTime, float Density, float RoomSize,
        bool initialState, UINT32 outputChannels)
    {
        /* Exemple complet gardé en commentaire pour référence :
           Active dès que ton pipeline XAudio2FX est prêt. */

           /*
           IUnknown* pReverb = nullptr;
           if (FAILED(XAudio2CreateReverb(&pReverb)) || !pReverb) return;

           XAUDIO2FX_REVERB_PARAMETERS r {};
           r.ReflectionsDelay   = ReflectionsDelay;
           r.ReverbDelay        = ReverbDelay;
           r.RearDelay          = RearDelay;
           r.PositionLeft       = PositionLeft;
           r.PositionRight      = PositionRight;
           r.PositionMatrixLeft = PositionMatrixLeft;
           r.PositionMatrixRight= PositionMatrixRight;
           r.EarlyDiffusion     = EarlyDiffusion;
           r.LateDiffusion      = LateDiffusion;
           r.LowEQGain          = LowEQGain;
           r.LowEQCutoff        = LowEQCutoff;
           r.HighEQGain         = HighEQGain;
           r.HighEQCutoff       = HighEQCutoff;
           r.RoomFilterFreq     = RoomFilterFreq;
           r.RoomFilterMain     = RoomFilterMain;
           r.RoomFilterHF       = RoomFilterHF;
           r.ReflectionsGain    = ReflectionsGain;
           r.ReverbGain         = ReverbGain;
           r.DecayTime          = DecayTime;
           r.Density            = Density;
           r.RoomSize           = RoomSize;

           IXAPOParameters* pXapoParams = nullptr;
           if (SUCCEEDED(pReverb->QueryInterface(__uuidof(IXAPOParameters),
                                                 reinterpret_cast<void**>(&pXapoParams))))
           {
               pXapoParams->SetParameters(&r, sizeof r);
               pXapoParams->Release();
           }

           m_effectInstances.push_back(static_cast<IXAPO*>(pReverb));
           AddEffect(pReverb, initialState, outputChannels);
           */
    }

    /* ============================ TODO : Echo ============================== */
    void SoundEffect::AddEcho() { /* à implémenter */ }

} // namespace gce

