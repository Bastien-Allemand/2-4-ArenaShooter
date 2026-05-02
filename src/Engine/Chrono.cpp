#include "pch.h"
#include "Chrono.h"
#include <Windows.h>

namespace gce 
{


Chrono::Chrono(): m_lastTime( timeGetTime() ) {}


void Chrono::Start()
{
    if (!m_paused)
        return;

    m_lastTime += timeGetTime() - m_pauseTime;
    m_paused = false;
    return;
}

void Chrono::Pause()
{
    if (m_paused)
        return;

    m_pauseTime = timeGetTime();
    m_paused = true;
}

float32 Chrono::GetElapsedTime() const
{
    if (m_paused)
    {
        uint64 const elapsedTime = m_pauseTime - m_lastTime;
        return elapsedTime / 1000.0f;
    }
    uint64 const systemTime = timeGetTime();
    uint64 const elapsedTime = systemTime - m_lastTime;
    return elapsedTime / 1000.0f;
}

float32 Chrono::Reset()
{
    if (m_paused)
    {
        uint64 const systemTime = timeGetTime();
        uint64 const elapsedTime = m_pauseTime - m_lastTime;
        m_lastTime = systemTime;
        m_pauseTime = systemTime;
        return elapsedTime / 1000.0f;
    }

    uint64 const systemTime = timeGetTime();
    uint64 const elapsedTime = systemTime - m_lastTime;
    m_lastTime = systemTime;
    return elapsedTime / 1000.0f;
}


}