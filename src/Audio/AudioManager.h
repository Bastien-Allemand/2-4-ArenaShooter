#ifndef AUDIO_AUDIOMANAGER_H_INCLUDED
#define AUDIO_AUDIOMANAGER_H_INCLUDED

#include <array>
#include "define.h"
#include "Soundbuffer.h"
#include "AudioCategories.h"

namespace gce
{
    class AudioManager
    {
    public:

        AudioManager();
        virtual ~AudioManager();
        HRESULT Init();
        HRESULT RegisterSoundFromPath(std::string const name, char16 const* path, int32 numberofpreSound = 0 , bool is3D = false);
        SoundBuffer* GetSoundBufferByID(uint32 id);
        SoundBuffer* GetSoundBuffer(std::string const name);
        const UnorderedMap<std::string, SoundBuffer*>& GetAllSounds() const;
        bool UnregisterSound(std::string const& name);

        void SetCategory(std::string const& name, Category category);
        AudioCategory GetCategoryByName(Category category) const;

    private:

        UnorderedMap<std::string, SoundBuffer*> m_sounds;
        UnorderedMap<uint32, SoundBuffer*> m_soundsById;
        uint32 m_nextID;
        std::array<AudioCategory, static_cast<size_t>(Category::COUNT)> m_categoryBuses;
    };
}

#endif