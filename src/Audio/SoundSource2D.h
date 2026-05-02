#ifndef AUDIO_SoundSource2D_H_INCLUDED
#define AUDIO_SoundSource2D_H_INCLUDED

#include "Sound.h"

namespace gce
{
    class SoundSource2D : public Sound
    {
    public:
        SoundSource2D();
        virtual ~SoundSource2D();

        virtual HRESULT Init(WAVEFORMATEX* wfx, XAUDIO2_BUFFER* buffer, uint32 const id , float gain
            , Category category, IXAudio2SubmixVoice* submix = nullptr) override;
        void Play() override;
        void Playing(float deltaTime = 0) override {};
        void Pause() override;
        void Resume() override;
        void Stop() override;
        void SetVolume(float32 volume , float32 category = 1.0f) override;
        void Delay(float deltaTime) override {};

    };
}

#endif