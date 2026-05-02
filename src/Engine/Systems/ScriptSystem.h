#ifndef ENGINE_SYSTEMS_SCRIPT_SYSTEM_H_INCLUDED
#define ENGINE_SYSTEMS_SCRIPT_SYSTEM_H_INCLUDED

#include "Event.hpp"

namespace gce
{
	struct Script;

	class ScriptSystem final
	{
	private:
		ScriptSystem() = default;
		~ScriptSystem() = default;

		Vector<Script*> m_scripts;

		Event<> m_startEvent;
		Event<> m_updateEvent;
		Event<> m_fixedUpdateEvent;

		void StartScripts();

		friend struct Script;
		friend class LifespanSystem;
		friend class GameManager;
	};
}


#endif