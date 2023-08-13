// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <engine/platform.h>

#include <SDL.h>

auto running = true;
auto frame_time = 0.f;

SDL_Window* sdl_window;
std::uint8_t const* key_state;
SDL_GameController* gameController;

auto static previous_time = 0ull;


namespace xc::platform {
    auto initialize(std::uint32_t width, std::uint32_t height) -> bool {
        if (SDL_Init(SDL_INIT_EVENTS) < 0) {
            SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "%s", SDL_GetError());
            return false;
        }

        sdl_window = SDL_CreateWindow("Hello SDL",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  static_cast<int>(width),
                                  static_cast<int>(height),
                                  SDL_WINDOW_OPENGL);
        if (!sdl_window) {
            SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "%s", SDL_GetError());
            return false;
        }

        previous_time = SDL_GetPerformanceCounter();

        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i)) {
                gameController = SDL_GameControllerOpen(i);
                if (!gameController) {
                    SDL_LogError(SDL_LOG_CATEGORY_INPUT, "Could not initialize game controller: %s", SDL_GetError());
                    return false;
                }
            }
        }
        return true;
    }

    auto uninitialize() -> void {
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
    }

    auto tick() -> void {
        auto event = SDL_Event{};
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        key_state = SDL_GetKeyboardState(nullptr);

        auto current_time = SDL_GetPerformanceCounter();
        frame_time = static_cast<float>(static_cast<double>(current_time - previous_time) *
                (1.0 / static_cast<double>(SDL_GetPerformanceFrequency())));
        previous_time = current_time;
    }

    SDL_Scancode static key_map[] = {
            SDL_SCANCODE_A, SDL_SCANCODE_B, SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E,
            SDL_SCANCODE_F, SDL_SCANCODE_G, SDL_SCANCODE_H, SDL_SCANCODE_I, SDL_SCANCODE_J,
            SDL_SCANCODE_K, SDL_SCANCODE_L, SDL_SCANCODE_M, SDL_SCANCODE_N, SDL_SCANCODE_O,
            SDL_SCANCODE_P, SDL_SCANCODE_Q, SDL_SCANCODE_R, SDL_SCANCODE_S, SDL_SCANCODE_T,
            SDL_SCANCODE_U, SDL_SCANCODE_V, SDL_SCANCODE_W, SDL_SCANCODE_X, SDL_SCANCODE_Y,
            SDL_SCANCODE_Z, SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3,
            SDL_SCANCODE_4, SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8,
            SDL_SCANCODE_9, SDL_SCANCODE_SPACE, SDL_SCANCODE_RETURN, SDL_SCANCODE_ESCAPE,
            SDL_SCANCODE_BACKSPACE, SDL_SCANCODE_TAB, SDL_SCANCODE_LSHIFT, SDL_SCANCODE_RSHIFT,
            SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL, SDL_SCANCODE_LALT, SDL_SCANCODE_RALT,
            SDL_SCANCODE_CAPSLOCK, SDL_SCANCODE_F1, SDL_SCANCODE_F2, SDL_SCANCODE_F3,
            SDL_SCANCODE_F4, SDL_SCANCODE_F5, SDL_SCANCODE_F6, SDL_SCANCODE_F7, SDL_SCANCODE_F8,
            SDL_SCANCODE_F9, SDL_SCANCODE_F10, SDL_SCANCODE_F11, SDL_SCANCODE_F12
    };

    auto key_down(key value) -> bool {
        return key_state[key_map[static_cast<int>(value)]];
    }

    SDL_GameControllerButton static button_map[] = {
            SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B, SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y,
            SDL_CONTROLLER_BUTTON_BACK, SDL_CONTROLLER_BUTTON_GUIDE, SDL_CONTROLLER_BUTTON_START,
            SDL_CONTROLLER_BUTTON_LEFTSTICK, SDL_CONTROLLER_BUTTON_RIGHTSTICK,
            SDL_CONTROLLER_BUTTON_LEFTSHOULDER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
            SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN,
            SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT
    };

    auto button_down(button value) -> bool {
        return SDL_GameControllerGetButton(gameController, button_map[static_cast<int>(value)]);
    }

} // namespace xc::platform
