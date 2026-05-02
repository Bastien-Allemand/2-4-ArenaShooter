#ifndef ENGINE_STATES_SYSTEM_H_INCLUDED
#define ENGINE_STATES_SYSTEM_H_INCLUDED
#include "define.h"

namespace gce 
{

	struct StateMachine;
	class GameObject;

	class StatesSystem final
	{
	public :
		
		StateMachine* CreateStateMachine(GameObject* pMe);
		void DestroyStateMachine(GameObject* pMe);
		
	private :
		
		StatesSystem() = default;
		~StatesSystem() = default;
		void HandleStateMachines();

		UnorderedMap<GameObject*, StateMachine*> m_stateMachineList;
		
		friend class GameManager;
	};

}
#endif
