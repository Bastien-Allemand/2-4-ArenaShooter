#include "Sound.h"
#include "AudioPlayer.h"

namespace gce
{
    /////////////////////////////////////
    /// @brief Default constructor for Sound class.
    /// @note Initializes member variables to default values.
    /////////////////////////////////////
    Sound::Sound() 
    {
        m_pWfx = nullptr;
        m_pBuffer = nullptr;
        m_pSourceVoice = nullptr;
        m_volume = 1.0f;
        m_soundId = 0;
        m_state = AudioState::STOP;
        m_gain = 0.0f;
    }

    /////////////////////////////////////
    /// @brief Destructor for Sound class.
    /// @note Cleans up the source voice if it exists.
    /////////////////////////////////////
    Sound::~Sound()
    {
        if (m_pSourceVoice)
        {
            m_pSourceVoice->DestroyVoice();
            m_pSourceVoice = nullptr;
        }
    }

    /////////////////////////////////////
    /// @brief Initializes the Sound object.
    /// @param wfx Pointer to wave format structure.
    /// @param buffer Pointer to audio buffer.
    /// @param id Unique sound ID.
    /// @return HRESULT indicating success or failure.
    /// @note Creates source voice and submits audio buffer.
    /////////////////////////////////////
    HRESULT Sound::Init(WAVEFORMATEX* wfx, XAUDIO2_BUFFER* buffer, uint32 const id , float gain 
        , Category category, IXAudio2SubmixVoice* submix)
    {
        m_soundId = id;
        m_pWfx = wfx;
        m_pBuffer = buffer;
        m_gain = gain;
        m_category = category;
        m_psubmix = submix;
        
        if (m_psubmix)
        {
            XAUDIO2_SEND_DESCRIPTOR sendDesc = { 0, submix };
            XAUDIO2_VOICE_SENDS sendList = { 1, &sendDesc };
        }

        HRESULT hr = S_OK;
        if (FAILED(hr = CreateSourceVoice(wfx)))
        {
            MessageBoxW(NULL, L"Failed to create source voice", L"Audio Error", MB_ICONERROR);
            return hr;
        }
        if (FAILED(hr = SubmitAudioBufferToVoice(buffer)))
        {
            MessageBoxW(NULL, L"Failed to submit audio buffer to voice", L"Audio Error", MB_ICONERROR);
            return hr;
        }

        return S_OK;
    }

    /////////////////////////////////////
    /// @brief Creates a source voice for playback.
    /// @param wfx Pointer to wave format structure.
    /// @return HRESULT indicating success or failure.
    /// @note Uses XAudio2 to create the source voice.
    /////////////////////////////////////
    HRESULT Sound::CreateSourceVoice(WAVEFORMATEX* wfx)
    {
        HRESULT hr = S_OK;
        if (FAILED(hr = AudioPlayer::GetInstance()->GetXAudio2()->CreateSourceVoice(&m_pSourceVoice, wfx, 0, XAUDIO2_DEFAULT_FREQ_RATIO, this)))
        {
            MessageBoxW(NULL, L"Failed to create source voice", L"Audio Error", MB_ICONERROR);
            return hr;
        }
        return S_OK;
    }

    /////////////////////////////////////
    /// @brief Submits audio buffer to the source voice.
    /// @param buffer Pointer to audio buffer.
    /// @return HRESULT indicating success or failure.
    /// @note Requires valid source voice to be created first.
    /////////////////////////////////////
    HRESULT Sound::SubmitAudioBufferToVoice(XAUDIO2_BUFFER* buffer)
    {
        if (!m_pSourceVoice)
        {
            MessageBoxW(NULL, L"Source voice not initialized", L"Audio Error", MB_ICONERROR);
            return E_POINTER;
        }

        HRESULT hr = S_OK;
        if (FAILED(hr = m_pSourceVoice->SubmitSourceBuffer(buffer)))
        {
            return hr;
        }

        std::cout << "Submitting buffer: " << buffer->AudioBytes << " bytes, "
            << "First sample: " << *reinterpret_cast<const float*>(buffer->pAudioData) << std::endl;

        return S_OK;
    }

    /////////////////////////////////////
    /// @brief Saves the current sound state.
    /// @note Stops playback and refills the buffer.
    /////////////////////////////////////
    void Sound::SaveSound()
    {
        if (m_pSourceVoice)
        {
            m_pSourceVoice->Stop(0);
            FillBufferAgain();
        }
    }

    float32 Sound::GetVolume() const
    {
        return m_volume;
    }

    Category Sound::GetCategory() const
    {
        return m_category;
    }

    /////////////////////////////////////
    /// @brief Refills the audio buffer.
    /// @note Resubmits the buffer to the source voice.
    /////////////////////////////////////
    void Sound::FillBufferAgain()
    {
        if (m_pSourceVoice) {
            // Resoumettre le buffer original
            m_pSourceVoice->SubmitSourceBuffer(m_pBuffer);
        }
    }

    /////////////////////////////////////
    /// @brief Sets the current audio state.
    /// @param state New state to set.
    /////////////////////////////////////
    void Sound::SetState(AudioState state)
    {
        m_state = state;
    }

    /////////////////////////////////////
    /// @brief Called when audio stream ends.
    /// @note Updates state to STOP.
    /////////////////////////////////////
    void Sound::OnStreamEnd()
    {
        m_state = AudioState::STOP;
    }

    /////////////////////////////////////
    /// @brief Gets the source voice interface.
    /// @return Pointer to IXAudio2SourceVoice.
    /////////////////////////////////////
    IXAudio2SourceVoice* Sound::GetVoice() const
    {
        return m_pSourceVoice;
    }

    /////////////////////////////////////
    /// @brief Gets the sound ID.
    /// @return The sound ID.
    /////////////////////////////////////
    uint32 Sound::GetSoundID() const
    {
        return m_soundId;
    }

    /////////////////////////////////////
    /// @brief Gets the current audio state.
    /// @return Current AudioState value.
    /////////////////////////////////////
    AudioState Sound::GetState() const
    {
        return m_state;
    }
}