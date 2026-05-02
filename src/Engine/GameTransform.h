#ifndef ENGINE_GAME_TRANSFORM_H_INCLUDED
#define ENGINE_GAME_TRANSFORM_H_INCLUDED

#include "define.h"

#include <Event.hpp>
#include <Maths/Vector3.h>
#include <Maths/Quaternion.h>
#include <Maths/Matrix.h>
#include <Maths/Transform.h>

#include <DynamicBuffer.h>
#include <Structs.h>

namespace gce {

class GameObject;


struct GameTransform final
{
    void SetLocalPosition( Vector3f32 const& position );
    void SetWorldPosition( Vector3f32 const& position );
    void LocalTranslate( Vector3f32 const& offset );
    void WorldTranslate( Vector3f32 const& offset );

    void SetLocalScale( Vector3f32 const& scale );
    void SetWorldScale( Vector3f32 const& scale );
    void LocalScale( Vector3f32 const& factor );
    void WorldScale( Vector3f32 const& factor );

    void SetLocalRotation( Quaternion const& rotation );
    void SetWorldRotation( Quaternion const& rotation );
    void SetLocalRotation( Vector3f32 const& angles );
    void SetWorldRotation( Vector3f32 const& angles );
    void LocalRotate( Vector3f32 const& angles );
    void WorldRotate( Vector3f32 const& angles );

    [[nodiscard]] Vector3f32 const& GetLocalPosition() const { return m_transform.position; }
    [[nodiscard]] Vector3f32 const& GetWorldPosition() const { return m_position; }

    [[nodiscard]] Vector3f32 const& GetLocalScale() const { return m_transform.scale; }
    [[nodiscard]] Vector3f32 const& GetWorldScale() const { return m_scale; }

    [[nodiscard]] Quaternion const& GetLocalRotation() const { return m_transform.rotation; }
    [[nodiscard]] Quaternion const& GetWorldRotation() const { return m_rotation; }

    [[nodiscard]] Vector3f32 const& GetLocalRight() const { return m_transform.right; }
    [[nodiscard]] Vector3f32 const& GetWorldRight() const { return m_right; }

    [[nodiscard]] Vector3f32 const& GetLocalUp() const { return m_transform.up; }
    [[nodiscard]] Vector3f32 const& GetWorldUp() const { return m_up; }

    [[nodiscard]] Vector3f32 const& GetLocalForward() const { return m_transform.forward; }
    [[nodiscard]] Vector3f32 const& GetWorldForward() const { return m_forward; }

    [[nodiscard]] Matrix const& GetWorldMatrix() const { return m_matrix; }

    struct Using { enum Enum: uint8 { NONE, POSITION = 1<<0, SCALE = 1<<1, ROTATION = 1<<2, }; };
    void ToggleUsing( Using::Enum flag );

    void UpdateMatrix();

    Event<uint8> dirtyEvent;
    
private:
    void UpdateLocalPosition();
    void UpdateWorldPosition();

    void UpdateLocalScale();
    void UpdateWorldScale();

    void UpdateLocalRotation();
    void UpdateWorldRotation();

    void UpdateLocalDirections();
    void UpdateWorldDirections();

    void NewParent();
    void RemoveParent();
    
    // Component
    GameObject* m_pOwner = nullptr;
    DynamicBuffer m_buffer{ sizeof(ObjectData) };
    
    // Dirty
    struct Dirty { enum Enum: uint8 {
        NONE,
        ANY,

        // LOCAL_POSITION   = 1<<1,
        // LOCAL_SCALE      = 1<<2,
        // LOCAL_ROTATION   = 1<<3,
        //
        // WORLD_POSITION   = 1<<4,
        // WORLD_SCALE      = 1<<5,
        // WORLD_ROTATION   = 1<<6,
        
        // POSITION   = LOCAL_POSITION   | WORLD_POSITION,
        // SCALE      = LOCAL_SCALE      | WORLD_SCALE,
        // ROTATION   = LOCAL_ROTATION   | WORLD_ROTATION,

