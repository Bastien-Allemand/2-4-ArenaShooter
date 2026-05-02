#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

//If solution generator not update add additionalDependencies yourself 
//AUDIO -> Properties -> Library -> additionalDependencies
//mfplat.lib;mf.lib;mfreadwrite.lib;mfuuid.lib;

#include "define.h"
#include <string>
#include <Core/Maths/Vector3.h>
#include "AudioCategories.h"

namespace gce
{
    namespace AudioUse
    {

        // Sound management
        bool LoadSound(std::string const& name, char16 const* path, int32 preAllocatedInstances = 0 , bool is3D = false);
        bool UnloadSound(std::string const& name);

        // Playback control
        void Play(std::string const& name, bool loop = false , Vector3f32 listenerPosition = Vector3f32(0 , 0 , 0));
        void Pause(std::string const& name);
        void Resume(std::string const& name);
        void Stop(std::string const& name);

        // Volume control
        void SetVolumeOfSound(std::string const& name, int32 volume); 
        void SetMasterVolume(int32 volume); 
        void SetVolumeOfCategory(Category category, int32 volume);

        // Category
        void SetAudioCategory(std::string const& name, Category category = Category::NONE);

        // Effects
        void ApplyReverb(std::string const& name);
        void ApplyGain(std::string const& name, float gainValue);
        void ApplyGainCategory(Category category, float gainValue);

        // State management
        void Update(float deltaTime); 
    
    }
}

#endif
