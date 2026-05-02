#include "pch.h"
#include "StatesSystem.h"

#include "GameObject.h"
#include "../StateMachine.h"

namespace gce 
{
	StateMachine* StatesSystem::CreateStateMachine(GameObject* pMe)
	{
		if (m_stateMachineList.contains(pMe))
			return m_stateMachineList[pMe];
		
		StateMachine* stateMachine = new StateMachine(pMe);

		m_stateMachineList[pMe] = stateMachine;
		return stateMachine;
	}

	void StatesSystem::DestroyStateMachine(GameObject* pMe)
	{
		if (m_stateMachineList.contains(pMe) == false)
			return;

		m_stateMachineList.erase(pMe);
	}

	//////////////////////////////////////////////////////////////////////
	/// @brief Updates all registered state machines.
	/// Iterates through the list of state machines and calls
	/// their Update() method to evaluate transition conditions
	/// and execute the current state logic.
	///
	/// @note This function is typically called once per frame
	/// to ensure real-time behavior processing of all state machines.
	//////////////////////////////////////////////////////////////////////
void StatesSystem::HandleStateMachines()
{
	for (auto& [gameObject, statemachine] : m_stateMachineList)
	{
		if (gameObject->IsActive())
			statemachine->Update();
	}
}

}