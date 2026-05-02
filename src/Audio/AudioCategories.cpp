#include "AudioCategories.h"
#include <define.h>
#include <Core/Maths/MathsFunctions.hpp>
#include "Helper.h"

namespace gce
{
    /////////////////////////////////////
    /// @brief Initializes an audio category (Music, Effects, Dialogue, etc.)
    /// @param xaudio Pointer to the XAudio2 engine instance.
    /// @param type The category type (enum Category).
    /// @param isMono Indicates whether the output is mono (1 channel) or stereo (2 channels).
    /////////////////////////////////////
    void AudioCategory::Init(IXAudio2* xaudio, Category type, bool isMono)
    {
        m_type = type;
        uint32 channels = (isMono = true) ? 1 : 2;
        xaudio->CreateSubmixVoice(&m_pSubmix, channels, 44100);
    }

    /////////////////////////////////////
    /// @brief Sets the volume of the audio category.
    /// @param volume User volume value (typically between 0 and 100).
    /////////////////////////////////////
    void AudioCategory::SetVolume(int32 volume)
    {
        m_volume = ConvertUserVolumeToActual(volume);
        if (m_pSubmix)
        {
            m_pSubmix->SetVolume(m_muted ? 0.0f : m_volume);
        }
    }

    /////////////////////////////////////
    /// @brief Mutes or unmutes this audio category.
    /// @param isMuted true to mute, false to unmute.
    /////////////////////////////////////
    void AudioCategory::Mute(bool isMuted)
    {
        m_muted = isMuted;
        if (m_pSubmix)
        {
            m_pSubmix->SetVolume(m_muted ? 0.0f : m_volume);
        }
    }

    /////////////////////////////////////
    /// @brief Retrieves the current effective volume.
    /// @return Volume as a float between 0.0f and 1.0f.
    /////////////////////////////////////
    float AudioCategory::GetVolume() const
    {
        return m_volume;
    }

    /////////////////////////////////////
    /// @brief Checks if the category is currently muted.
    /// @return true if muted, false otherwise.
    /////////////////////////////////////
    bool AudioCategory::IsMuted() const
    {
        return m_muted;
    }

    /////////////////////////////////////
    /// @brief Returns the submix voice associated with this category.
    /// @return Pointer to IXAudio2SubmixVoice.
    /////////////////////////////////////
    IXAudio2SubmixVoice* AudioCategory::GetSubmixVoice() const
    {
        return m_pSubmix;
    }
}
