#ifndef AUDIO_HELPER_H_INCLUDED
#define AUDIO_HELPER_H_INCLUDED

#include "define.h"

namespace gce
{
    /**
     * @brief Converts user-facing volume (0-100) to XAudio2 volume range
     * @param userVolume Volume level from 0 to 100
     * @return Volume value scaled for XAudio2
     */
    static float32 ConvertUserVolumeToActual(int32 userVolume)
    {
        return (userVolume < 0) ? 0 : (userVolume > 100) ? 0.85f : userVolume * 0.0085f;
    }

    /**
     * @brief Release pointeur
     */
    template <typename T>
    static void SafeRelease(T** ppResource)
    {
        if (ppResource && *ppResource)
        {
            (*ppResource)->Release();
            *ppResource = nullptr;
        }
    }
}


#endif