#include "pch.h"
#include "GameObject.h"

#include "Scene.h"
#include "Systems.h"
#include "GameManager.h"

namespace gce {
    
GameObject::GameObject( cstr const name ):
m_name( name ) {}

bool GameObject::IsActive() const
{
    return m_active && (HasParent() ? GetParent()->IsActive() : true);
}

void GameObject::AddChild(GameObject& child)
{
    if (child.m_pParent == this) return;
    m_children.PushBack(&child);
    child.m_pParent = this;
    child.transform.NewParent();
}

void GameObject::RemoveChild(GameObject& child)
{
    if (child.m_pParent != this) return;
    for ( uint64 i = 0; i < m_children.Size(); i++ )
        if( m_children[i] == &child )
            m_children.Erase(m_children.begin()+i);
    child.transform.RemoveParent();
    child.m_pParent = nullptr;
}

GameObject& GameObject::Create( Scene& scene ) // ask & or *
{
    GameObject* const pNew = new GameObject();
    pNew->m_pScene = &scene;
    pNew->transform.m_pOwner = pNew;
    GameManager::GetLifespanSystem().m_toCreate.gameObjects.Push( pNew );
    return *pNew;
}

void GameObject::Destroy()
{
    if ( m_destroyed ) return;
    m_destroyed = true;

    m_destroyEvent.Invoke();

    for (auto& [typeId, scriptId] : m_scripts)
        Script::s_deletionList.Push(scriptId);
    m_scripts.clear();

    if ( HasComponent<Camera>() ) RemoveComponent<Camera>();
    if ((HasComponent<Light>())) RemoveComponent<Light>();
    if ( HasComponent<SpriteRenderer>() ) RemoveComponent<SpriteRenderer>();
    if ( HasComponent<MeshRenderer>() ) RemoveComponent<MeshRenderer>();
    if ( HasComponent<Animator2D>() ) RemoveComponent<Animator2D>();
    if ( HasComponent<BoxCollider2D>() ) RemoveComponent<BoxCollider2D>();
    if ( HasComponent<CircleCollider2D>() ) RemoveComponent<CircleCollider2D>();
    if ( HasComponent<SphereCollider>() ) RemoveComponent<SphereCollider>();
    if ( HasComponent<BoxCollider>() ) RemoveComponent<BoxCollider>();
    if ( HasComponent<PhysicComponent>() ) RemoveComponent<PhysicComponent>();
    if ((HasComponent<TextRenderer>())) RemoveComponent<TextRenderer>();
    if ((HasComponent<UiButton>())) RemoveComponent<UiButton>();
    if ((HasComponent<UiImage>())) RemoveComponent<UiImage>();
    if ((HasComponent<SkyBoxComponent>())) RemoveComponent<SkyBoxComponent>();

    GameManager::GetStatesSystem().DestroyStateMachine(this);
    GameManager::GetLifespanSystem().m_toDelete.gameObjects.Push( this );

    for (GameObject* const pChild : m_children)
        pChild->Destroy();
}


}