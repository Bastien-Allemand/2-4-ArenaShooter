#ifndef AUDIO_SoundSource3D_H_INCLUDED
#define AUDIO_SoundSource3D_H_INCLUDED

#include "Sound.h"
#include <Core/Maths/Vector3.h>
#include <Core/Maths/MathsFunctions.hpp>

namespace gce
{
    class SoundSource3D : public Sound
    {
    public:

        virtual HRESULT Init(WAVEFORMATEX* wfx, XAUDIO2_BUFFER* buffer, uint32 const id , float gain
            , Category category, IXAudio2SubmixVoice* submix = nullptr) override;
        void Play() override;
        void Playing(float deltaTime = 0) override;
        void Pause() override;
        void Resume() override;
        void Stop() override;
        void SetVolume(float32 volume , float32 category = 1.0f) override;

        float DistanceTo() const;
        
        SoundSource3D();
        virtual ~SoundSource3D();
        void SetEmitterPosition(float x, float y, float z);
        void SetListenerPosition(float x, float y, float z);
        void Update3DAudio();
        void SetListenerForward(float x, float y, float z);
        void Delay(float deltaTime) override;

    private:

        void ApplyDistanceVolume();
        void ApplyDistanceDelay();
        void ApplyStereoPanning();
        void ApplyVelocity();

        void CircleAroundListener(float deltaTime);
        void InitializeOrbitFromCurrentPosition();

    private :

        Vector3f32 m_emitterPosition;
        Vector3f32 m_listenerPosition;
        Vector3f32 m_listenerForward;
        float m_minDistance;
        float m_maxDistance;

        float m_travelTime;      
        float m_elapsedSincePlay; 
        bool  m_waitingDelay;   
        
        Vector3f32 m_velocity;
        Vector3f32 m_orbitCenter;
        float m_orbitRadius;
        float m_orbitAngle;
        float m_orbitSpeed;
    };
}

#endif