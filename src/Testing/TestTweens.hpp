#ifndef TEST_TWEENS_HPP_INCLUDED
#define TEST_TWEENS_HPP_INCLUDED

#include <Tweens.hpp>
#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Tweens )

    WindowParam param;

    GameManager::Create();
    Scene& scene = Scene::Create();

    GameObject& cameraObject = GameObject::Create( scene );
    cameraObject.transform.LocalTranslate( { 0.0f, 0.0f, -10.0f } );
    cameraObject.AddScript<CameraScript>();
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = PI/4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };

    {
        GameObject& object = GameObject::Create( scene );
        object.SetName( "Object1" );
        object.AddScript<Object1Script>();
        MeshRenderer& meshRenderer = *object.AddComponent<MeshRenderer>();
        meshRenderer.SetGeometry(SHAPES.CUBE);
    }

    {
        GameObject& object = GameObject::Create( scene );
        object.SetName( "Object2" );
        object.transform.LocalTranslate( { 0.0f, 4.0f, 0.0f } );
        object.transform.LocalScale( { 5.0f, 1.0f, 0.5f } );
        MeshRenderer& meshRenderer = *object.AddComponent<MeshRenderer>();
        meshRenderer.SetGeometry(SHAPES.CUBE);
    }

    GameManager::Run( param );

    GameManager::Destroy();

END_TEST_MAIN_FUNCTION( Tweens )

static constexpr float32 speed = 0.03f;

DECLARE_SCRIPT( Object1Script, ScriptFlag::Start | ScriptFlag::Update )

Tween<Vector3f32>* pUpTween = nullptr;
Tween<Vector3f32>* pDownTween = nullptr;

Tween<Vector3f32>* pUpscaleTween = nullptr;
Tween<Vector3f32>* pDownscaleTween = nullptr;
bool upscaled = false;

void Start() override
{
    pUpTween = &TweenSystem::Create<Vector3f32>( { 0.0f, 0.0f, 0.0f }, { 0.0f, 2.5f, 0.0f }, Tweens::EaseOutBounce );
    pDownTween = &TweenSystem::Create<Vector3f32>( { 0.0f, 0.0f, 0.0f }, { 0.0f, -2.5f, 0.0f }, Tweens::EaseOutBounce );
    pUpscaleTween = &TweenSystem::Create<Vector3f32>( { 1.0f, 1.0f, 1.0f }, { 5.0f, 1.0f, 1.0f }, Tweens::EaseOutBounce );
    pDownscaleTween = &TweenSystem::Create<Vector3f32>( { 5.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, Tweens::EaseOutBounce );
}

void Update() override
{
    if ( GetKeyDown( Keyboard::UP ) || GetKeyDown( Keyboard::DOWN ) )
    {
        if ( upscaled ) pDownscaleTween->StartDuration( 1.0f, Function<void(GameTransform::*)(Vector3f32 const&)>( &GameTransform::SetLocalScale, &m_pOwner->transform ) );
        else pUpscaleTween->StartDuration( 1.0f, Function<void(GameTransform::*)(Vector3f32 const&)>( &GameTransform::SetLocalScale, &m_pOwner->transform ) );
        upscaled ^= true;
    }
    
    if ( GetKeyDown( Keyboard::UP ) )
    {
        pUpTween->StartDuration( 1.0f, Function<void(GameTransform::*)(Vector3f32 const&)>( &GameTransform::SetLocalPosition, &m_pOwner->transform ) );
        pUpTween->start = m_pOwner->transform.GetLocalPosition();
        pUpTween->end = pUpTween->start;
        pUpTween->end.y += 2.5f;
    }

    if ( GetKeyDown( Keyboard::DOWN ) )
    {
        pDownTween->StartDuration( 1.0f, Function<void(GameTransform::*)(Vector3f32 const&)>( &GameTransform::SetLocalPosition, &m_pOwner->transform ) );
        pDownTween->start = m_pOwner->transform.GetLocalPosition();
        pDownTween->end = pDownTween->start;
        pDownTween->end.y -= 2.5f;
    }

    if ( GetKey( Keyboard::RIGHT ) )
    {
        m_pOwner->transform.LocalTranslate( { speed, 0.0f, 0.0f } );
        pUpTween->start.x += speed;
        pUpTween->end.x += speed;
        pDownTween->start.x += speed;
        pDownTween->end.x += speed;
    }

    if ( GetKey( Keyboard::LEFT ) )
    {
        m_pOwner->transform.LocalTranslate( { -speed, 0.0f, 0.0f } );
        pUpTween->start.x -= speed;
        pUpTween->end.x -= speed;
        pDownTween->start.x -= speed;
        pDownTween->end.x -= speed;
    }
}

END_SCRIPT

DECLARE_SCRIPT( CameraScript, ScriptFlag::Start | ScriptFlag::Update )

Tween<Vector3f32>* pTranslateTween = nullptr;
Tween<Vector3f32>* pRotateTween = nullptr;

bool moved = true;

void Start() override
{
    pTranslateTween = &TweenSystem::Create<Vector3f32>( { 0.0f, 0.0f, -10.0f }, { 0.0f, 10.0f, 0.0f }, Tweens::Linear );
    pRotateTween = &TweenSystem::Create<Vector3f32>( { 0.0f, 0.0f, 0.0f }, { PI/2, 0.0f, 0.0f }, Tweens::EaseInSine );
}

void Update() override
{
    if ( GetKeyDown( Keyboard::SPACE ) == false ) return;
    moved ^= true;
    if ( moved )
    {
        pTranslateTween->StartDuration( 1.5f, Function<void(GameTransform::*)(Vector3f32 const&)>( &GameTransform::SetLocalPosition, &m_pOwner->transform ), true );
        pRotateTween->StartDuration( 1.5f, Function<void(GameTransform::*)(Vector3f32 const&)>( &GameTransform::SetLocalRotation, &m_pOwner->transform ), true );
        return;
    }
    pTranslateTween->StartDuration( 1.5f, Function<void(GameTransform::*)(Vector3f32 const&)>( &GameTransform::SetLocalPosition, &m_pOwner->transform ) );
    pRotateTween->StartDuration( 1.5f, Function<void(GameTransform::*)(Vector3f32 const&)>( &GameTransform::SetLocalRotation, &m_pOwner->transform ) );
}

END_SCRIPT

END_TEST( Tweens )


#endif