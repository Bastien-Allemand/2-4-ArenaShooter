#ifndef TEST_BUTTON_COMPONENT_HPP_INCLUDED
#define TEST_BUTTON_COMPONENT_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;


BEGIN_TEST(ButtonComponent)
{
    // ===== Initiating GCEngine ===== //

    WindowParam params;
    GameManager::Create();
    Scene& scene = Scene::Create();

    // ===== Initiating Render Elements ===== //

    ColorBrush redBrush    { Color::Red,     1.0f };
    ColorBrush greenBrush  { Color::Green,   1.0f };
    ColorBrush blueBrush   { Color::Blue,    1.0f };
    ColorBrush yellowBrush { Color::Yellow,  1.0f };

    BitMapBrush buttonBrush       { "res/Testing/Button/Button.png" };
    BitMapBrush button2Brush      { "res/Testing/Button/Button2.png" };
    BitMapBrush test1Brush        { "res/Textures/Test1.png" };
    BitMapBrush roundButtonBrush  { "res/Testing/Button/roundButton.png" };
    BitMapBrush roundButton2Brush { "res/Testing/Button/roundButton2.png" };

    // ===== Initiating Objects ===== //

    // First Button
    {
        GameObject& object = GameObject::Create( scene );
        object.transform.LocalTranslate( { 200.0f, 50.0f, 0.0f } );
        object.transform.LocalScale( { 216.0f, 69.0f, 1.0f } );
        UiButton& button = *object.AddComponent<UiButton>();

        button.AddListener( TestEventButton );
        button.AddListener( TestEventButton2 );

        button.pBitMapBrush = &buttonBrush;
        button.pHoverBitMapBrush = &button2Brush;

        // button.SetScale( { 0.1f, 0.1f, 0.1f } );
        // button.SetSize( { 200.0f, 80.0f } );
        // button.SetPosition( { 0.0f, 0.0f, 0.0f } );
    }

    // Second Button
    {
        // GameObject& object = GameObject::Create( scene );
        // UiButton& button = *object.AddComponent<UiButton>();
        // button.SetShape( UIButton::ROUND_RECT );
        //
        // button.AddListener( TestEventButton );
        // button.AddListener( TestEventButton2 );
        //
        // button.hoverMode = true;
        // button.pColorBrush = &greenBrush;
        // button.pBitMapBrush = &test1Brush;
        // button.pHoverBitMapBrush = &button2Brush;
        //
        // button.SetPosition( { 150.0f, 150.0f, 0.0f } );
        // button.SetSize( { 215.9f, 69.3f } );
        // button.SetScale( { 0.1f, 0.1f, 0.1f } );
        // button.SetRadius( { 40.0f, 40.0f } );
    }

    // Third Button
    {
        // GameObject& object = GameObject::Create(scene);
        // UiButton& button = *object.AddComponent<UiButton>();
        // button.SetShape( UiButton::ELLIPSE );
        //
        // button.AddListener( TestEventButton );
        // button.AddListener( TestEventButton2 );
        //
        // button.hoverMode = true;
        // button.pColorBrush = &redBrush;
        // button.pBitMapBrush = &roundButtonBrush;
        // button.pHoverBitMapBrush = &roundButton2Brush;
        //
        // button.SetPosition( { 800.0f, 350.0f, 0.0f } );
        // button.SetSize( { 252.25f, 256.0f } );
        // button.SetScale( { 0.25f, 0.25f, 0.25f } );
    }

    // Fourth Button
    {
        // GameObject& object = GameObject::Create(scene);
        // UIButton& button = *object.AddComponent<UIButton>();
        // button.SetShape( UIButton::LINE );
        //
        // button.AddListener( TestEventButton );
        // button.AddListener( TestEventButton2 );
        //
        // button.hoverMode = true;
        // button.pColorBrush = &yellowBrush;
        //
        // button.SetPosition( { 1200.0f, 0.0f, 0.0f } );
        // button.SetSize( { 1000.0f, 300.0f } );
    }

    // ===== Launching GCEngine ===== //

    GameManager::Run( params );

    GameManager::Destroy();
}
END_TEST_MAIN_FUNCTION(ButtonComponent)

static void TestEventButton()
{
    Console::Log("CLICK CLICK CLICK!!!");
}
static void TestEventButton2()
{
    Console::Log("CLACK CLACK CLACK!!!");
}

END_TEST(ButtonComponent)


#endif