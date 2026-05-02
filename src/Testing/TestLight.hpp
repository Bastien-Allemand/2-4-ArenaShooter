#ifndef TEST_LIGHT_HPP_INCLUDED
#define TEST_LIGHT_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;

DECLARE_SCRIPT(RotateScript, ScriptFlag::Update)

void Update(){ m_pOwner->transform.LocalRotate({ 0.0f, 0.008f, 0.008f }); }

END_SCRIPT



BEGIN_TEST( Light )

    GameManager::Create();
    Scene& scene = Scene::Create();
    
    Texture textureMetalAlbedo("res/Textures/metalR_albedo.jpg");
    Texture textureMetalNormal("res/Textures/metalR_normal.jpg");
    Texture textureMetalAmbient("res/Textures/metalR_ambient.jpg");
    Texture textureMetalRoughness("res/Textures/metalR_roughness.jpg");
    Texture textureMetalMetalness("res/Textures/metalR_metalness.jpg");
    Texture textureMetalDisplacement("res/Textures/metalR_displacement.jpg");

    GameObject& cameraObject = GameObject::Create( scene );
    cameraObject.transform.LocalTranslate( { 0.0f, 0.0f, -10.0f } );
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetMainCamera();
    camera.SetType( PERSPECTIVE );
    camera.perspective.fov = PI/4;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.nearPlane = 0.001f;

    GameObject& directionalLightObject = GameObject::Create( scene );
    Light& directionalLight = *directionalLightObject.AddComponent<Light>();
    directionalLight.DefaultDirectionLight();
    directionalLight.color = { 0.0f, 1.0f, 0.0f, 1.0f };

    GameObject& redPointLightObject = GameObject::Create( scene );
    Light& redPointLight = *redPointLightObject.AddComponent<Light>();
    redPointLight.DefaultPointLight();
    redPointLight.position = { 0.0f, 1.0f, 0.0f };
    redPointLight.color = { 1.0f, 0.0f, 0.0f, 1.0f };
    redPointLight.rimLightColor = { 1.0f, 0.0f, 0.0f };

    GameObject& donutObject = GameObject::Create( scene );
    donutObject.AddScript<RotateScript>();
    donutObject.transform.LocalTranslate( { -2.0f, 0.0f, 1.0f } );
    donutObject.transform.LocalRotate( { PI/7.2f, PI/4.0f, PI/4.0f } );
    donutObject.transform.LocalScale( { 1.5f, 1.5f,  1.5f } );
    MeshRenderer& donutMeshRenderer = *donutObject.AddComponent<MeshRenderer>();
    donutMeshRenderer.SetGeometry(SHAPES.DONUT);

    donutMeshRenderer.SetAlbedoTexture(&textureMetalAlbedo);
    donutMeshRenderer.SetAmbientTexture(&textureMetalAmbient);
    donutMeshRenderer.SetRoughnessTexture(&textureMetalRoughness);
    donutMeshRenderer.SetNormalTexture(&textureMetalNormal);
    donutMeshRenderer.SetDisplacementTexture(&textureMetalDisplacement);
    donutMeshRenderer.SetFresnelR0({ 0.972f, 0.960f, 0.915f });
    donutMeshRenderer.SetMetalnessTexture(&textureMetalMetalness);

    donutMeshRenderer.SetSubsurfaceScattering(0.2f);

    GameObject& capsuleObject = GameObject::Create( scene );
    capsuleObject.AddScript<RotateScript>();
    capsuleObject.transform.LocalTranslate( { 3.0f, 0.0f, 1.0f } );
    capsuleObject.transform.LocalRotate( { PI/5.0f, 0.0f, 0.0f } );
    capsuleObject.transform.LocalScale( { 1.5f, 1.5f,  1.5f } );
    MeshRenderer& capsuleMeshRenderer = *capsuleObject.AddComponent<MeshRenderer>();
    capsuleMeshRenderer.SetGeometry(SHAPES.CAPSULE);

    capsuleMeshRenderer.SetAlbedoTexture(&textureMetalAlbedo);
    capsuleMeshRenderer.SetAmbientTexture(&textureMetalAmbient);
    capsuleMeshRenderer.SetRoughnessTexture(&textureMetalRoughness);
    capsuleMeshRenderer.SetNormalTexture(&textureMetalNormal);
    capsuleMeshRenderer.SetDisplacementTexture(&textureMetalDisplacement);
    capsuleMeshRenderer.SetMetalnessTexture(&textureMetalMetalness);

    capsuleMeshRenderer.SetSubsurfaceScattering(0.2f);
    
    Console::Log("WindowParam Create", Console::BLUE);
    WindowParam windowParam;
    windowParam.title = L"Test Light";
    windowParam.width = 1080;
    windowParam.height = 720;
    windowParam.isFullScreen = true;
    
    GameManager::Run( windowParam );

    GameManager::Destroy();

END_TEST_MAIN_FUNCTION( Light )
END_TEST( Light )


#endif