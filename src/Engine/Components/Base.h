#ifndef ENGINE_COMPONENTS_BASE_H_INCLUDED
#define ENGINE_COMPONENTS_BASE_H_INCLUDED

#include <Containers/Vector.hpp>
#include "define.h"
#include "Containers/Queue.hpp"

namespace gce {

class GameObject;


struct Component
{
    virtual ~Component() = default;
    enum TypeEnum: uint8 {
        Camera,
        Light,
        SpriteRenderer,
        MeshRenderer,
        Animator2D,
        BoxCollider2D,
        CircleCollider2D,
        SphereCollider,
        BoxCollider,
        PhysicComponent,
        TextRenderer,
        UiButton,
        UiImage,
        SkyBoxComponent,
       
    };
    enum TypeMasks: uint64
    {
        NoneMask,
        CameraMask           = 1 << Camera,
        MeshRendererMask     = 1 << MeshRenderer,
        SkyBoxComponentMask  = 1 << SkyBoxComponent,
        
        LightMask            = 1 << Light,
        
        SpriteRendererMask   = 1 << SpriteRenderer,
        Animator2DMask       = 1 << Animator2D,

        BoxCollider2DMask    = 1 << BoxCollider2D,
        CircleCollider2DMask = 1 << CircleCollider2D,
        SphereColliderMask   = 1 << SphereCollider,
        BoxColliderMask      = 1 << BoxCollider,
        CollidersMask        = BoxCollider2DMask | CircleCollider2DMask | SphereColliderMask | BoxColliderMask,

        PhysicComponentMask  = 1 << PhysicComponent,
		TextRendererMask = 1 << TextRenderer,
        UiButtonMask  = 1 << UiButton,
        UiImageMask   = 1 << UiImage,
    };

    [[nodiscard]] GameObject& GetOwner();
    [[nodiscard]] GameObject const& GetOwner() const;
    [[nodiscard]] bool IsActive();
    void SetActive(bool Active);

protected:
    virtual void Init() {}

    bool m_created = false;
    bool m_destroyed = false;
    bool m_active = true;
    
    uint16 m_id = 0;
    GameObject* m_pOwner = nullptr;

    friend class GameObject;
};


template <class C, Component::TypeEnum typeId, uint64 requireMask = Component::NoneMask, uint64 familyMask = Component::NoneMask>
struct ComponentBase: Component
{
protected:
    static constexpr TypeEnum TypeId = typeId;
    static constexpr uint64 TypeMask = 1 << typeId;
    static constexpr uint64 RequireMask = requireMask;
    static constexpr uint64 FamilyMask = familyMask;

    inline static Vector<C*> s_list;
    inline static Queue<uint16> s_idsList;
    inline static Queue<uint16> s_creationList;
    inline static Queue<uint16> s_deletionList;

    friend class GameObject;
    friend class LifespanSystem;
    friend class GameManager;
    friend class UiSystem;
};


}

#include "Base.inl"

#endif