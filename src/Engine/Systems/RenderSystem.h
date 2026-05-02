#ifndef ENGINE_RENDER_SYSTEM_H_INCLUDED
#define ENGINE_RENDER_SYSTEM_H_INCLUDED

#include "define.h"

namespace gce {

class RenderSystem final
{
    RenderSystem() = default;
    ~RenderSystem() = default;

    void Handle3DRendering(); 
     
    friend class GameManager;
};


}

#endif