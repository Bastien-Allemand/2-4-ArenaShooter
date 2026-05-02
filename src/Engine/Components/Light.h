#ifndef ENGINE_SPOT_LIGHT_H_INCLUDED
#define ENGINE_SPOT_LIGHT_H_INCLUDED

#include "define.h"
#include "LightManager.h"
#include "Base.h"

namespace gce {
    
class Geometry;
    
DECLARE_COMPONENT( Light, ComponentBase), LightData
{
     Light& DefaultPointLight();
     Light& DefaultDirectionLight();
     Light& DefaultSpotLight();

    void UpdateLight() { LightManager::UpdateLight(*this); }

protected:
    using LightData::id;
    using LightData::numFramesDirty;
    using LightData::padding1;
    using LightData::padding2;
    using LightData::padding3;
    using LightData::padding4;
    
protected:
    void Init() override;   
};

}

#endif