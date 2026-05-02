#ifndef ENGINE_SKYBOX_COMPONENT_H_INCLUDED
#define ENGINE_SKYBOX_COMPONENT_H_INCLUDED

#include "define.h"
#include "Base.h"
#include "CubeMap.h"

namespace gce {

class Geometry;
class D12PipelineObject;
class Texture;

DECLARE_COMPONENT( SkyBoxComponent, ComponentBase ), public CubeMap
{
protected:
    friend class RenderSystem;
};

}

#endif