#include "pch.h"
#include "InputsMethods.h"

#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

namespace gce {


Vector2i32 GetMousePosition()
{
    POINT p;
    GetCursorPos( &p );
    ScreenToClient( InputSystem::s_windowHandle, &p );
    return { p.x, p.y };
}

void SetMousePosition( Vector2i32 const& coordinates )
{
    POINT p{ coordinates.x, coordinates.y };
    ClientToScreen( InputSystem::s_windowHandle, &p );
    SetCursorPos( p.x, p.y );
}


void LockMouseCursor()
{
    if ( InputSystem::s_windowHandle == nullptr ) return;
    InputSystem::s_cursorLocked = true;
    
    if ( InputSystem::s_cursorLocked == false || InputSystem::s_windowHandle == nullptr ) return;
    
    RECT clientRect;
    if ( GetClientRect( InputSystem::s_windowHandle, &clientRect ) == false ) return;

    POINT topLeft = { clientRect.left, clientRect.top };
    POINT bottomRight = { clientRect.right, clientRect.bottom };

    ClientToScreen( InputSystem::s_windowHandle, &topLeft );
    ClientToScreen( InputSystem::s_windowHandle, &bottomRight );

    RECT const clipRect = { topLeft.x, topLeft.y, bottomRight.x, bottomRight.y };
    ClipCursor( &clipRect );
}

void UnlockMouseCursor()
{
    InputSystem::s_cursorLocked = false;
    ClipCursor( nullptr );
}


void ShowMouseCursor()
{
    if ( InputSystem::s_cursorVisible ) return;
    InputSystem::s_cursorVisible = true;

    while ( InputSystem::s_cursorVisibilityCount < 0 )
        InputSystem::s_cursorVisibilityCount = ShowCursor( TRUE );
}

void HideMouseCursor()
{
    if ( InputSystem::s_cursorVisible == false ) return;
    InputSystem::s_cursorVisible = false;

    while ( InputSystem::s_cursorVisibilityCount >= 0 )
        InputSystem::s_cursorVisibilityCount = ShowCursor( FALSE );
}


void SetGamepadVibration( float32 leftMotorSpeed, float32 rightMotorSpeed, uint8 const controllerIndex )
{
    if ( controllerIndex >= InputSystem::MAX_CONTROLLERS ) return;

    leftMotorSpeed = leftMotorSpeed < 0.0f ? 0.0f : leftMotorSpeed > 1.0f ? 1.0f : leftMotorSpeed;
    rightMotorSpeed = rightMotorSpeed < 0.0f ? 0.0f : rightMotorSpeed > 1.0f ? 1.0f : rightMotorSpeed;

    InputSystem::s_gamepadVibration[controllerIndex][0] = leftMotorSpeed;
    InputSystem::s_gamepadVibration[controllerIndex][1] = rightMotorSpeed;

    XINPUT_VIBRATION vibration;
    vibration.wLeftMotorSpeed = static_cast<uint16>(leftMotorSpeed * 65535.0f);
    vibration.wRightMotorSpeed = static_cast<uint16>(rightMotorSpeed * 65535.0f);

    XInputSetState( controllerIndex, &vibration );
}


}