#ifndef ENGINE_CAMERA_H_INCLUDED
#define ENGINE_CAMERA_H_INCLUDED

#include <RenderCamera.h>
#include "define.h"
#include "Base.h"

namespace gce {


DECLARE_COMPONENT( Camera, ComponentBase ), RenderCamera
{
	uint32 screenId = 0;

    void SetMainCamera();
    void RemoveMainCamera();
    
protected:
    void Init() override;
    void UpdateCamera( uint8 dirty );

    bool m_mainCamera = false;

    friend class RenderSystem;
    friend class GameManager; // TEMPORARY
};


}

#endif
