#ifndef ENGINE_GAME_OBJECT_INL_INCLUDED
#define ENGINE_GAME_OBJECT_INL_INCLUDED
#include "GameObject.h"
#include "Script.h"
#include "GameManager.h"

namespace gce {
    
inline uint32 GameObject::GetID() const { return m_id; }
inline cstr GameObject::GetName() const { return m_name; }
inline Scene* GameObject::GetScene() const { return m_pScene; }
    
inline void GameObject::SetName(cstr name) { m_name = name; }
inline void GameObject::SetActive( bool const active ) { m_active = active; }

inline bool GameObject::HasParent() const { return m_pParent != nullptr; }
inline GameObject* GameObject::GetParent() { return m_pParent; }
inline GameObject const* GameObject::GetParent() const { return m_pParent; }
inline void GameObject::SetParent( GameObject& parent ) { parent.AddChild(*this); }
inline void GameObject::RemoveParent() { m_pParent->RemoveChild(*this); }

inline bool GameObject::HasChildren() const { return m_children.Empty() == false; }
inline uint64 GameObject::ChildrenCount() const { return m_children.Size(); }
inline Vector<GameObject*>& GameObject::GetChildren() { return m_children; }
inline Vector<GameObject*> const& GameObject::GetChildren() const { return m_children; }


template <class ComponentClass>
bool GameObject::HasComponent() const { return m_componentsBitmask & ComponentClass::TypeMask; }

template <class ComponentClass>
bool GameObject::HasComponentOfSameFamily() const { return m_componentsBitmask & ComponentClass::FamilyMask; }

template <class ComponentClass>
ComponentClass* GameObject::GetComponent() { return HasComponent<ComponentClass>() ? ComponentClass::s_list[m_components[ComponentClass::TypeId]] : nullptr; }

template <class ComponentClass>
ComponentClass const* GameObject::GetComponent() const { return HasComponent<ComponentClass>() ? ComponentClass::s_list[m_components[ComponentClass::TypeId]] : nullptr; }

template <class ComponentClass>
ComponentClass* GameObject::AddComponent()
{
    if ( HasComponent<ComponentClass>() ) return reinterpret_cast<ComponentClass*>(ComponentClass::s_list[m_components[ComponentClass::TypeId]]);
    if ( HasComponentOfSameFamily<ComponentClass>() ) return nullptr;
    if ( ComponentClass::RequireMask && ( m_componentsBitmask & ComponentClass::RequireMask ) == false ) return nullptr; // À VOIR
    m_componentsBitmask |= ComponentClass::TypeMask;
    // Add Component
    uint16 id;
    if ( ComponentClass::s_idsList.Empty() )
    {
        ComponentClass::s_list.PushBack( new ComponentClass );
        id = ComponentClass::s_list.Size() - 1;
    }
    else
    {
        id = ComponentClass::s_idsList.Front();
        ComponentClass::s_idsList.Pop();
    }
    Component& component = *ComponentClass::s_list[id];
    component.m_pOwner = this;
    component.m_id = id;
    m_components[ComponentClass::TypeId] = id;
    ComponentClass::s_creationList.Push( id );
    component.Init();
    return ComponentClass::s_list[id];
}

template <class ComponentClass>
void GameObject::RemoveComponent()
{
    if ( HasComponent<ComponentClass>() == false ) return;
    m_componentsBitmask &= ~ComponentClass::TypeMask;
    // Remove Component
    uint16 id = m_components[ComponentClass::TypeId];
    ComponentClass::s_deletionList.Push(id);
    ComponentClass::s_list[id]->m_destroyed = true;
    m_components.erase( ComponentClass::TypeId );
}

template<class ScriptClass>
bool GameObject::HasScript() const
{
    return m_scripts.contains(ScriptClass::s_TYPE_ID);
}

template<class ScriptClass>
ScriptClass* GameObject::AddScript()
{
    if (HasScript<ScriptClass>()) return reinterpret_cast<ScriptClass*>(Script::s_list[m_scripts[ScriptClass::s_TYPE_ID]]);
    uint16 id;
    if (Script::s_idsList.Empty())
    {
        Script::s_list.std.emplace_back();
        id = Script::s_list.Size() - 1;
    }
    else
    {
        id = Script::s_idsList.Front();
        Script::s_idsList.Pop();
    }
    Script::s_list[id] = new ScriptClass();
    Script::s_list[id]->m_id = id;
    Script::s_list[id]->m_pOwner = this;
    Script::s_list[id]->OnAwake();
    Script::s_creationList.Push(id);
    m_scripts[ScriptClass::s_TYPE_ID] = id;
    return reinterpret_cast<ScriptClass*>(Script::s_list[id]);
}

template<class ScriptClass>
ScriptClass* GameObject::GetScript()
{
    return HasScript<ScriptClass>() ? dynamic_cast<ScriptClass*>(Script::s_list[m_scripts[ScriptClass::s_TYPE_ID]]) : nullptr;
}

template<class ScriptClass>
ScriptClass const* GameObject::GetScript() const
{
    return HasScript<ScriptClass>() ? dynamic_cast<ScriptClass*>(Script::s_list[m_scripts[ScriptClass::s_TYPE_ID]]) : nullptr;
}

template<class ScriptClass>
void GameObject::RemoveScript()
{
    if (HasScript<ScriptClass>() == false) return;

    uint16 id = m_scripts[ScriptClass::s_TYPE_ID];
    Script::s_deletionList.Push(id);
    m_scripts.erase(ScriptClass::s_TYPE_ID);
}


}

#endif