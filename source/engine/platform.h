#pragma once

#include <engine/foundation.h>

enum class key {
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    NUM_0, NUM_1, NUM_2, NUM_3, NUM_4,
    NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
    SPACE, ENTER, ESCAPE, BACKSPACE, TAB,
    L_SHIFT, R_SHIFT, L_CTRL, R_CTRL,
    L_ALT, R_ALT, CAPS_LOCK, F1, F2, F3, F4, F5,
    F6, F7, F8, F9, F10, F11, F12
};

enum class button {
    A, B, X, Y,
    BACK, GUIDE, START,
    LEFT_STICK, RIGHT_STICK,
    LEFT_SHOULDER, RIGHT_SHOULDER,
    DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT
};


namespace xc::platform {
    auto initialize(std::uint32_t width, std::uint32_t height) -> bool;
    auto uninitialize() -> void;

    auto tick() -> void;

    auto key_down(key value) -> bool;
    auto button_down(button value) -> bool;
} // namespace xc::platform
