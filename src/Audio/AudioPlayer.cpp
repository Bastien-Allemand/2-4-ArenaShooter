#include "AudioPlayer.h"

namespace gce
{
    AudioPlayer* AudioPlayer::instancePtr = nullptr;

    /////////////////////////////////////
    //@brief Gets the singleton instance of AudioPlayer
    //@return Pointer to AudioPlayer instance
    /////////////////////////////////////
    AudioPlayer* AudioPlayer::GetInstance()
    {
        if (instancePtr == nullptr)
        {
            instancePtr = new AudioPlayer();
            instancePtr->Init();
        }
        return instancePtr;
    }

    /////////////////////////////////////
    //@brief Destroys the singleton instance
    /////////////////////////////////////
    void AudioPlayer::DestroyInstance()
    {
        if (instancePtr != nullptr)
        {
            delete instancePtr;
            instancePtr = nullptr;
        }
    }

    /////////////////////////////////////
    //@brief Constructs an AudioPlayer with default values
    /////////////////////////////////////
    AudioPlayer::AudioPlayer()
    {
        m_pXAudio2 = nullptr;
        m_pMasteringVoice = nullptr;
        m_pAudioManager = nullptr;
        m_masterVolume = 1;
        instancePtr = nullptr;
    }

    /////////////////////////////////////
    //@brief Destructor for AudioPlayer
    //@note Cleans up all XAudio2 resources and active Sounds
    /////////////////////////////////////
    AudioPlayer::~AudioPlayer()
    {
        for (auto soundSource2D : m_activeVoices)
        {
            delete soundSource2D;
        }
        m_activeVoices.Clear();

        if (m_pMasteringVoice)
        {
            m_pMasteringVoice->DestroyVoice();
            m_pMasteringVoice = nullptr;
        }

        if (m_pXAudio2)
        {
            m_pXAudio2->Release();
            m_pXAudio2 = nullptr;
        }

        if (m_pAudioManager)
        {
            delete m_pAudioManager;
            m_pAudioManager = nullptr;
        }
    }

