#include "pch.h"
#include "InputSystem.h"

#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "Xinput.lib")

namespace gce {


UnorderedMap<uint8, int32> InputSystem::s_keyboardMap{
    { Keyboard::BACKSPACE,       VK_BACK     },
    { Keyboard::TAB,             VK_TAB      },
    { Keyboard::RETURN,          VK_RETURN   },
    { Keyboard::PAUSE,           VK_PAUSE    },
    { Keyboard::CAPSLOCK,        VK_CAPITAL  },
    { Keyboard::ESCAPE,          VK_ESCAPE   },
    { Keyboard::SPACE,           VK_SPACE    },
    { Keyboard::PAGE_UP,         VK_PRIOR    },
    { Keyboard::PAGE_DOWN,       VK_NEXT     },
    { Keyboard::END,             VK_END      },
    { Keyboard::HOME,            VK_HOME     },
    { Keyboard::LEFT,            VK_LEFT     },
    { Keyboard::UP,              VK_UP       },
    { Keyboard::RIGHT,           VK_RIGHT    },
    { Keyboard::DOWN,            VK_DOWN     },
    { Keyboard::INSERT,          VK_INSERT   },
    { Keyboard::DELETE_,         VK_DELETE   },
    { Keyboard::LWINDOW,         VK_LWIN     },
    { Keyboard::RWINDOW,         VK_RWIN     },
    { Keyboard::NUMPAD0,         VK_NUMPAD0  },
    { Keyboard::NUMPAD1,         VK_NUMPAD1  },
    { Keyboard::NUMPAD2,         VK_NUMPAD2  },
    { Keyboard::NUMPAD3,         VK_NUMPAD3  },
    { Keyboard::NUMPAD4,         VK_NUMPAD4  },
    { Keyboard::NUMPAD5,         VK_NUMPAD5  },
    { Keyboard::NUMPAD6,         VK_NUMPAD6  },
    { Keyboard::NUMPAD7,         VK_NUMPAD7  },
    { Keyboard::NUMPAD8,         VK_NUMPAD8  },
    { Keyboard::NUMPAD9,         VK_NUMPAD9  },
    { Keyboard::NUMPAD_MULTIPLY, VK_MULTIPLY },
    { Keyboard::NUMPAD_ADD,      VK_ADD      },
    { Keyboard::NUMPAD_SUBTRACT, VK_SUBTRACT },
    { Keyboard::NUMPAD_DECIMAL,  VK_DECIMAL  },
    { Keyboard::NUMPAD_DIVIDE,   VK_DIVIDE   },
    { Keyboard::F1,              VK_F1       },
    { Keyboard::F2,              VK_F2       },
    { Keyboard::F3,              VK_F3       },
    { Keyboard::F4,              VK_F4       },
    { Keyboard::F5,              VK_F5       },
    { Keyboard::F6,              VK_F6       },
    { Keyboard::F7,              VK_F7       },
    { Keyboard::F8,              VK_F8       },
    { Keyboard::F9,              VK_F9       },
    { Keyboard::F10,             VK_F10      },
    { Keyboard::F11,             VK_F11      },
    { Keyboard::F12,             VK_F12      },
    { Keyboard::NUMLOCK,         VK_NUMLOCK  },
    { Keyboard::SCROLL_LOCK,     VK_SCROLL   },
    { Keyboard::LSHIFT,          VK_LSHIFT   },
    { Keyboard::RSHIFT,          VK_RSHIFT   },
    { Keyboard::LCONTROL,        VK_LCONTROL },
    { Keyboard::RCONTROL,        VK_RCONTROL },
    { Keyboard::LALT,            VK_LMENU    },
    { Keyboard::RALT,            VK_RMENU    },
    { Keyboard::A,              'A'          },
    { Keyboard::B,              'B'          },
    { Keyboard::C,              'C'          },
    { Keyboard::D,              'D'          },
    { Keyboard::E,              'E'          },
    { Keyboard::F,              'F'          },
    { Keyboard::G,              'G'          },
    { Keyboard::H,              'H'          },
    { Keyboard::I,              'I'          },
    { Keyboard::J,              'J'          },
    { Keyboard::K,              'K'          },
    { Keyboard::L,              'L'          },
    { Keyboard::M,              'M'          },
    { Keyboard::N,              'N'          },
    { Keyboard::O,              'O'          },
    { Keyboard::P,              'P'          },
    { Keyboard::Q,              'Q'          },
    { Keyboard::R,              'R'          },
    { Keyboard::S,              'S'          },
    { Keyboard::T,              'T'          },
    { Keyboard::U,              'U'          },
    { Keyboard::V,              'V'          },
    { Keyboard::W,              'W'          },
    { Keyboard::X,              'X'          },
    { Keyboard::Y,              'Y'          },
    { Keyboard::Z,              'Z'          },
    { Keyboard::_0,             '0'          },
    { Keyboard::_1,             '1'          },
    { Keyboard::_2,             '2'          },
    { Keyboard::_3,             '3'          },
    { Keyboard::_4,             '4'          },
    { Keyboard::_5,             '5'          },
    { Keyboard::_6,             '6'          },
    { Keyboard::_7,             '7'          },
    { Keyboard::_8,             '8'          },
    { Keyboard::_9,             '9'          },
    { Keyboard::²,              VK_OEM_7     },
};
UnorderedMap<uint8, int32> InputSystem::s_mouseMap{
    { Mouse::LEFT,   VK_LBUTTON  },
    { Mouse::RIGHT,  VK_RBUTTON  },
    { Mouse::MIDDLE, VK_MBUTTON  },
    { Mouse::EXTRA_1, VK_XBUTTON1 },
    { Mouse::EXTRA_2, VK_XBUTTON2 },
};

static constexpr WORD GAMEPAD_BUTTON_MAPPING[Gamepad::AMOUNT] = {
    XINPUT_GAMEPAD_A,
    XINPUT_GAMEPAD_B,
    XINPUT_GAMEPAD_X,
    XINPUT_GAMEPAD_Y,
    XINPUT_GAMEPAD_LEFT_SHOULDER,
    XINPUT_GAMEPAD_RIGHT_SHOULDER,
    XINPUT_GAMEPAD_BACK,
    XINPUT_GAMEPAD_START,
    XINPUT_GAMEPAD_LEFT_THUMB,
    XINPUT_GAMEPAD_RIGHT_THUMB,
    XINPUT_GAMEPAD_DPAD_UP,
    XINPUT_GAMEPAD_DPAD_DOWN,
    XINPUT_GAMEPAD_DPAD_LEFT,
    XINPUT_GAMEPAD_DPAD_RIGHT,
};


static float NormalizeStickValue(SHORT value, SHORT deadzone) {
    if (value > deadzone) {
        return static_cast<float>(value - deadzone) / (32767.0f - deadzone);
    }
    else if (value < -deadzone) {
        return static_cast<float>(value + deadzone) / (32767.0f - deadzone);
    }
    return 0.0f;
}

static float NormalizeTriggerValue( BYTE value ) { return static_cast<float>(value) / 255.0f; }


void InputSystem::HandleInputs()
{
    s_windowHandle = GetActiveWindow();
    
    for (uint8 i = 0; i < Keyboard::AMOUNT; i++)
        s_keyboardStates[i] = GetAsyncKeyState(s_keyboardMap[i]) ?
        (s_keyboardStates[i] & PRESSED ? PRESSED : DOWN) :
        (s_keyboardStates[i] & PRESSED ? UP : NONE);

    for (uint8 i = 0; i < Mouse::AMOUNT; i++)
        s_mouseStates[i] = GetAsyncKeyState(s_mouseMap[i]) ?
        (s_mouseStates[i] & PRESSED ? PRESSED : DOWN) :
        (s_mouseStates[i] & PRESSED ? UP : NONE);

    for (uint8 controllerIndex = 0; controllerIndex < MAX_CONTROLLERS; controllerIndex++) {
        XINPUT_STATE state;
        DWORD result = XInputGetState(controllerIndex, &state);

        if (result == ERROR_SUCCESS) {
            s_gamepadConnected[controllerIndex] = true;

            for (uint8 buttonIndex = 0; buttonIndex < Gamepad::AMOUNT; buttonIndex++) {
                bool isPressed = (state.Gamepad.wButtons & GAMEPAD_BUTTON_MAPPING[buttonIndex]) != 0;

                s_gamepadStates[controllerIndex][buttonIndex] = isPressed ?
                    (s_gamepadStates[controllerIndex][buttonIndex] & PRESSED ? PRESSED : DOWN) :
                    (s_gamepadStates[controllerIndex][buttonIndex] & PRESSED ? UP : NONE);
            }

            constexpr BYTE TRIGGER_DEADZONE = 30;
            s_gamepadTriggers[controllerIndex][0] = state.Gamepad.bLeftTrigger > TRIGGER_DEADZONE ?
                NormalizeTriggerValue(state.Gamepad.bLeftTrigger) : 0.0f;
            s_gamepadTriggers[controllerIndex][1] = state.Gamepad.bRightTrigger > TRIGGER_DEADZONE ?
                NormalizeTriggerValue(state.Gamepad.bRightTrigger) : 0.0f;

            constexpr SHORT STICK_DEADZONE = 7849;
            s_gamepadSticks[controllerIndex][0] = NormalizeStickValue(state.Gamepad.sThumbLX, STICK_DEADZONE);
            s_gamepadSticks[controllerIndex][1] = NormalizeStickValue(state.Gamepad.sThumbLY, STICK_DEADZONE);
            s_gamepadSticks[controllerIndex][2] = NormalizeStickValue(state.Gamepad.sThumbRX, STICK_DEADZONE);
            s_gamepadSticks[controllerIndex][3] = NormalizeStickValue(state.Gamepad.sThumbRY, STICK_DEADZONE);

        }
        else {
            s_gamepadConnected[controllerIndex] = false;

            for (uint8 buttonIndex = 0; buttonIndex < Gamepad::AMOUNT; buttonIndex++) {
                s_gamepadStates[controllerIndex][buttonIndex] = NONE;
            }
            s_gamepadTriggers[controllerIndex][0] = 0.0f;
            s_gamepadTriggers[controllerIndex][1] = 0.0f;
            s_gamepadSticks[controllerIndex][0] = 0.0f;
            s_gamepadSticks[controllerIndex][1] = 0.0f;
            s_gamepadSticks[controllerIndex][2] = 0.0f;
            s_gamepadSticks[controllerIndex][3] = 0.0f;
        }
    }
}


}