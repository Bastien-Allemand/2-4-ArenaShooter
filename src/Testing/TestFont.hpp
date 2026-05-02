#ifndef TESTING_TEST_FONT_HPP_INCLUDED
#define TESTING_TEST_FONT_HPP_INCLUDED

#include <Windows.h>
#include <Render.h>
#include <Engine.h>
#include <Shapes.h>
#include <Font.h>
#include "Test.h"

using namespace gce;

BEGIN_TEST(Font)
{
	WindowParam windowParam;
	windowParam.title = L"Test Fonts";

	GameManager::Create();
	Scene& scene = Scene::Create();

	GameObject& cameraObject = GameObject::Create(scene);
	cameraObject.AddScript<FontScript>();
	cameraObject.transform.LocalTranslate({ 0, 0.0f, -5 });
	Camera& camera = *cameraObject.AddComponent<Camera>();
	camera.SetMainCamera();
	camera.SetType(ORTHOGRAPHIC); // PERSPECTIVE  ORTHOGRAPHIC
	camera.orthographic.viewWidth = 3.4f;
	//camera.orthographic.viewHeight = 80;
	camera.orthographic.farPlane = 500.0f;
	camera.orthographic.nearPlane = 0.01f;

	// ========== SECTION 1:  ==========

	GameObject& staticCircle = GameObject::Create(scene);
	MeshRenderer& meshRendererStaticCircle = *staticCircle.AddComponent<MeshRenderer>();
	meshRendererStaticCircle.SetGeometry(SHAPES.SPHERE);

	staticCircle.transform.SetLocalPosition(Vector3f32(0.0f, 0.0f, 0.0f));
	staticCircle.transform.LocalScale(Vector3f32(0.5f, 0.5f, 0.5f));

	// ========== UI ==========
	Font font(L"Arial");
	//ColorBrush colorBrush(Color::Red, 1.0f);
	String const& path = "res/Textures/bricks.jpg";
	BitMapBrush colorBrush( path );
	colorBrush.SetTransformMatrix( { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f );

	GameObject& textObject = GameObject::Create(scene);
	TextRenderer& textRenderer = *textObject.AddComponent<TextRenderer>();
	textRenderer.pFont = &font;
	textRenderer.pBrush = &colorBrush;
	textRenderer.text = L"Hello World!";
	textRenderer.rectPosF = new RectanglePosF(500.0f, 500.0f, 1000.0f, 100.0f);

	GameManager::Run(windowParam);
	GameManager::Destroy();
}

END_TEST_MAIN_FUNCTION( Font )

DECLARE_SCRIPT( FontScript, ScriptFlag::Update )

void Update() override
{
	if ( GetKey( Keyboard::LEFT ) ) m_pOwner->transform.LocalTranslate( { -0.1f, 0.0f, 0.0f } );
	if ( GetKey( Keyboard::RIGHT ) ) m_pOwner->transform.LocalTranslate( { 0.1f, 0.0f, 0.0f } );
	if ( GetKey( Keyboard::UP ) ) m_pOwner->transform.LocalTranslate( { 0.0f, 0.1f, 0.0f } );
	if ( GetKey( Keyboard::DOWN ) ) m_pOwner->transform.LocalTranslate( { 0.0f, -0.1f, 0.0f } );
}

END_SCRIPT

END_TEST( Font )

#endif 