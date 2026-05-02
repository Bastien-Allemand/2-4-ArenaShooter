#include "Audio.h"
#include "AudioPlayer.h"

namespace gce
{
    namespace AudioUse
    {

        /////////////////////////////////////
        /// @brief Loads a sound into the audio engine.
        /// @param name Unique name of the sound.
        /// @param path Path to the audio file (UTF-16).
        /// @param preAllocatedInstances Number of pre-allocated instances.
        /// @param is3D Indicates whether the sound should be treated as 3D.
        /// @return true if the sound was successfully loaded.
        /////////////////////////////////////
        bool LoadSound(std::string const& name, char16 const* path, int32 preAllocatedInstances, bool is3D)
        {
            return SUCCEEDED(AudioPlayer::GetInstance()->GetAudioManager()->RegisterSoundFromPath(name, path, preAllocatedInstances, is3D));
        }

        /////////////////////////////////////
        /// @brief Unloads a sound from the audio engine.
        /// @param name Name of the sound to remove.
        /// @return true after stopping and unregistering the sound.
        /////////////////////////////////////
        bool UnloadSound(std::string const& name)
        {
            AudioPlayer::GetInstance()->StopSoundOf(name);
            AudioPlayer::GetInstance()->GetAudioManager()->UnregisterSound(name);
            return true;
        }

        /////////////////////////////////////
        /// @brief Plays a sound by its name.
        /// @param name Name of the sound to play.
        /// @param loop If true, loops the sound and sets it as background.
        /// @param listenerPosition 3D position of the listener (used if spatialization is enabled).
        /////////////////////////////////////
        void Play(std::string const& name, bool loop, Vector3f32 listenerPosition)
        {
            if (loop)
            {
                AudioPlayer::GetInstance()->MakeThisSongBackground(name);
            }
            AudioPlayer::GetInstance()->PlaySoundOf(name, listenerPosition);
        }

        /////////////////////////////////////
        /// @brief Pauses a currently playing sound.
        /// @param name Name of the sound to pause.
        /////////////////////////////////////
        void Pause(std::string const& name)
        {
            AudioPlayer::GetInstance()->PauseSoundOf(name);
        }

        /////////////////////////////////////
        /// @brief Resumes a previously paused sound.
        /// @param name Name of the sound to resume.
        /////////////////////////////////////
        void Resume(std::string const& name)
        {
            AudioPlayer::GetInstance()->ResumeSoundOf(name);
        }

        /////////////////////////////////////
        /// @brief Stops a sound immediately.
        /// @param name Name of the sound to stop.
        /////////////////////////////////////
        void Stop(std::string const& name)
        {
            AudioPlayer::GetInstance()->StopSoundOf(name);
        }

        /////////////////////////////////////
        /// @brief Sets the volume of a specific sound.
        /// @param name Name of the target sound.
        /// @param volume Volume from 0 to 100.
        /////////////////////////////////////
        void SetVolumeOfSound(std::string const& name, int32 volume)
        {
            AudioPlayer::GetInstance()->SetVolumeOf(name, volume);
        }

        /////////////////////////////////////
        /// @brief Sets the volume of a sound category.
        /// @param category The audio category.
        /// @param volume Volume from 0 to 100.
        /////////////////////////////////////
        void SetVolumeOfCategory(Category category, int32 volume)
        {
            AudioPlayer::GetInstance()->GetAudioManager()->GetCategoryByName(category).SetVolume(volume);
            AudioPlayer::GetInstance()->SetVolumeOfCategory(category, volume);
        }

        /////////////////////////////////////
        /// @brief Applies a global master volume to all sounds.
        /// @param volume Master volume from 0 to 100.
        /////////////////////////////////////
        void SetMasterVolume(int32 volume)
        {
            AudioPlayer::GetInstance()->SetMasterVolume(volume);
        }

        /////////////////////////////////////
        /// @brief Applies a reverb effect to a specific sound.
        /// @param name Name of the target sound.
        /////////////////////////////////////
        void ApplyReverb(std::string const& name)
        {
            AudioPlayer::GetInstance()->CreateReverbEffect(name);
        }

        /////////////////////////////////////
        /// @brief Applies a gain effect to a specific sound.
        /// @param name Name of the target sound.
        /// @param gainValue Linear gain multiplier (e.g. 1.0 = no change).
        /////////////////////////////////////
        void ApplyGain(std::string const& name, float gainValue)
        {
            AudioPlayer::GetInstance()->CreateGainEffect(name, gainValue);
        }

        /////////////////////////////////////
        /// @brief Applies a gain effect to a specific category.
        /// @param category Category of the target sound.
        /// @param gainValue Linear gain multiplier (e.g. 1.0 = no change).
        ////////////////////////////////////
        void ApplyGainCategory(Category category, float gainValue)
        {
            AudioPlayer::GetInstance()->GaintoCategory(category, gainValue);
        }

        /////////////////////////////////////
        /// @brief Assigns a category to a sound.
        /// @param name Name of the sound.
        /// @param category Category to assign.
        /////////////////////////////////////
        void SetAudioCategory(std::string const& name, Category category)
        {
            AudioPlayer::GetInstance()->GetAudioManager()->SetCategory(name, category);
        }

        /////////////////////////////////////
        /// @brief Updates the internal state of the audio engine.
        /// @param deltaTime Time elapsed since the last update (in seconds).
        /////////////////////////////////////
        void Update(float deltaTime)
        {
            AudioPlayer::GetInstance()->Update(deltaTime);
        }
    }
}
