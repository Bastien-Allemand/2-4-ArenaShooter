#ifndef ENGINE_MESH_RENDERER_H_INCLUDED
#define ENGINE_MESH_RENDERER_H_INCLUDED

#include "define.h"
#include <Structs.h>
#include "Base.h"

namespace gce {

class Geometry;
class D12PipelineObject;
class Texture;

DECLARE_COMPONENT( MeshRenderer, ComponentBase ), protected RenderItem
{
    void SetAlbedoTexture(Texture const* texture);
    void SetMetalnessTexture(Texture const* texture);
    void SetRoughnessTexture(Texture const* texture);
    void SetDisplacementTexture(Texture const* texture);
    void SetNormalTexture(Texture const* texture);
    void SetAmbientTexture(Texture const* texture);

    void SetSubsurfaceScattering(float32 value);
    void SetFresnelR0(Vector3f32 const& vector);

    void SetGeometry(Geometry* geometry);

    Geometry const* GetGeometry() const;
protected:
    void Init() override;

    void UpdateWorld( uint8 dirty );

    Material* pMaterial = &objData.material;
    
    using RenderItem::pGeometry;
    using RenderItem::pPso;
    
    friend class RenderSystem;
};


}

#endif