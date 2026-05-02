#include "SoundSource2D.h"
#include "AudioPlayer.h"

namespace gce
{
    /////////////////////////////////////
    /// @brief Default constructor.
    /// @note Initializes as a basic Sound.
    /////////////////////////////////////
    SoundSource2D::SoundSource2D() : Sound()
    {
    }

    /////////////////////////////////////
    /// @brief Destructor.
    /// @note Cleans up resources.
    /////////////////////////////////////
    SoundSource2D::~SoundSource2D()
    {
    }

    /////////////////////////////////////
    /// @brief Initializes the 2D sound source.
    /// @param wfx Pointer to wave format structure.
    /// @param buffer Pointer to audio buffer.
    /// @param id Unique sound ID.
    /// @return HRESULT indicating success or failure.
    /// @note Calls base Sound::Init.
    /////////////////////////////////////
    HRESULT SoundSource2D::Init(WAVEFORMATEX* wfx, XAUDIO2_BUFFER* buffer, uint32 const id , float gain
        , Category category, IXAudio2SubmixVoice* submix)
    {
        Sound::Init(wfx, buffer, id, gain , category , submix);
        return S_OK;
    }

    /////////////////////////////////////
    /// @brief Starts playback.
    /// @note Sets state to PLAYING.
    /////////////////////////////////////
    void SoundSource2D::Play()
    {
        if (m_pSourceVoice)
        {
            m_pSourceVoice->Start(0);
            SetState(AudioState::PLAYING);
        }
    }

    /////////////////////////////////////
    /// @brief Pauses playback.
    /// @note Only pauses if currently playing.
    /////////////////////////////////////
    void SoundSource2D::Pause()
    {
        if (m_pSourceVoice)
        {
            m_pSourceVoice->Stop(0);
            SetState(AudioState::PAUSE);
        }
    }

    /////////////////////////////////////
    /// @brief Resumes playback.
    /// @note Only resumes if currently paused.
    /////////////////////////////////////
    void SoundSource2D::Resume()
    {
        if (m_pSourceVoice)
        {
            m_pSourceVoice->Start(0);
            SetState(AudioState::PLAYING);
        }
    }

    /////////////////////////////////////
    /// @brief Stops playback.
    /// @note Sets state to STOP.
    /////////////////////////////////////
    void SoundSource2D::Stop()
    {
        if (m_pSourceVoice)
        {
            m_pSourceVoice->Stop(0);
            SetState(AudioState::STOP);
        }
    }

    /////////////////////////////////////
    /// @brief Sets playback volume.
    /// @param volume Volume level (0.0-1.0).
    /// @note Updates both internal volume and source voice.
    /////////////////////////////////////
    void SoundSource2D::SetVolume(float32 volume , float32 category)
    {
        m_volume = volume;
        if (m_pSourceVoice)
        {
            m_pSourceVoice->SetVolume(category * volume * m_gain);
        }
    }
}