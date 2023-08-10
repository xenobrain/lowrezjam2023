// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <engine/platform.h>

#include <SDL.h>

auto running = true;

SDL_Window* sdl_window;


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
    }
} // namespace xc::platform