        // DIRECTIONS = 1<<6,
    }; };
    Dirty::Enum m_dirty = Dirty::ANY;

    [[nodiscard]] bool IsDirty( Dirty::Enum flags ) const;
    
    Using::Enum m_using = static_cast<Using::Enum>( Using::POSITION | Using::SCALE | Using::ROTATION );

    // Maths
    Transform m_transform;

    Vector3f32 m_position { 0.0f, 0.0f, 0.0f }; /// @remarks Il ne faut pas le récupérer directement ici mais plutôt utiliser la méthode GetWorldPosition()
    Vector3f32 m_scale { 1.0f, 1.0f, 1.0f }; /// @remarks Il ne faut pas le récupérer directement ici mais plutôt utiliser la méthode GetWorldScale()

    Vector3f32 m_right { 1.0f, 0.0f, 0.0f }; /// @remarks Il ne faut pas le récupérer directement ici mais plutôt utiliser la méthode GetWorldRight()
    Vector3f32 m_up { 0.0f, 1.0f, 0.0f }; /// @remarks Il ne faut pas le récupérer directement ici mais plutôt utiliser la méthode GetWorldUp()
    Vector3f32 m_forward { 0.0f, 0.0f, 1.0f }; /// @remarks Il ne faut pas le récupérer directement ici mais plutôt utiliser la méthode GetWorldForward()

    Quaternion m_rotation { 0.0f, 0.0f, 0.0f, 1.0f }; /// @remarks Il ne faut pas le récupérer directement ici mais plutôt utiliser la méthode GetWorldRotation()

    Matrix m_matrix;

    friend struct SphereCollider;
	friend struct CircleCollider2D;
	friend struct BoxCollider2D;
    friend struct BoxCollider;
    friend class GameObject;
    friend class RenderSystem;

    friend Dirty::Enum operator|( Dirty::Enum, Dirty::Enum );
    friend Dirty::Enum operator~( Dirty::Enum );
    friend Dirty::Enum& operator|=( Dirty::Enum&, Dirty::Enum );
    friend Dirty::Enum& operator&=( Dirty::Enum&, Dirty::Enum );
    friend Dirty::Enum& operator^=( Dirty::Enum&, Dirty::Enum );
    friend Using::Enum operator|( Using::Enum, Using::Enum );
    friend Using::Enum& operator^=( Using::Enum&, Using::Enum );
};

inline GameTransform::Dirty::Enum operator|( GameTransform::Dirty::Enum const a, GameTransform::Dirty::Enum const b ) { return static_cast<GameTransform::Dirty::Enum>( static_cast<uint8>(a) | static_cast<uint8>(b) ); }
inline GameTransform::Dirty::Enum operator~( GameTransform::Dirty::Enum const a ) { return static_cast<GameTransform::Dirty::Enum>( ~static_cast<uint8>(a) ); }
inline GameTransform::Dirty::Enum& operator|=( GameTransform::Dirty::Enum& a, GameTransform::Dirty::Enum const b ) { reinterpret_cast<uint8&>(a) |= static_cast<uint8>(b); return a; }
inline GameTransform::Dirty::Enum& operator&=( GameTransform::Dirty::Enum& a, GameTransform::Dirty::Enum const b ) { reinterpret_cast<uint8&>(a) &= static_cast<uint8>(b); return a; }
inline GameTransform::Dirty::Enum& operator^=( GameTransform::Dirty::Enum& a, GameTransform::Dirty::Enum const b ) { reinterpret_cast<uint8&>(a) ^= static_cast<uint8>(b); return a; }
inline GameTransform::Using::Enum operator|( GameTransform::Using::Enum const a, GameTransform::Using::Enum const b ) { return static_cast<GameTransform::Using::Enum>( static_cast<uint8>(a) | static_cast<uint8>(b) ); }
inline GameTransform::Using::Enum& operator^=( GameTransform::Using::Enum& a, GameTransform::Using::Enum const b ) { reinterpret_cast<uint8&>(a) ^= static_cast<uint8>(b); return a; }


}

#endif