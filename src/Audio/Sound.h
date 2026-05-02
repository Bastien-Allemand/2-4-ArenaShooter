#ifndef AUDIO_Sound_H_INCLUDED
#define AUDIO_Sound_H_INCLUDED

#include "define.h"
#include <XAudio2.h>
#include "AudioCategories.h"

namespace gce
{
    enum class AudioState
    {
        START,
        DELAY,
        PLAYING,
        PAUSE,
        RESUME,
        STOP
        
    };

    class Sound : public IXAudio2VoiceCallback
    {
    public:
        Sound();
        virtual ~Sound();

        virtual HRESULT Init(WAVEFORMATEX* wfx, XAUDIO2_BUFFER* buffer, uint32 const id , float gain 
            , Category category, IXAudio2SubmixVoice* submix = nullptr) = 0;
        virtual void Play() = 0;
        virtual void Playing(float deltaTime = 0) = 0;
        virtual void Pause() = 0;
        virtual void Resume() = 0;
        virtual void Stop() = 0;
        virtual void SetVolume(float32 volume , float32 category = 1.0f) = 0;
        virtual void Delay(float deltaTime) = 0;

        IXAudio2SourceVoice* GetVoice() const;
        uint32 GetSoundID() const;
        void SetState(AudioState state);
        AudioState GetState() const;

        void FillBufferAgain();
        void SaveSound();

        float32 GetVolume() const;
        Category GetCategory() const;

        void STDMETHODCALLTYPE OnStreamEnd() override;
        void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32) override {}
        void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override {}
        void STDMETHODCALLTYPE OnBufferStart(void*) override {}
        void STDMETHODCALLTYPE OnBufferEnd(void*) override {}
        void STDMETHODCALLTYPE OnLoopEnd(void*) override {}
        void STDMETHODCALLTYPE OnVoiceError(void*, HRESULT) override {}

        //C un dragon céleste pour la mettre en protected ? ya kizaro qui va se ramener pour me mettre des coup de pied si je la touche ???
        HRESULT SubmitAudioBufferToVoice(XAUDIO2_BUFFER* buffer);

    protected:
        WAVEFORMATEX* m_pWfx;
        XAUDIO2_BUFFER* m_pBuffer;
        IXAudio2SourceVoice* m_pSourceVoice;
        float32 m_volume;
        uint32 m_soundId;
        AudioState m_state;

        HRESULT CreateSourceVoice(WAVEFORMATEX* wfx);
        

        float m_gain;
        Category m_category;
        IXAudio2SubmixVoice* m_psubmix;
    };
}

#endif