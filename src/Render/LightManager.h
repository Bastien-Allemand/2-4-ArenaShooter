#ifndef RENDER_LIGHT_MANAGER_H_INCLUDED
#define RENDER_LIGHT_MANAGER_H_INCLUDED

#include "Structs.h"

namespace gce
{
class LightManager
{
public:
    static void AddLight(LightData& light);
    static void RemoveLight(int32 lightID);
    static void UpdateLight(LightData& light);
    static void SetLightsProperties(float32 minTessDistance, float32 maxTessDistance, float32 minTessFactor, float32 maxTessFactor, float32 ambientIntensity);
    static void ClearLights();
    static LightData& GetLight(uint32 index);
    static uint32 GetLightCount();

    static LightData CreateDirectionalLight(Vector3f32 const& direction, Vector4 const& color, float32 intensity, float32 strength);
    static LightData CreatePointLight(Vector3f32 const& position, Vector4 const& color, float32 range, float32 intensity, float32 strength);
    static LightData CreateSpotLight(Vector3f32 const& position, Vector3f32 const& direction, Vector4 const& color, float32 spotAngle, float32 penumbraAngle, float32 range, float32 intensity, float32 strength);


    static void UpdateLightCB();
private:

    LightManager();
    ~LightManager();

    static LightManager& Instance()
    {
        static LightManager lm;
        return lm;
    }

    LightPassData m_lightPassData;
    uint32 m_lastFreeIndexInList = 0;
    uint32 m_lightCount = 0;
};
}

#endif