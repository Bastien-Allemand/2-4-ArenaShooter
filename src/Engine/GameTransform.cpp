#include "pch.h"
#include "GameTransform.h"

#include "GameObject.h"

namespace gce {


void GameTransform::SetLocalPosition( Vector3f32 const& position )
{
    m_transform.position = position;
    UpdateWorldPosition();
    m_dirty |= Dirty::ANY;// | Dirty::LOCAL_POSITION;
}

void GameTransform::SetWorldPosition( Vector3f32 const& position )
{
    m_position = position;
    UpdateLocalPosition();
    m_dirty |= Dirty::ANY;// | Dirty::WORLD_POSITION;
}

void GameTransform::LocalTranslate( Vector3f32 const& offset )
{
    m_transform.position += offset;
    UpdateWorldPosition();
    m_dirty |= Dirty::ANY;// | Dirty::LOCAL_POSITION;
}

void GameTransform::WorldTranslate( Vector3f32 const& offset )
{
    m_position += offset;
    UpdateLocalPosition();
    m_dirty |= Dirty::ANY;// | Dirty::WORLD_POSITION;
}


void GameTransform::SetLocalScale( Vector3f32 const& scale )
{
    m_transform.scale = scale;
    UpdateWorldScale();
    m_dirty |= Dirty::ANY;// | Dirty::LOCAL_SCALE;
}

void GameTransform::SetWorldScale( Vector3f32 const& scale )
{
    m_scale = scale;
    UpdateLocalScale();
    m_dirty |= Dirty::ANY;// | Dirty::WORLD_SCALE;
}

void GameTransform::LocalScale( Vector3f32 const& factor )
{
    m_transform.scale *= factor;
    UpdateWorldScale();
    m_dirty |= Dirty::ANY;// | Dirty::LOCAL_SCALE;
}

void GameTransform::WorldScale( Vector3f32 const& factor )
{
    m_scale *= factor;
    UpdateLocalScale();
    m_dirty |= Dirty::ANY;// | Dirty::WORLD_SCALE;
}


void GameTransform::SetLocalRotation( Quaternion const& rotation )
{
	m_transform.rotation = rotation;
    UpdateWorldRotation();
    UpdateLocalDirections();
    m_dirty |= Dirty::ANY;// | Dirty::LOCAL_ROTATION;
}

void GameTransform::SetLocalRotation( Vector3f32 const& angles )
{
    Quaternion result = Quaternion::RotationAxis( m_transform.right, angles.x );
    result *= Quaternion::RotationAxis( m_transform.up, angles.y );
    result *= Quaternion::RotationAxis( m_transform.forward, angles.z );
    m_transform.rotation = result;
    UpdateWorldRotation();
    UpdateLocalDirections();
    m_dirty |= Dirty::ANY;// | Dirty::LOCAL_ROTATION;
}

void GameTransform::SetWorldRotation( Quaternion const& rotation )
{
    m_rotation = rotation;
    UpdateLocalRotation();
    UpdateWorldDirections();
    m_dirty |= Dirty::ANY;// | Dirty::WORLD_ROTATION;
}

void GameTransform::SetWorldRotation( Vector3f32 const& angles )
{
    m_rotation = Quaternion::RotationEuler( angles );
    UpdateLocalRotation();
    UpdateWorldDirections();
    m_dirty |= Dirty::ANY;// | Dirty::WORLD_ROTATION;
}

void GameTransform::LocalRotate( Vector3f32 const& angles )
{
    Quaternion result = Quaternion::RotationAxis( m_transform.right, angles.x );
    result *= Quaternion::RotationAxis( m_transform.up, angles.y );
    result *= Quaternion::RotationAxis( m_transform.forward, angles.z );
	m_transform.rotation *= result;
    UpdateWorldRotation();
    UpdateLocalDirections();
    m_dirty |= Dirty::ANY;// | Dirty::LOCAL_ROTATION;
}

void GameTransform::WorldRotate( Vector3f32 const& angles )
{
    m_rotation *= Quaternion::RotationEuler( angles );
    UpdateLocalRotation();
    UpdateWorldDirections();
    m_dirty |= Dirty::ANY;// | Dirty::WORLD_ROTATION;
}


void GameTransform::ToggleUsing( Using::Enum const flag )
{
    m_using ^= flag;
    if ( m_pOwner->HasParent() == false ) return;
    NewParent();
}


void GameTransform::UpdateMatrix()
{
    if ( m_dirty == Dirty::NONE ) return;

    Vector3f32 scaleFactor { 1.0f, 1.0f, 1.0f };

    if ( m_pOwner->HasParent() && m_using & Using::SCALE ) scaleFactor = m_pOwner->GetParent()->transform.GetWorldScale();
        
    m_matrix.SetValue( 0, 0, m_transform.scale.x * m_transform.right.x * scaleFactor.x );
    m_matrix.SetValue( 0, 1, m_transform.scale.x * m_transform.right.y * scaleFactor.y );
    m_matrix.SetValue( 0, 2, m_transform.scale.x * m_transform.right.z * scaleFactor.z );

    m_matrix.SetValue( 1, 0, m_transform.scale.y * m_transform.up.x * scaleFactor.x );
    m_matrix.SetValue( 1, 1, m_transform.scale.y * m_transform.up.y * scaleFactor.y );
    m_matrix.SetValue( 1, 2, m_transform.scale.y * m_transform.up.z * scaleFactor.z );

    m_matrix.SetValue( 2, 0, m_transform.scale.z * m_transform.forward.x * scaleFactor.x );
    m_matrix.SetValue( 2, 1, m_transform.scale.z * m_transform.forward.y * scaleFactor.y );
    m_matrix.SetValue( 2, 2, m_transform.scale.z * m_transform.forward.z * scaleFactor.z );

    if ( m_pOwner->HasParent() && m_using & Using::ROTATION ) m_matrix *= Matrix::Rotation( m_pOwner->GetParent()->transform.GetWorldRotation() );

    m_matrix.SetValue( 3, 0, m_position.x );
    m_matrix.SetValue( 3, 1, m_position.y );
    m_matrix.SetValue( 3, 2, m_position.z );

    for ( GameObject* const pChild : m_pOwner->GetChildren() )
    {
        if ( pChild == nullptr ) break;
        pChild->transform.m_dirty |= Dirty::ANY;
        pChild->transform.UpdateMatrix();
    }

    dirtyEvent.Invoke( m_dirty );
    m_dirty = Dirty::NONE;
}


/// @brief Updates LocalPosition from WorldPosition
void GameTransform::UpdateLocalPosition()
{
    m_transform.position = m_position;
    if ( m_pOwner->HasParent() && m_using & Using::POSITION )
    {
        GameTransform const& parentTransform = m_pOwner->GetParent()->transform;
        m_transform.position -= parentTransform.GetWorldPosition();
        if ( m_using & Using::ROTATION )
        {
            Matrix positionMatrix = Matrix::Translation( m_transform.position );
            positionMatrix *= Matrix::Rotation( parentTransform.GetWorldRotation() ).GetInverse(); // Can be optimized with 3x3 matrices (vector3 * matrix3x3 = vector3)
            m_transform.position.x = positionMatrix.GetValue( 3, 0 );
            m_transform.position.y = positionMatrix.GetValue( 3, 1 );
            m_transform.position.z = positionMatrix.GetValue( 3, 2 );
        }
        if ( m_using & Using::SCALE ) m_transform.position /= parentTransform.GetWorldScale();
    }

    for ( GameObject* const pChild : m_pOwner->GetChildren() )
    {
        if ( pChild == nullptr ) break;
        pChild->transform.m_dirty |= Dirty::ANY;
        pChild->transform.UpdateWorldPosition();
    }
}

/// @brief Updates WorldPosition from LocalPosition
void GameTransform::UpdateWorldPosition()
{
    m_position = m_transform.position;
    if ( m_pOwner->HasParent() && m_using & Using::POSITION )
    {
        GameTransform const& parentTransform = m_pOwner->GetParent()->transform;
        if ( m_using & Using::SCALE ) m_position *= parentTransform.GetWorldScale();
        if ( m_using & Using::ROTATION )
        {
            Matrix positionMatrix = Matrix::Translation( m_position );
            positionMatrix *= Matrix::Rotation( parentTransform.GetWorldRotation() ); // Can be optimized with 3x3 matrices (vector3 * matrix3x3 = vector3)
            m_position.x = positionMatrix.GetValue( 3, 0 );
            m_position.y = positionMatrix.GetValue( 3, 1 );
            m_position.z = positionMatrix.GetValue( 3, 2 );
        }
        m_position += parentTransform.GetWorldPosition();
    }

    for ( GameObject* const pChild : m_pOwner->GetChildren() )
    {
        if ( pChild == nullptr ) break;
        pChild->transform.m_dirty |= Dirty::ANY;
        pChild->transform.UpdateWorldPosition();
    }
}


/// @brief Updates LocalScale from WorldScale
void GameTransform::UpdateLocalScale()
{
    m_transform.scale = m_scale;
    if ( m_pOwner->HasParent() && m_using & Using::SCALE ) m_transform.scale /= m_pOwner->GetParent()->transform.GetWorldScale();

    for ( GameObject* const pChild : m_pOwner->GetChildren() )
    {
        if ( pChild == nullptr ) break;
        pChild->transform.m_dirty |= Dirty::ANY;
        pChild->transform.UpdateWorldScale();
    }
}

/// @brief Updates WorldScale from LocalScale
void GameTransform::UpdateWorldScale()
{
    m_scale = m_transform.scale;
    if ( m_pOwner->HasParent() && m_using & Using::SCALE ) m_scale *= m_pOwner->GetParent()->transform.GetWorldScale();

    for ( GameObject* const pChild : m_pOwner->GetChildren() )
    {
        if ( pChild == nullptr ) break;
        pChild->transform.m_dirty |= Dirty::ANY;
        pChild->transform.UpdateWorldScale();
    }

    if ( m_using & Using::SCALE ) UpdateWorldPosition();
}


/// @brief Updates LocalRotation from WorldRotation
void GameTransform::UpdateLocalRotation()
{
    m_transform.rotation = m_rotation;
    if ( m_pOwner->HasParent() && m_using & Using::ROTATION ) m_transform.rotation *= m_pOwner->GetParent()->transform.GetWorldRotation().GetInverse();
    UpdateLocalDirections();

    for ( GameObject* const pChild : m_pOwner->GetChildren() )
    {
        if ( pChild == nullptr ) break;
        pChild->transform.m_dirty |= Dirty::ANY;
        pChild->transform.UpdateWorldRotation();
    }
}

/// @brief Updates WorldRotation from LocalRotation
void GameTransform::UpdateWorldRotation()
{
    m_rotation = m_transform.rotation;
    if ( m_pOwner->HasParent() && m_using & Using::ROTATION ) m_rotation *= m_pOwner->GetParent()->transform.GetWorldRotation();
    UpdateWorldDirections();

    for ( GameObject* const pChild : m_pOwner->GetChildren() )
    {
        if ( pChild == nullptr ) break;
        pChild->transform.m_dirty |= Dirty::ANY;
        pChild->transform.UpdateWorldRotation();
    }

    if ( m_using & Using::ROTATION ) UpdateWorldPosition();
}


/// @brief Updates LocalDirections from LocalRotation
void GameTransform::UpdateLocalDirections()
{
    Matrix const localRotationMatrix = Matrix::Rotation( m_transform.rotation ); // There might be a better way to update right, up and forward
    m_transform.right.x = localRotationMatrix.GetValue( 0, 0 );
    m_transform.right.y = localRotationMatrix.GetValue( 0, 1 );
    m_transform.right.z = localRotationMatrix.GetValue( 0, 2 );
    m_transform.up.x = localRotationMatrix.GetValue( 1, 0 );
    m_transform.up.y = localRotationMatrix.GetValue( 1, 1 );
    m_transform.up.z = localRotationMatrix.GetValue( 1, 2 );
    m_transform.forward.x = localRotationMatrix.GetValue( 2, 0 );
    m_transform.forward.y = localRotationMatrix.GetValue( 2, 1 );
    m_transform.forward.z = localRotationMatrix.GetValue( 2, 2 );
}

/// @brief Updates WorldDirections from WorldRotation
void GameTransform::UpdateWorldDirections()
{
    Matrix const worldRotationMatrix = Matrix::Rotation( m_rotation ); // There might be a better way to update right, up and forward
    m_right.x = worldRotationMatrix.GetValue( 0, 0 );
    m_right.y = worldRotationMatrix.GetValue( 0, 1 );
    m_right.z = worldRotationMatrix.GetValue( 0, 2 );
    m_up.x = worldRotationMatrix.GetValue( 1, 0 );
    m_up.y = worldRotationMatrix.GetValue( 1, 1 );
    m_up.z = worldRotationMatrix.GetValue( 1, 2 );
    m_forward.x = worldRotationMatrix.GetValue( 2, 0 );
    m_forward.y = worldRotationMatrix.GetValue( 2, 1 );
    m_forward.z = worldRotationMatrix.GetValue( 2, 2 );
}


void GameTransform::NewParent()
{
    RemoveParent();
    UpdateLocalPosition();
    UpdateLocalScale();
    UpdateLocalRotation();
    UpdateLocalDirections();
}

void GameTransform::RemoveParent()
{
    m_transform.position = m_position;
    m_transform.scale = m_scale;
    m_transform.rotation = m_rotation;
    m_transform.right = m_right;
    m_transform.up = m_up;
    m_transform.forward = m_forward;
}


bool GameTransform::IsDirty( Dirty::Enum const flags ) const { return ( m_pOwner->HasParent() && m_pOwner->GetParent()->transform.IsDirty( flags ) ) || m_dirty & flags; }


}