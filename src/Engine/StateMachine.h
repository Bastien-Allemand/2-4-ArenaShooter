#ifndef CORE_STATE_MACHINE_H_INCLUDED
#define CORE_STATE_MACHINE_H_INCLUDED

namespace gce
{

	class GameObject;

	using OnBegin = void(*)(GameObject* pMe);
	using OnUpdate = void(*)(GameObject* pMe);
	using OnEnd = void(*)(GameObject* pMe);

	struct StateMachine
	{
		StateMachine(GameObject* pMe);
		
		struct Condition
		{
			bool(*condition)(GameObject* pMe);
		};

		struct Transition
		{
			Vector<Condition> conditions;
			String target;
		};

		struct Action
		{
			OnBegin onBegin;
			OnUpdate onUpdate;
			OnEnd onEnd;
		};

		void AddAction(String const & name, Action action = {});
		void AddAction(String const & name, OnBegin pOnBegin = nullptr, OnUpdate pOnUpdate = nullptr, OnEnd pOnEnd = nullptr);

		void SetOnBeginAction(String const & name, OnBegin pOnBegin);
		void SetOnUpdateAction(String const & name, OnUpdate pOnUpdate);
		void SetOnEndAction(String const & name, OnEnd pOnEnd);

		void AddTransition(Transition transition);
		void AddTransition(Vector<Condition>& conditions, String const& target);

		void Update();

		void Transit(String const& target);

		String actualAction;

	private:

		GameObject* m_pMe = nullptr;

		String m_actualAction;

		UnorderedMap< String, Action> m_actions;
		Vector<Transition> m_transitions;

		friend class StatesSystem;
	};
}
#endif