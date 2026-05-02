#ifndef ENGINE_COMPONENTS_BASE_INL_INCLUDED
#define ENGINE_COMPONENTS_BASE_INL_INCLUDED


namespace gce {


inline GameObject& Component::GetOwner() { return *m_pOwner; }
inline GameObject const& Component::GetOwner() const { return *m_pOwner; }


}

#endif