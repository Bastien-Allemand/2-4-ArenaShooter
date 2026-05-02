#ifndef ENGINE_CHRONO_H_INCLUDED
#define ENGINE_CHRONO_H_INCLUDED

#include "define.h"

namespace gce 
{

class Chrono final
{
public:
    Chrono();
    ~Chrono() = default;

    void Start();
    void Pause();
    float32 GetElapsedTime() const;
    float32 Reset();
    
private:
    bool m_paused = false;
    uint64 m_pauseTime;
    uint64 m_lastTime;
};


}

#endif