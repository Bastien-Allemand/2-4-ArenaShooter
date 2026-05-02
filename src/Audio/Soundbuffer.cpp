#include "Soundbuffer.h"
#include "Helper.h"

namespace gce
{
    // FourCC codes for WAV file chunks
    constexpr uint32 fourcc_RIFF = 0x46464952; // 'RIFF' 
    constexpr uint32 fourcc_WAVE = 0x45564157; // 'WAVE'
    constexpr uint32 fourcc_FMT  = 0x20746D66; // 'fmt ' 
    constexpr uint32 fourcc_DATA = 0x61746164; // 'data' 

    /////////////////////////////////////
    //@brief Constructs a SoundBuffer with default values
    //@note Initializes audio structures and sets default volume (1.0f)
    /////////////////////////////////////
    SoundBuffer::SoundBuffer()
    {
        m_wfx = {};
        m_buffer = {};
        m_numberOfPresound = 0;
        m_name = TEXT(' ');
        m_id = 0;
        m_volume = 1.0f;
        m_isBackgroundSound = false;
        m_pSoundEffect = nullptr;
        m_is3D = false;
        m_normalizationGain = 1.0f;
        m_category = Category::NONE;
        m_categoryVolume = 1.0f;
        m_psubmix = nullptr;
    }

    /////////////////////////////////////
    //@brief Destructor for SoundBuffer
    //@note Frees allocated audio data memory
    /////////////////////////////////////
    SoundBuffer::~SoundBuffer()
    {
        if (m_buffer.pAudioData)
        {
            delete[] m_buffer.pAudioData;
        }
    }

    /////////////////////////////////////
    //@brief Initializes the Sound buffer from file
    //@param strFileName Path to audio file
    //@param name Identifier for this Sound
    //@param numberOfPresound Number of pre-allocated instances
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT SoundBuffer::Init(char16 const* strFileName, std::string const name, int32 numberOfPresound , bool is3D)
    {
        m_name = name;
        m_numberOfPresound = numberOfPresound;
        m_is3D = is3D;
        memset(&m_wfx, 0, sizeof(WAVEFORMATEX)); //Initialise de zero de la taille d'un waveformatex 

        HRESULT hr = S_OK;

        if (IsMP3(strFileName))
        {
            if (FAILED(hr = OpenMP3File(strFileName)))
            {
                MessageBoxW(NULL, L"Failed to open mp3", L"Audio Error", MB_ICONERROR);
                return hr;
            }
        }
        else
        {
            if (FAILED(hr = OpenAudioFile(strFileName)))
            {
                MessageBoxW(NULL, L"Failed to open audio file", L"Audio Error", MB_ICONERROR);
                return hr;
            }
        }

        m_normalizationGain = ComputeNormalizationFactor();
        std::cout << "Gain " << m_name << " : " << m_normalizationGain << '\n';
        return S_OK;
    }

    /////////////////////////////////////
    //@brief Opens and parses a WAV audio file
    //@param filePath Path to the audio file
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT SoundBuffer::OpenAudioFile(char16 const* filePath)
    {
        HANDLE hFile = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            MessageBoxW(NULL, L"Failed to open audio file", L"Audio Error", MB_ICONERROR);
            return HRESULT_FROM_WIN32(GetLastError());
        }

        HRESULT hr = LocateRiffSegment(hFile);
        if (FAILED(hr))
        {
            MessageBoxW(NULL, L"Invalid WAV file format", L"Audio Error", MB_ICONERROR);
            return hr;
        }

        CloseHandle(hFile);

