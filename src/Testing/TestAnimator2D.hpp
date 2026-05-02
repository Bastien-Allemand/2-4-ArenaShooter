#ifndef TEST_ANIMATOR2D_H_INCLUDED
#define TEST_ANIMATOR2D_H_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( Animator2D )

    GameManager::Create();

    Scene& scene = Scene::Create();

    GameObject& cameraObject = GameObject::Create( scene );
    cameraObject.transform.LocalTranslate( { 0.0f, 0.0f, -10.0f } );
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.aspectRatio = 600.0f / 400.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };

    GameObject& cube = GameObject::Create( scene );
    cube.transform.SetLocalPosition({ 0, 0, 0 });
    cube.transform.LocalScale({ 0.25f, 1.0f, 0 });

    MeshRenderer& meshRenderer = *cube.AddComponent<MeshRenderer>();
    meshRenderer.SetGeometry(SHAPES.CUBE);

    Animator2D& animator = *cube.AddComponent<Animator2D>();
    animator.AddAnimation("res/Textures/metalR_albedo.jpg");
    animator.AddAnimation("res/Textures/bricks.jpg");
    animator.AddAnimation("res/Textures/stone2_albedo.jpg");
    animator.SetFrameRate(2.0f); // 2 images par seconde
    animator.SetLooping(true);

    WindowParam windowParam;
    windowParam.title = L"Split Screen Test";
    windowParam.width = 1280;
    windowParam.height = 720;
    windowParam.isFullScreen = false;
    windowParam.isSplitScreen = false;
    windowParam.screenDisposition = SplitScreenDisposition::SQUARE_4_PLAYERS;
    GameManager::Run( windowParam );

    GameManager::Destroy();
}

END_TEST( Animator2D )


#endif