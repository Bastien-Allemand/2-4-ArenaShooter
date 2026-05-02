#include "pch.h"
#include "ScriptSystem.h"

void gce::ScriptSystem::StartScripts()
{
	m_startEvent.Invoke();
	m_startEvent.ClearListeners();
}