        return S_OK;
    }

    /////////////////////////////////////
    //@brief Locates a specific chunk in a WAV file
    //@param hFile File handle to search
    //@param fourcc FourCC code of chunk to find
    //@param dwChunkSize [out] Size of found chunk
    //@param dwChunkDataPosition [out] Offset to chunk data
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT SoundBuffer::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
    {
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        DWORD dwChunkType = 0;
        DWORD dwChunkDataSize = 0;
        DWORD dwRIFFDataSize = 0;
        DWORD dwFileType = 0;
        DWORD dwOffset = 0;

        HRESULT hr = S_OK;
        while (hr == S_OK)
        {
            DWORD dwRead = 0;
            DWORD header[2] = { 0 };

            if (!ReadFile(hFile, header, sizeof(header), &dwRead, NULL))
            {
                return HRESULT_FROM_WIN32(GetLastError());
            }

            dwChunkType = header[0];
            dwChunkDataSize = header[1];

            switch (dwChunkType)
            {
            // riff is a type that stock audio data
            case fourcc_RIFF:
                dwRIFFDataSize = dwChunkDataSize;
                dwChunkDataSize = 4;
                if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                }
                break;

            default:
                LARGE_INTEGER li = { 0 };
                li.QuadPart = dwChunkDataSize;
                // SetFilePointerEx more effective than SetFilePointer
                if (INVALID_SET_FILE_POINTER == SetFilePointerEx(hFile, li, NULL, FILE_CURRENT))
                {
                    return HRESULT_FROM_WIN32(GetLastError());
                }
            }

            dwOffset += sizeof(DWORD) * 2;

            if (dwChunkType == fourcc)
            {
                dwChunkSize = dwChunkDataSize;
                dwChunkDataPosition = dwOffset;
                return S_OK;
            }

            dwOffset += dwChunkDataSize;
            if (0 >= dwRIFFDataSize) return S_FALSE;
        }
        return S_OK;
    }

    /////////////////////////////////////
    //@brief Reads data from a located chunk
    //@param hFile File handle to read from
    //@param buffer Buffer to store data
    //@param buffersize Size of buffer
    //@param bufferoffset Offset to read from
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT SoundBuffer::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
    {
        // A chunk is a part of a riff he generally have (ID / SIZE / DATA)
        LARGE_INTEGER li = { 0 };
        li.QuadPart = bufferoffset;
        if (INVALID_SET_FILE_POINTER == SetFilePointerEx(hFile, li, NULL, FILE_BEGIN))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        DWORD bytesRead = 0;
        if (0 == ReadFile(hFile, buffer, buffersize, &bytesRead, NULL))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }
        return (bytesRead == buffersize) ? S_OK : E_FAIL;
    }

    /////////////////////////////////////
    //@brief Locates and validates the RIFF segment in a WAV file
    //@param hFile File handle to search
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT SoundBuffer::LocateRiffSegment(HANDLE hFile)
    {
        DWORD dwChunkSize = 0;
        DWORD dwChunkPosition = 0;
        FindChunk(hFile, fourcc_RIFF, dwChunkSize, dwChunkPosition);

        DWORD filetype = 0;
        ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
        if (filetype != fourcc_WAVE)
        {
            MessageBoxW(NULL, L"File type is not WAVE", L"Audio Error", MB_ICONERROR);
            return S_FALSE;
        }

        LocateFMTSegmentAndfillWaveStruct(hFile, dwChunkSize, dwChunkPosition);
        LocateData(hFile, dwChunkSize, dwChunkPosition);

        return S_OK;
    }

    /////////////////////////////////////
    //@brief Locates and processes the format chunk
    //@param hFile File handle to search
    //@param dwChunkSize Size of chunk
    //@param dwChunkPosition Position of chunk
    /////////////////////////////////////
    void SoundBuffer::LocateFMTSegmentAndfillWaveStruct(HANDLE hFile, DWORD dwChunkSize, DWORD dwChunkPosition)
    {
        // FMT is the information about the format 
        FindChunk(hFile, fourcc_FMT, dwChunkSize, dwChunkPosition);
        ReadChunkData(hFile, &m_wfx, dwChunkSize, dwChunkPosition);
    }

    /////////////////////////////////////
    //@brief Locates and processes the data chunk
    //@param hFile File handle to search
    //@param dwChunkSize Size of chunk
    //@param dwChunkPosition Position of chunk
    /////////////////////////////////////
    void SoundBuffer::LocateData(HANDLE hFile, DWORD dwChunkSize, DWORD dwChunkPosition)
    {
        // DATA is the data audio
        FindChunk(hFile, fourcc_DATA, dwChunkSize, dwChunkPosition);

        BYTE* pDataBuffer = new BYTE[dwChunkSize];
        if (SUCCEEDED(ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition)))
        {
            FillAudioBuffer(dwChunkSize, pDataBuffer);
        }
        else
        {
            MessageBoxW(NULL, L"Failed to read chunk data", L"Audio Error", MB_ICONERROR);
            delete[] pDataBuffer;
        }
    }

    /////////////////////////////////////
    //@brief Fills the audio buffer structure with loaded data
    //@param dwChunkSize Size of audio data
    //@param pDataBuffer Pointer to audio data
    /////////////////////////////////////
    void SoundBuffer::FillAudioBuffer(DWORD dwChunkSize, BYTE* pDataBuffer)
    {
        m_buffer.AudioBytes = dwChunkSize;
        m_buffer.pAudioData = pDataBuffer;
        m_buffer.Flags = XAUDIO2_END_OF_STREAM;
    }

    /////////////////////////////////////
    //@brief Creates a playable Sound instance from this buffer
    //@return Pointer to new Sound object
    //@note Caller is responsible for memory management
    /////////////////////////////////////
    Sound* SoundBuffer::CreateSound()
    {
        if (!m_is3D)
        {
            SoundSource2D* Sound = new SoundSource2D();
            Sound->Init(&m_wfx, &m_buffer, m_id , m_normalizationGain, m_category , m_psubmix);
            Sound->SetVolume(m_volume , m_categoryVolume);
            return Sound;
        }
        else
        {
            SoundSource3D* Sound = new SoundSource3D();
            Sound->Init(&m_wfx, &m_buffer, m_id , m_normalizationGain, m_category, m_psubmix);
            Sound->SetVolume(m_volume , m_categoryVolume);
            return Sound;
        }
    }

    /////////////////////////////////////
    //@brief Gets the wave format structure
    //@return Copy of WAVEFORMATEX structure
    /////////////////////////////////////
    WAVEFORMATEX const& SoundBuffer::Getwfx() const
    {
        return m_wfx;
    }

    /////////////////////////////////////
    //@brief Gets the audio buffer structure
    //@return Copy of XAUDIO2_BUFFER structure
    /////////////////////////////////////
    XAUDIO2_BUFFER SoundBuffer::GetBuffer() const
    {
        return m_buffer;
    }

    /////////////////////////////////////
    //@brief Gets the Sound's name/identifier
    //@return Name character
    /////////////////////////////////////
    std::string SoundBuffer::GetName() const
    {
        return m_name;
    }

    /////////////////////////////////////
    //@brief Gets the number of pre-allocated instances
    //@return Count of pre-Sounds
    /////////////////////////////////////
    int32 SoundBuffer::GetNbrOfPreSound() const
    {
        return m_numberOfPresound;
    }

    /////////////////////////////////////
    //@brief Gets the Sound's unique ID
    //@return Sound ID
    /////////////////////////////////////
    uint32 SoundBuffer::GetID() const
    {
        return m_id;
    }

    /////////////////////////////////////
    //@brief Sets the Sound's unique ID
    //@param id New ID to set
    /////////////////////////////////////
    void SoundBuffer::SetID(uint32 id)
    {
        m_id = id;
    }

    /////////////////////////////////////
    //@brief Sets the default volume for Sounds created from this buffer
    //@param volume Volume level (0-100)
    /////////////////////////////////////
    void SoundBuffer::SetVolume(int32 volume)
    {
        m_volume = ConvertUserVolumeToActual(volume);
    }

    /////////////////////////////////////
    //@brief Gets the current volume setting
    //@return Current volume level (0-100)
    /////////////////////////////////////
    float32 SoundBuffer::GetVolume() const
    {
        return m_volume;
    }

    /////////////////////////////////////
    //@brief Marks this Sound as a background/looping Sound
    /////////////////////////////////////
    void SoundBuffer::MakeItBackgroundSound()
    {
        m_isBackgroundSound = true;
    }

    /////////////////////////////////////
    //@brief Checks if this is a background Sound
    //@return True if marked as background Sound
    /////////////////////////////////////
    bool SoundBuffer::GetIsBackgroundSound() const
    {
        return m_isBackgroundSound;
    }

    /////////////////////////////////////
    //@brief Checks if a file path has an MP3 extension
    //@param filePath Path to check
    //@return True if extension is .mp3 (case insensitive)
    /////////////////////////////////////
    bool SoundBuffer::IsMP3(char16 const* filePath)
    {
        const char16* pExt = nullptr;
        pExt = filePath;
        while (*filePath)
        {
            if (*filePath == L'.')
            {
                pExt = filePath;
            }
            ++filePath;
        }

        if (pExt != nullptr)
        {
            return (pExt[0] == L'.' && (pExt[1] == L'm' || pExt[1] == L'M') && (pExt[2] == L'p' || pExt[2] == L'P') && (pExt[3] == L'3') && pExt[4] == L'\0');
        }
        else
        {
            return false;
        }
    }

    /////////////////////////////////////
    //@brief Opens and converts an MP3 file to WAV format
    //@param filePath Path to MP3 file
    //@return HRESULT indicating success or failure
    //@note Uses Media Foundation to decode MP3 and creates temporary WAV file
    /////////////////////////////////////
    HRESULT SoundBuffer::OpenMP3File(const wchar_t* filePath)
    {
        HRESULT hr = MFStartup(MF_VERSION);
        if (FAILED(hr))
        {
            MessageBoxW(NULL, L"Failed to initialize Media Foundation", L"Audio Error", MB_ICONERROR);
            return hr;
        }
            

        IMFSourceReader* pReader = nullptr;
        hr = MFCreateSourceReaderFromURL(filePath, nullptr, &pReader);
        if (FAILED(hr)) 
        {
            MessageBoxW(NULL, L"Failed to create source reader form url", L"Audio Error", MB_ICONERROR);
            MFShutdown();
            return hr;
        }

        IMFMediaType* pAudioTypeOut = nullptr;
        hr = MFCreateMediaType(&pAudioTypeOut);
        if (FAILED(hr)) 
        {
            MessageBoxW(NULL, L"No audio data found in MP3 file", L"Audio Error", MB_ICONERROR);
            SafeRelease(&pReader);
            MFShutdown();
            return hr;
        }

        pAudioTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio); 
        pAudioTypeOut->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);  
        pAudioTypeOut->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
        pAudioTypeOut->SetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, 44100);

        if (m_is3D)
        {
            pAudioTypeOut->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 1); // mono
        }
        else
        {
            pAudioTypeOut->SetUINT32(MF_MT_AUDIO_NUM_CHANNELS, 2); // stereo
        }

        hr = pReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pAudioTypeOut); // tell to my reader what he need to use to read the file i gave him
        SafeRelease(&pAudioTypeOut);
        if (FAILED(hr)) 
        {
            MessageBoxW(NULL, L"Failed to set current media type", L"Audio Error", MB_ICONERROR);
            SafeRelease(&pReader);
            MFShutdown();
            return hr;
        }

        std::vector<BYTE> audioBuffer;

        while (true)
        {
            DWORD flags = 0;
            IMFSample* pSample = nullptr;
            hr = pReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &pSample);
            if (FAILED(hr) || (flags & MF_SOURCE_READERF_ENDOFSTREAM))
            {
                break;
            }
               

            if (pSample)
            {
                IMFMediaBuffer* pBuffer = nullptr;
                hr = pSample->ConvertToContiguousBuffer(&pBuffer); //give data to buffer 
                if (SUCCEEDED(hr))
                {
                    BYTE* pData = nullptr;
                    DWORD maxLength = 0, currentLength = 0;
                    hr = pBuffer->Lock(&pData, &maxLength, &currentLength); //Give the right to access the data 
                    if (SUCCEEDED(hr))
                    {
                        audioBuffer.insert(audioBuffer.end(), pData, pData + currentLength); // give the data to audioBuffer
                        pBuffer->Unlock();
                    }
                    SafeRelease(&pBuffer);
                }
                SafeRelease(&pSample);
            }
        }

        SafeRelease(&pReader);
        MFShutdown();

        if (audioBuffer.empty())
        {
            MessageBoxW(NULL, L"Audio buffer empty", L"Audio Error", MB_ICONERROR);
            return E_FAIL;
        }
            
        // Create a temp wave file
        DWORD dataSize = static_cast<DWORD>(audioBuffer.size());
        DWORD fileSize = 44 + dataSize; 

        std::vector<BYTE> wavBuffer(fileSize);
        BYTE* pByte = wavBuffer.data();

        memcpy(pByte, "RIFF", 4);
        *reinterpret_cast<DWORD*>(pByte + 4) = fileSize - 8;
        memcpy(pByte + 8, "WAVE", 4);
        memcpy(pByte + 12, "fmt ", 4);
        *reinterpret_cast<DWORD*>(pByte + 16) = 16; // size of fmt chunk
        *reinterpret_cast<WORD*>(pByte + 20) = 1;   // PCM format

        WORD numChannels = m_is3D ? 1 : 2;
        *reinterpret_cast<WORD*>(pByte + 22) = numChannels;              // channel
        *reinterpret_cast<DWORD*>(pByte + 24) = 44100;                   // sample rate
        *reinterpret_cast<DWORD*>(pByte + 28) = 44100 * numChannels * 2; // byte rate
        *reinterpret_cast<WORD*>(pByte + 32) = numChannels * 2;          // block align
        *reinterpret_cast<WORD*>(pByte + 34) = 16;                       // bits per sample

        memcpy(pByte + 36, "data", 4);
        *reinterpret_cast<DWORD*>(pByte + 40) = dataSize;
        memcpy(pByte + 44, audioBuffer.data(), dataSize);

        // Fill the buffer in the temp file
        wchar_t tempPath[MAX_PATH];
        wchar_t tempFile[MAX_PATH];
        GetTempPathW(MAX_PATH, tempPath);
        GetTempFileNameW(tempPath, L"WAV", 0, tempFile);
        HANDLE hFile = CreateFileW(tempFile, GENERIC_WRITE | GENERIC_READ, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, nullptr);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            MessageBoxW(NULL, L"Temp wav file invalid value", L"Audio Error", MB_ICONERROR);
            return E_FAIL;
        }
        DWORD bytesWritten = 0;
        WriteFile(hFile, wavBuffer.data(), fileSize, &bytesWritten, nullptr);

        SetFilePointer(hFile, 0, nullptr, FILE_BEGIN);

        // Manage the wav file
        hr = LocateRiffSegment(hFile);
        CloseHandle(hFile);

        return hr;
    }

    /////////////////////////////////////
    //@brief Writes WAV file data from MP3 source
    //@param pReader Source reader for MP3 file
    //@param hFile Handle to output WAV file
    //@param msecAudioData Duration to convert (0 for entire file)
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT SoundBuffer::WriteWaveFile(IMFSourceReader* pReader, HANDLE hFile, LONG msecAudioData)
    {
        HRESULT hr = S_OK;
        DWORD cbHeader = 0, cbAudioData = 0;
        IMFMediaType* pAudioType = nullptr;

        if (FAILED(hr = ConfigureAudioStream(pReader, &pAudioType)))
        {
            MessageBoxW(NULL, L"Failed to configure audio stream", L"Audio Error", MB_ICONERROR);
            return hr;
        }

        if (FAILED(hr = WriteWaveHeader(hFile, pAudioType, &cbHeader)))
        {
            MessageBoxW(NULL, L"Failed to write wave header", L"Audio Error", MB_ICONERROR);
            return hr;
        }

        if (FAILED(hr = WriteWaveData(hFile, pReader, UINT32_MAX, &cbAudioData)))
        {
            MessageBoxW(NULL, L"Failed to write wave data", L"Audio Error", MB_ICONERROR);
            return hr;
        }

        if (FAILED(hr = FixUpChunkSizes(hFile, cbHeader, cbAudioData)))
        {
            MessageBoxW(NULL, L"Failed to fix up chunk size", L"Audio Error", MB_ICONERROR);
            return hr;
        }

        SafeRelease(&pAudioType);
        return hr;
    }

    /////////////////////////////////////
    //@brief Configures audio stream for PCM output
    //@param pReader Source reader interface
    //@param ppPCMAudio [out] Receives configured media type
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT SoundBuffer::ConfigureAudioStream(IMFSourceReader* pReader, IMFMediaType** ppPCMAudio)
    {
        IMFMediaType* pPartial = nullptr;
        IMFMediaType* pFull = nullptr;
        HRESULT hr = S_OK;

        if (FAILED(hr = MFCreateMediaType(&pPartial)))
        {
            MessageBoxW(NULL, L"Failed to create media type", L"MF Error", MB_ICONERROR);
            goto exit;
        }

        hr = pPartial->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
        if (FAILED(hr = pPartial->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)))
        {
            MessageBoxW(NULL, L"Failed to set GUID", L"MF", MB_ICONERROR);
            goto exit;
        }

        if (FAILED(hr = pReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pPartial)))
        {
            MessageBoxW(NULL, L"Failed to set current media type", L"MF", MB_ICONERROR);
            goto exit;
        }

        if (FAILED(hr = pReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pFull)))
        {
            MessageBoxW(NULL, L"Failed to get media type", L"MF", MB_ICONERROR);
            goto exit;
        }

        if (FAILED(hr = pReader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE)))
        {
            MessageBoxW(NULL, L"Failed to set stream selection", L"MF", MB_ICONERROR);
            goto exit;
        }

        *ppPCMAudio = pFull;
        (*ppPCMAudio)->AddRef();

        exit :

            SafeRelease(&pPartial);
            SafeRelease(&pFull);
            return hr;
    }

    /////////////////////////////////////
    //@brief Writes WAV file header to output file
    //@param hFile Handle to output file
    //@param pWaveFormat Media type describing audio format
    //@param pcbWritten [out] Receives bytes written
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT SoundBuffer::WriteWaveHeader(HANDLE hFile, IMFMediaType* pWaveFormat, DWORD* pcbWritten)
    {
        HRESULT hr = S_OK;
        UINT32 cbFormat = 0;
        WAVEFORMATEX* pWavFormat = nullptr;

        if (pcbWritten) *pcbWritten = 0;

        hr = MFCreateWaveFormatExFromMFMediaType(pWaveFormat, &pWavFormat, &cbFormat);
        if (FAILED(hr))
        {
            MessageBoxW(NULL, L"Failed to create wave format", L"MF", MB_ICONERROR);
            return hr;
        }

        DWORD header[] = 
        {
            FCC('RIFF'),        // RIFF header
            0,                  // File size 
            FCC('WAVE'),        // Wave type
            FCC('fmt '),        // Start of 'fmt ' chunk
            cbFormat            // Size of fmt chunk
        };

        DWORD dataHeader[] = { FCC('data'), 0 }; 

        DWORD bytesWritten = 0;
        if (!WriteFile(hFile, header, sizeof(header), &bytesWritten, nullptr))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            CoTaskMemFree(pWavFormat);
            return hr;
        }

        if (!WriteFile(hFile, pWavFormat, cbFormat, &bytesWritten, nullptr))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            CoTaskMemFree(pWavFormat);
            return hr;
        }

        if (!WriteFile(hFile, dataHeader, sizeof(dataHeader), &bytesWritten, nullptr))
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
            CoTaskMemFree(pWavFormat);
            return hr;
        }

        if (pcbWritten) *pcbWritten = sizeof(header) + cbFormat + sizeof(dataHeader);
        CoTaskMemFree(pWavFormat);
        return S_OK;
    }

    /////////////////////////////////////
    //@brief Writes audio data to WAV file
    //@param hFile Handle to output file
    //@param pReader Source reader for audio data
    //@param maxBytes Maximum bytes to write
    //@param totalWritten [out] Receives total bytes written
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT SoundBuffer::WriteWaveData(HANDLE hFile, IMFSourceReader* pReader, DWORD maxBytes, DWORD* totalWritten)
    {
        HRESULT hr = S_OK;
        DWORD written = 0;
        DWORD total = 0;
        while (true)
        {
            DWORD flags = 0;
            IMFSample* pSample = nullptr;
            IMFMediaBuffer* pBuffer = nullptr;
            BYTE* pData = nullptr;
            DWORD size = 0;
            hr = pReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &pSample);
            if (FAILED(hr) || (flags & MF_SOURCE_READERF_ENDOFSTREAM))
            {
                MessageBoxW(NULL, L"Failed to read sample", L"MF", MB_ICONERROR);
                break;
            }
            if (SUCCEEDED(pSample->ConvertToContiguousBuffer(&pBuffer)) && SUCCEEDED(pBuffer->Lock(&pData, nullptr, &size)))
            {
                if (total + size > maxBytes)
                {
                    size = maxBytes - total;
                }
                if (size > 0 && !WriteFile(hFile, pData, size, &written, nullptr))
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                }
                pBuffer->Unlock();
                total += written;
            }

            SafeRelease(&pBuffer);
            SafeRelease(&pSample);
            if (FAILED(hr) || total >= maxBytes)
            {
                MessageBoxW(NULL, L"Failed to write wave data", L"MF", MB_ICONERROR);
                break;
            }
        }
        if (totalWritten)
        {
            *totalWritten = total;
        }
        return hr;
    }

    /////////////////////////////////////
    //@brief Updates WAV file header with final chunk sizes
    //@param hFile Handle to WAV file
    //@param cbHeader Size of header section
    //@param cbAudioData Size of audio data section
    //@return HRESULT indicating success or failure
    /////////////////////////////////////
    HRESULT SoundBuffer::FixUpChunkSizes(HANDLE hFile, DWORD cbHeader, DWORD cbAudioData)
    {
        LARGE_INTEGER li = {};
        ULONGLONG totalSize = static_cast<ULONGLONG>(cbAudioData) + cbHeader - 8;
        if (totalSize > 0xFFFFFFFF)
        {
            return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
        }
        DWORD cbRiffSize = static_cast<DWORD>(totalSize);
        DWORD cbDataSize = cbAudioData;

        li.QuadPart = 4;
        if (!SetFilePointerEx(hFile, li, nullptr, FILE_BEGIN)) {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        DWORD written = 0;
        if (!WriteFile(hFile, &cbRiffSize, sizeof(DWORD), &written, nullptr)) {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        li.QuadPart = static_cast<LONGLONG>(cbHeader) - 4;
        if (!SetFilePointerEx(hFile, li, nullptr, FILE_BEGIN)) {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        if (!WriteFile(hFile, &cbDataSize, sizeof(DWORD), &written, nullptr)) {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        return S_OK;
    }

    /////////////////////////////////////
    //@brief Get the effect of the sound
    //@return SoundEffect class
    //@note Create a new one if not existant
    /////////////////////////////////////
    SoundEffect* SoundBuffer::GetEffect() 
    {
        if (m_pSoundEffect == nullptr)
        {
            m_pSoundEffect = new SoundEffect();
        }

        return m_pSoundEffect;
    }

    /////////////////////////////////////
    //@brief Get the effect of the sound
    //@return SoundEffect class
    /////////////////////////////////////
    bool SoundBuffer::GetIs3DSound() const
    {
        return m_is3D;
    }

    /////////////////////////////////////
    //@brief Calculate the gain value needed to normalize sound
    //@param Target dbFS
    //@return Gain value
    /////////////////////////////////////
    float SoundBuffer::ComputeNormalizationFactor(float targetDbFS)
    {
        // dBFS (decibels relative to full scale) represents the amplitude of a signal
        // We use dBFS to evaluate the perceived loudness of a signal,
        // and by defining a target, we can normalize all sounds to play back at a consistent volume.

        // Step 1 Get Data and modify it to work on a correct plage (PCM 16 bits) (enable to get the amplitude wave of the sound)
        if (!m_buffer.pAudioData)
        {
            return 1.0f;
        }
        const int16_t* pSamples = reinterpret_cast<const int16_t*>(m_buffer.pAudioData); //Get Audio data (samples) and transform them to work on 16 bits (pcm bits default value)
        size_t totalSamp = m_buffer.AudioBytes / sizeof(int16_t); // we get total samples 

        // Step 2 We find the average volume of the audio using rms (root mean square)
        double accSq = 0.0;
        for (size_t i = 0; i < totalSamp; ++i)
        {
            // Normalize value around -1 and 1 to be in the range of rms
            // So we divide by max positive value of max amplitude
            double s = pSamples[i] / 32767.0;   // In 16-bit PCM audio, full scale is 32767.
            accSq += s * s; //We do sum of squares to remove negative symbole and not lost information
        }
        double rms = Sqrt(static_cast<float32>(accSq / totalSamp)); // formula of rms sqrt(1/n * sum of square)
        if (rms <= 0.0)
        {
            return 1.0f;
        }

        // Step 3 find the gain 
        float rmsDb = 20.0f * std::log10(static_cast<float>(rms)); // convert rms to dbfs (signal power 20.0f * log10(rms))
        float gainDb = targetDbFS - rmsDb; // compare the average and target      
        return std::pow(10.0f, gainDb / 20.0f); // us inverse formula of db to get a linear value 
    }

    void SoundBuffer::SetUpCategory(Category category, IXAudio2SubmixVoice* submix , float32 categoryVolume)
    {
        m_category = category;
        m_psubmix = submix;
        m_categoryVolume = categoryVolume;
    }

}
