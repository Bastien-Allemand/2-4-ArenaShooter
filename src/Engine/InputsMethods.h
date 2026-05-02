#ifndef ENGINE_INPUTS_METHODS_H_INCLUDED
#define ENGINE_INPUTS_METHODS_H_INCLUDED

#include <Maths/Vector2.hpp>
#include "InputsEnums.h"
#include "Systems/InputSystem.h"

namespace gce {


// Mouse
[[nodiscard]] inline bool GetButton( Mouse::Button const button ) { return InputSystem::s_mouseStates[button] == InputSystem::States::PRESSED; }
[[nodiscard]] inline bool GetButtonUp( Mouse::Button const button ) { return InputSystem::s_mouseStates[button] == InputSystem::States::UP; }
[[nodiscard]] inline bool GetButtonDown( Mouse::Button const button ) { return InputSystem::s_mouseStates[button] == InputSystem::States::DOWN; }

[[nodiscard]] Vector2i32 GetMousePosition();
void SetMousePosition( Vector2i32 const& coordinates );

void LockMouseCursor();
void UnlockMouseCursor();
[[nodiscard]] inline bool IsMouseCursorLocked() { return InputSystem::s_cursorLocked; }         

void ShowMouseCursor();
void HideMouseCursor();
[[nodiscard]] inline bool IsMouseCursorVisible() { return InputSystem::s_cursorVisible; }

// Keyboard
[[nodiscard]] inline bool GetKey( Keyboard::Key const key ) { return InputSystem::s_keyboardStates[key] & InputSystem::States::PRESSED; }
[[nodiscard]] inline bool GetKeyUp( Keyboard::Key const key ) { return InputSystem::s_keyboardStates[key] == InputSystem::States::UP; }
[[nodiscard]] inline bool GetKeyDown( Keyboard::Key const key ) { return InputSystem::s_keyboardStates[key] == InputSystem::States::DOWN; }

// Gamepad
[[nodiscard]] inline bool GetButton( Gamepad::Button const button ) { for ( InputSystem::States const (&state)[Gamepad::AMOUNT] : InputSystem::s_gamepadStates ) if ( state[button] & InputSystem::States::PRESSED ) return true; return false; }
[[nodiscard]] inline bool GetButton( Gamepad::Button const button, uint8 const controllerIndex ) { return controllerIndex < InputSystem::MAX_CONTROLLERS && InputSystem::s_gamepadStates[controllerIndex][button] & InputSystem::States::PRESSED; }
[[nodiscard]] inline bool GetButtonUp( Gamepad::Button const button ) { for ( InputSystem::States const (&state)[Gamepad::AMOUNT] : InputSystem::s_gamepadStates ) if ( state[button] == InputSystem::States::UP ) return true; return false; }
[[nodiscard]] inline bool GetButtonUp( Gamepad::Button const button, uint8 const controllerIndex ) { return controllerIndex < InputSystem::MAX_CONTROLLERS && InputSystem::s_gamepadStates[controllerIndex][button] == InputSystem::States::UP; }
[[nodiscard]] inline bool GetButtonDown( Gamepad::Button const button ) { for ( InputSystem::States const (&state)[Gamepad::AMOUNT] : InputSystem::s_gamepadStates ) if ( state[button] == InputSystem::States::DOWN ) return true; return false; }
[[nodiscard]] inline bool GetButtonDown( Gamepad::Button const button, uint8 const controllerIndex ) { return controllerIndex < InputSystem::MAX_CONTROLLERS && InputSystem::s_gamepadStates[controllerIndex][button] == InputSystem::States::DOWN; }

[[nodiscard]] inline float32 GetLeftTrigger( uint8 const controllerIndex = 0 ) { return controllerIndex < InputSystem::MAX_CONTROLLERS ? InputSystem::s_gamepadTriggers[controllerIndex][0] : 0.0f; }
[[nodiscard]] inline float32 GetRightTrigger( uint8 const controllerIndex = 0 ) { return controllerIndex < InputSystem::MAX_CONTROLLERS ? InputSystem::s_gamepadTriggers[controllerIndex][1] : 0.0f; }

[[nodiscard]] inline float32 GetLeftStickX( uint8 const controllerIndex = 0 ) { return controllerIndex < InputSystem::MAX_CONTROLLERS ? InputSystem::s_gamepadSticks[controllerIndex][0] : 0.0f; }
[[nodiscard]] inline float32 GetLeftStickY( uint8 const controllerIndex = 0 ) { return controllerIndex < InputSystem::MAX_CONTROLLERS ? InputSystem::s_gamepadSticks[controllerIndex][1] : 0.0f; }
[[nodiscard]] inline Vector2f32 GetLeftStick( uint8 const controllerIndex = 0 ) { return { GetLeftStickX( controllerIndex ), GetLeftStickY( controllerIndex ) }; }

[[nodiscard]] inline float32 GetRightStickX( uint8 const controllerIndex = 0 ) { return controllerIndex < InputSystem::MAX_CONTROLLERS ? InputSystem::s_gamepadSticks[controllerIndex][2] : 0.0f; }
[[nodiscard]] inline float32 GetRightStickY( uint8 const controllerIndex = 0 ) { return controllerIndex < InputSystem::MAX_CONTROLLERS ? InputSystem::s_gamepadSticks[controllerIndex][3] : 0.0f; }
[[nodiscard]] inline Vector2f32 GetRightStick( uint8 const controllerIndex = 0 ) { return { GetRightStickX( controllerIndex ), GetRightStickY( controllerIndex ) }; }

[[nodiscard]] inline bool IsGamepadConnected() { for ( bool const connected : InputSystem::s_gamepadConnected ) if ( connected ) return true; return false; }
[[nodiscard]] inline bool IsGamepadConnected( uint8 const controllerIndex ) { return controllerIndex < InputSystem::MAX_CONTROLLERS && InputSystem::s_gamepadConnected[controllerIndex]; }

void SetGamepadVibration( float32 leftMotorSpeed, float32 rightMotorSpeed, uint8 controllerIndex = 0 );
inline void StopGamepadVibration( uint8 const controllerIndex = 0 ) { SetGamepadVibration( 0.0f, 0.0f, controllerIndex ); }


}

#endif