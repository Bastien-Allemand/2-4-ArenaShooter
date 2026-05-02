#ifndef ENGINE_INPUTS_ENUMS_H_INCLUDED
#define ENGINE_INPUTS_ENUMS_H_INCLUDED

#include "define.h"

namespace gce {


struct Mouse { enum Button: uint8 {
    LEFT, RIGHT, MIDDLE,
    EXTRA_1, EXTRA_2,
    AMOUNT,
    // Aliases
    BACKWARD = EXTRA_1,
    FORWARD = EXTRA_2,
}; };


struct Keyboard { enum Key: uint8 {
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
    NUMPAD0, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,
    NUMPAD_DIVIDE, NUMPAD_MULTIPLY, NUMPAD_SUBTRACT, NUMPAD_ADD, NUMPAD_RETURN, NUMPAD_DECIMAL,

    NUMLOCK, CAPSLOCK, SCROLL_LOCK,
    PAUSE,

    ²,

    LCTRL, RCTRL,
    LSHIFT, RSHIFT,
    LALT, RALT,
    LWINDOW, RWINDOW,

    UP, LEFT, DOWN, RIGHT,

    ESCAPE, TAB, SPACE, BACKSPACE, RETURN,
    INSERT, DELETE_,
    HOME, END,
    PAGE_UP, PAGE_DOWN,

    AMOUNT,

    // Aliases

    ESC = ESCAPE,
    SPACEBAR = SPACE,

    ENTER = RETURN,
    NUMPAD_ENTER = NUMPAD_RETURN,

    LCONTROL = LCTRL,
    RCONTROL = RCTRL,
    ALTGR = RALT,

    UP_ARROW = UP,
    LEFT_ARROW = LEFT,
    DOWN_ARROW = DOWN,
    RIGHT_ARROW = RIGHT,
}; };


struct Gamepad { enum Button : uint8 {
    A, B, X, Y,  
    LEFT_SHOULDER, RIGHT_SHOULDER,
    BACK, START,
    LEFT_THUMB, RIGHT_THUMB,
    D_PAD_UP, D_PAD_DOWN,
    D_PAD_LEFT, D_PAD_RIGHT,

    AMOUNT,

    // Aliases XBox

    LB = LEFT_SHOULDER,
    RB = RIGHT_SHOULDER,
    LS = LEFT_THUMB,
    RS = RIGHT_THUMB,

    // Aliases Playstation

    CROSS = A,
    CIRCLE = B,
    SQUARE = X,
    TRIANGLE = Y,
    L1 = LEFT_SHOULDER,
    R1 = RIGHT_SHOULDER,
    L3 = LEFT_THUMB,
    R3 = RIGHT_THUMB,
    SELECT = BACK,

    // Aliases Directional

    UP = D_PAD_UP,
    DOWN = D_PAD_DOWN,
    LEFT = D_PAD_LEFT,
    RIGHT = D_PAD_RIGHT,
}; };


}

#endif