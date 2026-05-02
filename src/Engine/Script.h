#ifndef ENGINE_COMPONENTS_SCRIPT_H
#define ENGINE_COMPONENTS_SCRIPT_H

#include <Containers/Vector.hpp>
#include <Containers/Queue.hpp>
#include "define.h"


enum ScriptFlag
{
	Awake			 = 1 << 0,
	Start            = 1 << 1,
	Update           = 1 << 2,
	FixedUpdate      = 1 << 3,
	Destroy          = 1 << 4,

	CollisionEnter   = 1 << 5,
	CollisionStay    = 1 << 6,
	CollisionExit    = 1 << 7,

	Collision2DEnter = 1 << 8,
	Collision2DStay  = 1 << 8,
	Collision2DExit  = 1 << 10,
};

#define DECLARE_SCRIPT( name, flags)                  \
    struct name: gce::Script                          \
    {												  \
		inline static const uint16 s_TYPE_ID = gce::Script::_StepId(); \
        name(uint8 f = 0): Script(f | flags){}

#define DECLARE_CHILD_SCRIPT(name, nameParent, flags)    \
	struct name : nameParent							\
	{													\
		inline static const uint16 s_TYPE_ID = gce::Script::_StepId(); \
		name(uint8 f = 0) : nameParent(f | flags) {}

#define END_SCRIPT };

template <typename ...Args>
struct Event;

namespace gce
{
	class GameObject;

	struct Script
	{
	public:
		Script() {};
		Script(uint8 flags) : m_flags(flags) {};
		~Script() = default;

	protected:
		GameObject* m_pOwner;
		uint16 m_id;
		inline static Vector<Script*> s_list;
		inline static Queue<uint16> s_idsList;
		inline static Queue<uint16> s_creationList;
		inline static Queue<uint16> s_deletionList;

		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void FixedUpdate() {}
		virtual void Destroy() {}

		virtual void CollisionEnter(GameObject* other) {}
		virtual void CollisionStay(GameObject* other) {}
		virtual void CollisionExit(GameObject* other) {}

		virtual void Collision2DEnter(GameObject* other) {}
		virtual void Collision2DStay(GameObject* other) {}
		virtual void Collision2DExit(GameObject* other) {}

		bool IsActive() const;
		void SetActive(bool active);
		static uint16 _StepId() { return s_nextId++; } // SHOULD NOT BE USED

	private:
		bool m_hadCollider = false;
		bool m_activated = true;
		uint8 m_flags = 0;
		inline static uint16 s_nextId = 0;
		void* m_pListeners[10];

		void Init();
		void UnInit();

		void OnAwake() { if (m_flags & ScriptFlag::Awake) Awake(); }
		void OnStart() { if ( IsActive() ) Start(); }
		void OnUpdate() 
		{ 
			if ( IsActive() ) Update();
		}
		void OnFixedUpdate() { if ( IsActive() ) FixedUpdate(); }
		void OnDestroy() { if ( IsActive() ) Destroy(); }

		void OnCollisionEnter(GameObject* const pOther) { if (IsActive()) CollisionEnter(pOther); }
		void OnCollisionStay(GameObject* const pOther) { if (IsActive()) CollisionStay(pOther); }
		void OnCollisionExit(GameObject* const pOther) { if (IsActive()) CollisionExit(pOther); }

		void OnCollision2DEnter(GameObject* const pOther) { if (IsActive()) Collision2DEnter(pOther); }
		void OnCollision2DStay(GameObject* const pOther) { if (IsActive()) Collision2DStay(pOther); }
		void OnCollision2DExit(GameObject* const pOther) { if (IsActive()) Collision2DExit(pOther); }

		friend class GameObject;
		friend class LifespanSystem;
	};
}

#endif
