#ifndef AUDIO_AUDIOPLAYER_H_INCLUDED
#define AUDIO_AUDIOPLAYER_H_INCLUDED

#include <Containers/Vector.hpp>
#include "define.h"
#include <comdef.h>
#include "Helper.h"
#include "AudioManager.h"

namespace gce
{
	class AudioPlayer
	{
	private:

	static AudioPlayer* instancePtr;
	AudioPlayer();
	virtual ~AudioPlayer();

	public:

	AudioPlayer(AudioPlayer const& obj) = delete;
	static AudioPlayer* GetInstance();
	static void DestroyInstance();
	HRESULT Init();
	void CreatePreSound();
	void Update(float deltaTime);
	void PlaySoundOf(std::string const name , Vector3f32 listenerPosition = Vector3f32(0, 0, 0));
	void PauseSound();
	void PauseSoundOf(std::string const name);
	void ResumeSound();
	void ResumeSoundOf(std::string const name);
	void StopSound();
	void StopSoundOf(std::string const name);
	void SetVolumeOf(std::string const name, int32 volume);
	void SetMasterVolume(int32 newVolume);
	AudioManager* GetAudioManager() const;
	IXAudio2* GetXAudio2() const;
	void MakeThisSongBackground(std::string const name);
	IXAudio2MasteringVoice* GetMasteringVoice() const;
	void SetVolumeOfCategory(Category category, int32 volume);

	//Effect 
	void CreateReverbEffect(const std::string& name);
	void CreateGainEffect(const std::string& name, float gainValue);
	void GaintoCategory(Category category, float gainValue);

	private:

	IXAudio2MasteringVoice* m_pMasteringVoice;
	AudioManager* m_pAudioManager;
	float32 m_masterVolume;
	Vector<Sound*> m_activeVoices;
	IXAudio2* m_pXAudio2;
	};
}

#endif