#ifndef ENGINE_INPUT_SYSTEM_H_INCLUDED
#define ENGINE_INPUT_SYSTEM_H_INCLUDED

#include "define.h"
#include "InputsEnums.h"

struct HWND__;
typedef HWND__* HWND;

namespace gce {


class InputSystem final
{
public:
    static constexpr uint8 MAX_CONTROLLERS = 4;

    enum States : uint8 {
        NONE    = 0b00,
        PRESSED = 0b01,
        UP      = 0b10,
        DOWN    = 0b11
    };

private:
    static void HandleInputs();

    static UnorderedMap<uint8, int32> s_keyboardMap;
    static UnorderedMap<uint8, int32> s_mouseMap;

    inline static States s_mouseStates[Mouse::AMOUNT];
    inline static States s_keyboardStates[Keyboard::AMOUNT];
    inline static States s_gamepadStates[MAX_CONTROLLERS][Gamepad::AMOUNT];
    inline static float32 s_gamepadTriggers[MAX_CONTROLLERS][2]; 
    inline static float32 s_gamepadSticks[MAX_CONTROLLERS][4];
    inline static float32 s_gamepadVibration[MAX_CONTROLLERS][2];
    inline static bool s_gamepadConnected[MAX_CONTROLLERS];

    inline static HWND s_windowHandle = nullptr;
    inline static bool s_cursorLocked = false;
    inline static bool s_cursorVisible = true;
    inline static int32 s_cursorVisibilityCount = 0;

    friend bool GetButton( Mouse::Button button );
    friend bool GetButtonUp( Mouse::Button button );
    friend bool GetButtonDown( Mouse::Button button );

    friend Vector2i32 GetMousePosition();
    friend void SetMousePosition( Vector2i32 const& coordinates );

    friend void LockMouseCursor();
    friend void UnlockMouseCursor();
    friend bool IsMouseCursorLocked();

    friend void ShowMouseCursor();
    friend void HideMouseCursor();
    friend bool IsMouseCursorVisible();

    friend bool GetKey( Keyboard::Key key );
    friend bool GetKeyUp( Keyboard::Key key );
    friend bool GetKeyDown( Keyboard::Key key );

    friend bool GetButton( Gamepad::Button button );
    friend bool GetButton( Gamepad::Button button, uint8 controllerIndex );
    friend bool GetButtonUp( Gamepad::Button button );
    friend bool GetButtonUp( Gamepad::Button button, uint8 controllerIndex );
    friend bool GetButtonDown( Gamepad::Button button );
    friend bool GetButtonDown( Gamepad::Button button, uint8 controllerIndex );

    friend float32 GetLeftTrigger( uint8 controllerIndex );
    friend float32 GetRightTrigger( uint8 controllerIndex );
    friend float32 GetLeftStickX( uint8 controllerIndex );
    friend float32 GetLeftStickY( uint8 controllerIndex );
    friend float32 GetRightStickX( uint8 controllerIndex );
    friend float32 GetRightStickY( uint8 controllerIndex );

    friend bool IsGamepadConnected();
    friend bool IsGamepadConnected( uint8 controllerIndex );

    friend void SetGamepadVibration( float32 leftMotorSpeed, float32 rightMotorSpeed, uint8 controllerIndex );

    friend class GameManager;
};


}

#endif