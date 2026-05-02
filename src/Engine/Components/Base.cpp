#include "pch.h"
#include "Base.h"

#include "GameObject.h"

namespace gce
{
	bool Component::IsActive()
	{
		return m_active && m_pOwner->IsActive();
	}

	void Component::SetActive(bool active)
	{
		m_active = active;
	}


}