    /////////////////////////////////////
    //@brief Initializes the audio system
    //@return HRESULT indicating success or failure
    //@note Initializes COM, XAudio2, and creates mastering voice
    /////////////////////////////////////
    HRESULT AudioPlayer::Init()
    {
        HRESULT hr = S_OK;

        if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)))
        {
            if (hr == 0x80010106)
            {
                CoUninitialize();
                hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
            }
            else
            {
                MessageBoxW(NULL, L"Failed to initilialize com", L"Audio Error", MB_ICONERROR);
                return hr;
            }
        }

        if (FAILED(hr = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
        {
            MessageBoxW(NULL, L"Failed to initilialize Xaudio2", L"Audio Error", MB_ICONERROR);
            return hr;
        }

        if (FAILED(hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice)))
        {
            MessageBoxW(NULL, L"Failed to create mastering voice", L"Audio Error", MB_ICONERROR);
            return hr;
        }

        m_pAudioManager = new AudioManager();
        if (FAILED(hr = m_pAudioManager->Init()))
        {
            MessageBoxW(NULL, L"Failed to initilialize audio manager", L"Audio Error", MB_ICONERROR);
            return hr;
        }

        return S_OK;
    }

    /////////////////////////////////////
    //@brief Creates pre-allocated Sound instances
    //@note Creates the specified number of instances for each Sound
    /////////////////////////////////////
    void AudioPlayer::CreatePreSound()
    {
        for (auto& soundPair : m_pAudioManager->GetAllSounds())
        {
            SoundBuffer* pBuffer = soundPair.second;
            int32 preSoundCount = pBuffer->GetNbrOfPreSound();
            if (preSoundCount > 0)
            {
                for (int32 i = 0; i < preSoundCount; ++i)
                {
                    Sound* pPreSound = pBuffer->CreateSound();
                    if (pPreSound)
                    {
                        m_activeVoices.PushBack(pPreSound);
                    }
                }
            }
        }
    }

    /////////////////////////////////////
    //@brief Updates the audio system state
    //@note Manages all active voices (playing, stopping, pausing)
    /////////////////////////////////////
    void AudioPlayer::Update(float deltaTime)
    {
        for (auto it = m_activeVoices.begin(); it != m_activeVoices.end();)
        {
            Sound* pSound = *it;
            AudioState state = pSound->GetState();
            switch (state)
            {
            case AudioState::STOP:
            {
                SoundBuffer* pBuffer = m_pAudioManager->GetSoundBufferByID(pSound->GetSoundID());
                if (pBuffer->GetIsBackgroundSound())
                {
                    pSound->FillBufferAgain();
                }
                else
                {
                    int32 inActiveVoice = 0;
                    for (auto& s : m_activeVoices)
                    {
                        if (s->GetSoundID() == pSound->GetSoundID())
                        {
                            inActiveVoice++;
                        }
                    }

                    if (inActiveVoice > pBuffer->GetNbrOfPreSound())
                    {
                        delete pSound;
                        it = m_activeVoices.Erase(it);
                    }
                    else
                    {
                        pSound->SaveSound();
                        ++it;
                    }
                }
                break;
            }

            case AudioState::START:
            {
                pSound->Play();
                ++it;
                break;
            }

            case AudioState::PLAYING:
            {
                pSound->Playing();
                ++it;
                break;
            }
            case AudioState::PAUSE:
            {
                pSound->Pause();
                ++it;
                break;
            }

            case AudioState::RESUME:
            {
                pSound->Resume();
                ++it;
                break;
            }

            case AudioState::DELAY:
            {
                pSound->Delay(deltaTime);
                ++it;
                break;
            }

            }
        }
    }

    /////////////////////////////////////
    //@brief Plays a Sound by name
    //@param name Identifier of the Sound to play
    /////////////////////////////////////
    void AudioPlayer::PlaySoundOf(std::string const name , Vector3f32 listenerPosition)
    {
        SoundBuffer* pBuffer = m_pAudioManager->GetSoundBuffer(name);
        if (!pBuffer) return;

        uint32 soundId = pBuffer->GetID();

        for (auto& sound : m_activeVoices)
        {
            if (sound->GetSoundID() == soundId && sound->GetState() == AudioState::STOP)
            {
                sound->SetState(AudioState::START);
                if (pBuffer->GetIs3DSound())
                {
                    SoundSource3D* pSound3D = dynamic_cast<SoundSource3D*>(sound);
                    pSound3D->SetListenerPosition(listenerPosition.x, listenerPosition.y, listenerPosition.z);
                    pSound3D->SetListenerForward(0, 0, 1);
                }
                sound->Play();
                return;
            }
        }

        Sound* pNewSound = pBuffer->CreateSound();
        if (pNewSound)
        {
            pNewSound->SetState(AudioState::PLAYING);
            if (pBuffer->GetIs3DSound())
            {
                SoundSource3D* pSound3D = dynamic_cast<SoundSource3D*>(pNewSound);
                pSound3D->SetListenerPosition(listenerPosition.x, listenerPosition.y, listenerPosition.z);
                pSound3D->SetListenerForward(0, 0, 1);
            }
            pNewSound->Play();
            m_activeVoices.PushBack(pNewSound);
        }
    }

    /////////////////////////////////////
    //@brief Pauses all active Sounds
    /////////////////////////////////////
    void AudioPlayer::PauseSound()
    {
        for (auto& sound : m_activeVoices)
        {
            sound->SetState(AudioState::PAUSE);
        }
    }

    /////////////////////////////////////
    //@brief Resumes all paused Sounds
    /////////////////////////////////////
    void AudioPlayer::ResumeSound()
    {
        for (auto& sound : m_activeVoices)
        {
            if (sound->GetState() == AudioState::PAUSE)
            {
                sound->SetState(AudioState::RESUME);
            }
        }
    }

    /////////////////////////////////////
    //@brief Pauses a specific Sound by name
    //@param name Identifier of the Sound to pause
    /////////////////////////////////////
    void AudioPlayer::PauseSoundOf(std::string const name)
    {
        SoundBuffer* pBuffer = m_pAudioManager->GetSoundBuffer(name);
        if (!pBuffer) return;

        uint32 soundId = pBuffer->GetID();

        for (auto& sound : m_activeVoices)
        {
            if (sound->GetSoundID() == soundId && sound->GetState() == AudioState::PLAYING)
            {
                sound->SetState(AudioState::PAUSE);
                return;
            }
        }
    }

    /////////////////////////////////////
    //@brief Resumes a specific paused Sound by name
    //@param name Identifier of the Sound to resume
    /////////////////////////////////////
    void AudioPlayer::ResumeSoundOf(std::string const name)
    {
        SoundBuffer* pBuffer = m_pAudioManager->GetSoundBuffer(name);
        if (!pBuffer) return;

        uint32 soundId = pBuffer->GetID();
            
        for (auto& sound : m_activeVoices)
        {
            if (sound->GetSoundID() == soundId && sound->GetState() == AudioState::PAUSE)
            {
                sound->SetState(AudioState::RESUME);
                return;
            }
        }
    }

    /////////////////////////////////////
    //@brief Stops and removes all active Sounds
    /////////////////////////////////////
    void AudioPlayer::StopSound()
    {
        for (auto it = m_activeVoices.begin(); it != m_activeVoices.end();)
        {
            Sound* pSound = *it;
            delete pSound;
            it = m_activeVoices.Erase(it);
        }
    }

    /////////////////////////////////////
    //@brief Stops and removes sound
    //@param name of the song to stop
    /////////////////////////////////////
    void AudioPlayer::StopSoundOf(std::string const name)
    {
        auto it = std::remove_if(m_activeVoices.begin(), m_activeVoices.end(), [&](Sound* pVoice) 
            {
                if (pVoice->GetSoundID() == m_pAudioManager->GetSoundBuffer(name)->GetID()) 
                {
                    delete pVoice;
                    return true;
                }
                return false;
            });
        m_activeVoices.Erase(it, m_activeVoices.end());
    }

    /////////////////////////////////////
    //@brief Sets volume for all instances of a specific Sound
    //@param name Identifier of the Sound
    //@param volume New volume level (0-100)
    /////////////////////////////////////
    void AudioPlayer::SetVolumeOf(std::string const name, int32 volume)
    {
        SoundBuffer* pBuffer = m_pAudioManager->GetSoundBuffer(name);
        pBuffer->SetVolume(volume);
        if (!pBuffer)
        {
            return;
        }
        uint32 soundId = pBuffer->GetID();
        for (auto& sound : m_activeVoices)
        {
            if (sound->GetSoundID() == soundId)
            {
                sound->SetVolume(pBuffer->GetVolume());
            }
        }
    }

    /////////////////////////////////////
    //@brief Sets volume for all instances of a specific Category
    //@param category 
    //@param volume New volume level (0-100)
    /////////////////////////////////////
    void AudioPlayer::SetVolumeOfCategory(Category category, int32 volume)
    {
        for (auto& sound : m_activeVoices)
        {
            if (sound->GetCategory() == category)
            {
                float32 volumeToSend = ConvertUserVolumeToActual(volume);
                sound->SetVolume(sound->GetVolume(), volumeToSend);
            }
        }
    }

    /////////////////////////////////////
    //@brief Sets the master volume for all audio output
    //@param newVolume Volume level (0-100)
    /////////////////////////////////////
    void AudioPlayer::SetMasterVolume(int32 newVolume)
    {
        m_masterVolume = ConvertUserVolumeToActual(newVolume);
        if (m_pMasteringVoice)
        {
            m_pMasteringVoice->SetVolume(m_masterVolume);
        }
    }

    /////////////////////////////////////
    //@brief Gets the audio manager instance
    //@return Pointer to AudioManager
    /////////////////////////////////////
    AudioManager* AudioPlayer::GetAudioManager() const
    {
        return m_pAudioManager;
    }

    /////////////////////////////////////
    //@brief Gets the XAudio2 engine interface
    //@return Pointer to IXAudio2
    /////////////////////////////////////
    IXAudio2* AudioPlayer::GetXAudio2() const
    {
        return m_pXAudio2;
    }

    /////////////////////////////////////
    //@brief Marks a Sound as background/looping
    //@param name Identifier of the Sound
    /////////////////////////////////////
    void AudioPlayer::MakeThisSongBackground(std::string const name)
    {
        SoundBuffer* pBuffer = m_pAudioManager->GetSoundBuffer(name);
        if (pBuffer)
        {
            pBuffer->MakeItBackgroundSound();
        }
    }

    /////////////////////////////////////
    //@brief Gets the XAudio2 mastering voice interface
    //@return Pointer to IXAudio2MasteringVoice
    /////////////////////////////////////
    IXAudio2MasteringVoice* AudioPlayer::GetMasteringVoice() const
    {
        return m_pMasteringVoice;
    }

    /////////////////////////////////////
    //@brief Add an effect to a sound (reverb)
    //@param Name of the song
    /////////////////////////////////////
    void AudioPlayer::CreateReverbEffect(const std::string& name)
    {
        SoundBuffer* pBuffer = m_pAudioManager->GetSoundBuffer(name);
        if (!pBuffer)
        {
            return;
        } 
        SoundEffect* pEffect = pBuffer->GetEffect();
        pEffect->AddReverb();
        uint32 soundId = pBuffer->GetID();
        for (auto& SoundSource2D : m_activeVoices)
        {
            if (SoundSource2D->GetSoundID() == soundId)
            {
                pEffect->ApplyToSourceVoice(SoundSource2D->GetVoice());
            }
        }
    }

    void AudioPlayer::CreateGainEffect(const std::string& name, float gainValue)
    {
        SoundBuffer* buffer = m_pAudioManager->GetSoundBuffer(name);
        if (!buffer) return;

        SoundEffect* effect = buffer->GetEffect();
        effect->AddGain(gainValue);

        for (auto& source : m_activeVoices) {
            if (source->GetSoundID() == buffer->GetID()) {
                // 1. Appliquer les effets
                effect->ApplyToSourceVoice(source->GetVoice());

                // 2. Forcer un recalcul des buffers
                XAUDIO2_BUFFER audioBuffer = buffer->GetBuffer(); // Stocker la copie
                source->GetVoice()->FlushSourceBuffers();
                source->SubmitAudioBufferToVoice(&audioBuffer); 

                // 3. Red�marrer si n�cessaire
                if (source->GetState() == AudioState::PLAYING) {
                    source->GetVoice()->Start();
                }
            }
        }
    }

    void AudioPlayer::GaintoCategory(Category category, float gainValue)
    {
        for (auto& sound : m_activeVoices)
        {
            if (sound->GetCategory() == category)
            {
                SoundBuffer* buffer = m_pAudioManager->GetSoundBufferByID(sound->GetSoundID());
                if (!buffer) return;

                SoundEffect* effect = buffer->GetEffect();
                effect->AddGain(gainValue);

                for (auto& source : m_activeVoices) {
                    if (source->GetSoundID() == buffer->GetID()) {
                        // 1. Appliquer les effets
                        effect->ApplyToSourceVoice(source->GetVoice());

                        // 2. Forcer un recalcul des buffers
                        XAUDIO2_BUFFER audioBuffer = buffer->GetBuffer(); // Stocker la copie
                        source->GetVoice()->FlushSourceBuffers();
                        source->SubmitAudioBufferToVoice(&audioBuffer);

                        // 3. Red�marrer si n�cessaire
                        if (source->GetState() == AudioState::PLAYING) {
                            source->GetVoice()->Start();
                        }
                    }
                }
            }
        }
    }

}