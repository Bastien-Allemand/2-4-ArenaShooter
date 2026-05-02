#ifndef AUDIO_SoundBUFFER_H_INCLUDED
#define AUDIO_SoundBUFFER_H_INCLUDED

#include "Helper.h"
#include <Core/define.h>
#include <Core/Maths/Vector3.h>
#include "SoundSource2D.h"
#include "SoundSource3D.h"
#include "SoundEffect.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>

#include "AudioCategories.h"

class Sound;

namespace gce
{
    class SoundBuffer
    {
    public:

        SoundBuffer();
        virtual ~SoundBuffer();
        HRESULT Init(char16 const* strFileName, std::string const name ,int32 numberOfPresound = 0 , bool is3D = false);
        Sound* CreateSound();
        WAVEFORMATEX const& Getwfx() const;
        XAUDIO2_BUFFER GetBuffer() const;
        std::string GetName() const;
        int32 GetNbrOfPreSound() const;
        uint32 GetID() const;
        void SetID(uint32 id);
        void SetVolume(int32 volume);
        float32 GetVolume() const;
        void MakeItBackgroundSound();
        bool GetIsBackgroundSound() const;
        SoundEffect* GetEffect();
        bool GetIs3DSound() const;
        float ComputeNormalizationFactor(float targetDbFS = -20);

        void SetUpCategory(Category cateogry , IXAudio2SubmixVoice* submix , float32 categoryVolume);

    private:

        bool IsMP3(char16 const* filePath);
        HRESULT OpenMP3File(const wchar_t* filePath);
        HRESULT ConfigureAudioStream(IMFSourceReader* pReader, IMFMediaType** ppPCMAudio);
        HRESULT WriteWaveHeader(HANDLE hFile, IMFMediaType* pWaveFormat, DWORD* pcbWritten);
        HRESULT WriteWaveFile(IMFSourceReader* pReader, HANDLE hFile, LONG msecAudioData);
        HRESULT WriteWaveData(HANDLE hFile, IMFSourceReader* pReader, DWORD cbMaxAudioData, DWORD* pcbAudioData);
        HRESULT FixUpChunkSizes(HANDLE hFile, DWORD cbHeader, DWORD cbAudioData);
        HRESULT OpenAudioFile(char16 const* strFileName);
        HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
        HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
        HRESULT LocateRiffSegment(HANDLE hFile);
        void LocateFMTSegmentAndfillWaveStruct(HANDLE hFile, DWORD dwChunkSize, DWORD dwChunkPosition);
        void LocateData(HANDLE hFile, DWORD dwChunkSize, DWORD dwChunkPosition);
        void FillAudioBuffer(DWORD dwChunkSize, BYTE* pDataBuffer);

    private:

        WAVEFORMATEX m_wfx;
        XAUDIO2_BUFFER m_buffer;
        std::string m_name;
        int32 m_numberOfPresound;
        uint32 m_id;
        float32 m_volume;
        bool m_isBackgroundSound;
        SoundEffect* m_pSoundEffect;
        bool m_is3D;
        float m_normalizationGain;  
        Category m_category;
        float32 m_categoryVolume;
        IXAudio2SubmixVoice* m_psubmix;

    };
}

#endif