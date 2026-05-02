#include "SoundSource3D.h"
#include "AudioPlayer.h"
#include <string>
#include <iostream>

namespace gce
{
    /////////////////////////////////////
    /// @brief Default constructor.
    /// @note Initializes all 3D audio properties.
    /////////////////////////////////////
    SoundSource3D::SoundSource3D() : Sound()
    {
        m_emitterPosition = {};
        m_listenerPosition = {};
        m_listenerForward = {};
        m_minDistance = 0.0f;
        m_maxDistance = 0.0f;
        m_volume = 1.0f;
        m_travelTime = 0.0f;
        m_elapsedSincePlay = 0.0f;
        m_waitingDelay = false;

        m_velocity = {};
        m_orbitCenter = {};
        m_orbitRadius = 0.0f;
        m_orbitAngle = 0.0f;
        m_orbitSpeed = 1.0f;
    }

    /////////////////////////////////////
    /// @brief Destructor.
    /// @note Cleans up resources.
    /////////////////////////////////////
    SoundSource3D::~SoundSource3D()
    {
    }

    /////////////////////////////////////
    /// @brief Initializes the 3D sound source.
    /// @param wfx Pointer to wave format structure.
    /// @param buffer Pointer to audio buffer.
    /// @param id Unique sound ID.
    /// @return HRESULT indicating success or failure.
    /// @note Sets default distance values.
    /////////////////////////////////////
    HRESULT SoundSource3D::Init(WAVEFORMATEX* wfx, XAUDIO2_BUFFER* buffer, uint32 const id , float gain
        , Category category, IXAudio2SubmixVoice* submix)
    {
        Sound::Init(wfx, buffer, id , gain , category , submix);
        m_minDistance = 0.0f;
        m_maxDistance = 80.0f;
        return S_OK;
    }

    /////////////////////////////////////
    /// @brief Starts 3D playback.
    /// @note Initializes orbit and sets DELAY state.
    /////////////////////////////////////
    void SoundSource3D::Play()
    {
        SetEmitterPosition(-50.0f, 0.0f, 0.0f);
        InitializeOrbitFromCurrentPosition();
        Update3DAudio();
        m_elapsedSincePlay = 0.0f;
        m_waitingDelay = true;
        SetState(AudioState::DELAY);
    }

    /////////////////////////////////////
    /// @brief Updates 3D audio during playback.
    /// @param deltaTime Time since last update.
    /// @note Handles orbiting and updates 3D effects.
    /////////////////////////////////////
    void SoundSource3D::Playing(float deltaTime)
    {
        CircleAroundListener(deltaTime);
        Update3DAudio();
    }

    /////////////////////////////////////
    /// @brief Pauses 3D playback.
    /// @note Only pauses if currently playing.
    /////////////////////////////////////
    void SoundSource3D::Pause()
    {
        if (m_pSourceVoice)
        {
            m_pSourceVoice->Stop(0);
            SetState(AudioState::PAUSE);
        }
    }

    /////////////////////////////////////
    /// @brief Resumes 3D playback.
    /// @note Only resumes if currently paused.
    /////////////////////////////////////
    void SoundSource3D::Resume()
    {
        if (m_pSourceVoice)
        {
            m_pSourceVoice->Start(0);
            SetState(AudioState::PLAYING);
        }
    }

    /////////////////////////////////////
    /// @brief Stops 3D playback.
    /// @note Sets state to STOP.
    /////////////////////////////////////
    void SoundSource3D::Stop()
    {
        if (m_pSourceVoice)
        {
            m_pSourceVoice->Stop(0);
            SetState(AudioState::STOP);
        }
    }

    /////////////////////////////////////
    /// @brief Sets 3D sound volume.
    /// @param volume Volume level (0.0-1.0).
    /// @note Updates both internal volume and source voice.
    /////////////////////////////////////
    void SoundSource3D::SetVolume(float32 volume , float32 category)
    {
        m_volume = volume;
        if (m_pSourceVoice)
        {
            m_pSourceVoice->SetVolume(category * volume * m_gain);
        }
    }

    /////////////////////////////////////
    /// @brief Handles delay before playback starts.
    /// @param deltaTime Time since last update.
    /// @note Manages sound travel time simulation.
    /////////////////////////////////////
    void SoundSource3D::Delay(float deltaTime)
    {
        if (m_waitingDelay)
        {
            m_elapsedSincePlay += deltaTime;
            if (m_elapsedSincePlay >= m_travelTime)
            {
                m_pSourceVoice->Start(0);
                m_waitingDelay = false;
                SetState(AudioState::PLAYING);
            }
        }
    }

    /////////////////////////////////////
    /// @brief Sets emitter position.
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param z Z coordinate.
    /////////////////////////////////////
    void SoundSource3D::SetEmitterPosition(float x, float y, float z)
    {
        m_emitterPosition = { x, y, z };
    }

    /////////////////////////////////////
    /// @brief Sets listener position.
    /// @param x X coordinate.
    /// @param y Y coordinate.
    /// @param z Z coordinate.
    /////////////////////////////////////
    void SoundSource3D::SetListenerPosition(float x, float y, float z)
    {
        m_listenerPosition = { x, y, z };
    }

    /////////////////////////////////////
    /// @brief Sets listener forward vector.
    /// @param x X component.
    /// @param y Y component.
    /// @param z Z component.
    /// @note Normalizes the vector.
    /////////////////////////////////////
    void SoundSource3D::SetListenerForward(float x, float y, float z)
    {
        m_listenerForward = Vector3f32(x, y, z).Normalize();
    }

