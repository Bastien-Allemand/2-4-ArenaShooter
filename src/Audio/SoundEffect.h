#ifndef AUDIO_SoundEffect_H_INCLUDED
#define AUDIO_SoundEffect_H_INCLUDED

#include "define.h"
#include <XAudio2.h>
#include <xaudio2fx.h>
#include <xapo.h>

#include "GainEffect.h"

namespace gce
{
    class SoundEffect : public IUnknown
    {
    public:
        SoundEffect();
        virtual ~SoundEffect();

        void AddReverb(
            UINT32 ReflectionsDelay       = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY,
            BYTE ReverbDelay              = XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY,
            BYTE RearDelay                = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY,
            BYTE PositionLeft             = XAUDIO2FX_REVERB_DEFAULT_POSITION,
            BYTE PositionRight            = XAUDIO2FX_REVERB_DEFAULT_POSITION,
            BYTE PositionMatrixLeft       = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX,
            BYTE PositionMatrixRight      = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX,
            BYTE EarlyDiffusion           = XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION,
            BYTE LateDiffusion            = XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION,
            BYTE LowEQGain                = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN,
            BYTE LowEQCutoff              = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF,
            BYTE HighEQGain               = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN,
            BYTE HighEQCutoff             = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF,
            float RoomFilterFreq          = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ,
            float RoomFilterMain          = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN,
            float RoomFilterHF            = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF,
            float ReflectionsGain         = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN,
            float ReverbGain              = XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN,
            float DecayTime               = XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME,
            float Density                 = XAUDIO2FX_REVERB_DEFAULT_DENSITY,
            float RoomSize                = XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE,
            bool initialState = true,
            UINT32 outputChannels = 2
        );

        void AddEcho();
        void AddGain(float gainValue);

        void ApplyToSourceVoice(IXAudio2SourceVoice* pSourceVoice);

    private :

        STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
        STDMETHOD_(ULONG, AddRef)() override;
        STDMETHOD_(ULONG, Release)() override;

        void AddEffect(IUnknown* pEffect, bool initialState, UINT32 outputChannels);



        std::vector<XAUDIO2_EFFECT_DESCRIPTOR> m_effectDescriptors;
        std::vector<IXAPO*> m_effectInstances;
        std::vector<IXAPOParameters*> m_xapoParameters;
        ULONG m_refCount;
    };


}

#endif