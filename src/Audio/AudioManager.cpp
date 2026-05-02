#include "AudioManager.h"
#include "AudioPlayer.h"

namespace gce
{
    
    /////////////////////////////////////
    //@brief Constructs an AudioManager with empty Sound collections
    /////////////////////////////////////
    AudioManager::AudioManager()
    {
        m_sounds = {};
        m_soundsById = {};
        m_nextID = 1;
    }

    /////////////////////////////////////
    //@brief Destructor for AudioManager
    //@note Cleans up all registered Sound buffers
    /////////////////////////////////////
    AudioManager::~AudioManager()
    {
        for (auto it = m_sounds.begin(); it != m_sounds.end(); ++it)
        {
            delete it->second;
        }

        m_sounds.clear();
        m_soundsById.clear();
    }

    /////////////////////////////////////
    //@brief Initializes the audio manager
    //@return Always returns S_OK
    /////////////////////////////////////
    HRESULT AudioManager::Init()
    {
        for (size_t i = 0; i < m_categoryBuses.size(); ++i)
        {
            m_categoryBuses[i].Init(AudioPlayer::GetInstance()->GetXAudio2(), static_cast<Category>(i));
        }

        return S_OK;
    }

    /////////////////////////////////////
    //@brief Registers a new Sound from file
    //@param name Identifier for the Sound
    //@param path File path to audio resource
    //@param numberofpreSound Number of pre-allocated instances
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT AudioManager::RegisterSoundFromPath(std::string const name, const char16* path, int32 numberofpreSound , bool is3D)
    {
        HRESULT hr = S_OK;

        SoundBuffer* pSoundbuffer = new SoundBuffer();
        if (FAILED(hr = pSoundbuffer->Init(path, name, numberofpreSound, is3D)))
        {
            delete pSoundbuffer;
            return hr;
        }
        pSoundbuffer->SetID(m_nextID++);
        m_sounds[name] = pSoundbuffer;
        m_soundsById[pSoundbuffer->GetID()] = pSoundbuffer;

        return S_OK;
    }

    /////////////////////////////////////
    //@brief Gets a Sound buffer by name
    //@param name Identifier for the Sound
    //@return Pointer to SoundBuffer or nullptr if not found
    /////////////////////////////////////
    SoundBuffer* AudioManager::GetSoundBuffer(std::string const name)
    {
        auto it = m_sounds.find(name);
        if (it != m_sounds.end())
        {
            return it->second;
        }

        return nullptr;
    }

    /////////////////////////////////////
    //@brief Gets a Sound buffer by ID
    //@param id Unique identifier for the Sound
    //@return Pointer to SoundBuffer or nullptr if not found
    /////////////////////////////////////
    SoundBuffer* AudioManager::GetSoundBufferByID(uint32 id)
    {
        auto it = m_soundsById.find(id);
        if (it != m_soundsById.end())
        {
            return it->second;
        }

        return nullptr;
    }

    /////////////////////////////////////
    //@brief Gets all registered Sounds
    //@return Const reference to Sound map
    /////////////////////////////////////
    const UnorderedMap<std::string, SoundBuffer*>& AudioManager::GetAllSounds() const
    {
        return m_sounds;
    }

    /////////////////////////////////////
    //@brief Unregisters a Sound from file
    //@param name Identifier for the Sound
    //@return bool indicating success or failure
    /////////////////////////////////////
    bool AudioManager::UnregisterSound(std::string const& name)
    {
        auto nameIt = m_sounds.find(name);
        if (nameIt == m_sounds.end())
        {
            return false; 
        }

        SoundBuffer* pBuffer = nameIt->second;
        uint32 id = pBuffer->GetID();

        auto idIt = m_soundsById.find(id);
        if (idIt != m_soundsById.end())
        {
            m_soundsById.erase(idIt);
        }

        m_sounds.erase(nameIt);
        delete pBuffer;

        return true;
    }

    void AudioManager::SetCategory(std::string const& name, Category category)
    {
        auto it = m_sounds.find(name);
        if (it == m_sounds.end())
        {
            return;
        }

        SoundBuffer* buffer = it->second;
        if (!buffer)
        {
            return;
        }

        IXAudio2SubmixVoice* submix = m_categoryBuses[static_cast<size_t>(category)].GetSubmixVoice();
        float32 categoryVolume = m_categoryBuses[static_cast<size_t>(category)].GetVolume();
        buffer->SetUpCategory(category , submix , categoryVolume);
    }

    AudioCategory AudioManager::GetCategoryByName(Category category) const
    {
        return m_categoryBuses[static_cast<size_t>(category)];
    }
}