    /////////////////////////////////////
    /// @brief Updates all 3D audio effects.
    /// @note Applies distance, panning and Doppler effects.
    /////////////////////////////////////
    void SoundSource3D::Update3DAudio()
    {
        ApplyDistanceVolume();
        ApplyDistanceDelay();
        ApplyStereoPanning();
        ApplyVelocity();
    }

    /////////////////////////////////////
    /// @brief Calculates distance to listener.
    /// @return Distance in units.
    /////////////////////////////////////
    float SoundSource3D::DistanceTo() const
    {
        float dx = m_emitterPosition.x - m_listenerPosition.x;
        float dy = m_emitterPosition.y - m_listenerPosition.y;
        float dz = m_emitterPosition.z - m_listenerPosition.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    /////////////////////////////////////
    /// @brief Applies volume based on distance.
    /// @note Uses inverse square law attenuation.
    /////////////////////////////////////
    void SoundSource3D::ApplyDistanceVolume()
    {
        if (!m_pSourceVoice)
            return;

        float distance = DistanceTo();
        float t = (distance - m_minDistance) / (m_maxDistance - m_minDistance);
        float rolloff = 16.0f;
        float attenuation = 1.0f / (1.0f + t * t * rolloff);
        float finalVolume = Clamp(attenuation * (m_volume), 0.0f, 1.0f);

        m_pSourceVoice->SetVolume(finalVolume);
    }

    /////////////////////////////////////
    /// @brief Calculates sound travel time.
    /// @note Based on distance and speed of sound.
    /////////////////////////////////////
    void SoundSource3D::ApplyDistanceDelay()
    {
        if (!m_pSourceVoice)
            return;

        const float speedOfSound = 343.0f;
        m_travelTime = DistanceTo() / speedOfSound;
    }

    /////////////////////////////////////
    /// @brief Applies stereo panning effect.
    /// @note Based on emitter position relative to listener.
    /////////////////////////////////////
    void SoundSource3D::ApplyStereoPanning()
    {
        if (!m_pSourceVoice)
        {
            return;
        }
            
        Vector3f32 toEmitter = (m_emitterPosition - m_listenerPosition).Normalize(); //vecteur directeur 
        Vector3f32 up = { 0.0f, 1.0f, 0.0f };
        Vector3f32 right = up.CrossProduct(m_listenerForward).Normalize(); //répere spatial par rapport au joueur

        float pan = Clamp(toEmitter.DotProduct(right), -1.0f, 1.0f); // on essaye de savoir si c'est a droite ou gauche en poucentage 
        float angle = (pan + 1.0f) * (PI * 0.25f); // on passe de panoramique à angle 

        float leftVolume = Cos(angle);
        float rightVolume = Sin(angle);

        XAUDIO2_VOICE_DETAILS details;
        m_pSourceVoice->GetVoiceDetails(&details);

        if (details.InputChannels == 1)
        {
            float matrix[2] = { leftVolume, rightVolume };
            m_pSourceVoice->SetOutputMatrix(nullptr, 1, 2, matrix);
        }
    }

    /////////////////////////////////////
    /// @brief Applies Doppler effect.
    /// @note Based on relative velocity between emitter and listener.
    /////////////////////////////////////
    void SoundSource3D::ApplyVelocity()
    {
        float speedOfSound = 343.0f;

        Vector3f32 dir = m_emitterPosition - m_listenerPosition;
        float distance = dir.Norm();
        Vector3f32 normDir = dir / distance;

        float vEmitter = m_velocity.DotProduct(normDir);
        float vListener = 0.0f;

        float doppler = (speedOfSound + vListener) / (speedOfSound + vEmitter);
        doppler = Clamp(doppler, 0.5f, 2.0f);

        m_pSourceVoice->SetFrequencyRatio(doppler);
    }

    /////////////////////////////////////
    /// @brief Initializes orbit parameters.
    /// @note Calculates initial orbit radius and angle.
    /////////////////////////////////////
    void SoundSource3D::InitializeOrbitFromCurrentPosition()
    {
        m_orbitCenter = m_listenerPosition;

        Vector3f32 relative = m_emitterPosition - m_orbitCenter;
        m_orbitRadius = relative.Norm();

        if (m_orbitRadius > 0.0001f)
        {
            m_orbitAngle = atan2(relative.z, relative.x);
        }
        else
        {
            m_orbitAngle = 0.0f;
            m_orbitRadius = 1.0f;
        }
    }

    /////////////////////////////////////
    /// @brief Updates emitter orbit around listener.
    /// @param deltaTime Time since last update.
    /// @note Calculates new position and velocity.
    /////////////////////////////////////
    void SoundSource3D::CircleAroundListener(float deltaTime)
    {
        const float minDelta = 0.001f;
        if (deltaTime < minDelta) 
        {
            deltaTime = minDelta;
        }

        m_orbitSpeed = 5.0f;

        m_orbitAngle += deltaTime * m_orbitSpeed;

        float x = cos(m_orbitAngle) * m_orbitRadius;
        float z = sin(m_orbitAngle) * m_orbitRadius;

        std::wcout << L"New Position: (" << x << L", 0, " << z << L")\n";

        Vector3f32 newPosition = m_orbitCenter + Vector3f32(x, 0.0f, z);
        SetEmitterPosition(newPosition.x, newPosition.y, newPosition.z);

        float dx = -sin(m_orbitAngle) * m_orbitRadius * m_orbitSpeed;
        float dz = cos(m_orbitAngle) * m_orbitRadius * m_orbitSpeed;
        m_velocity = Vector3f32(dx, 0.0f, dz);

        std::wcout << L"Current Angle: " << m_orbitAngle << L" radians\n";
    }
}
