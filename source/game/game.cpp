// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <engine/platform.h>
#include <engine/renderer.h>
#include <engine/sound.h>

#include <game/constants.h>
#include <game/miner.h>
#include <game/nasty.h>
#include <game/player.h>
#include <game/projectile.h>
#include <game/radar.h>


extern bool running;
extern float frame_time;

auto elapsed_time = 0.f;

auto static initialize() -> void {
    player.create();
    initialize_radar();
    initialize_miners();
    initialize_nasties();
    initialize_projectiles();
}

auto static update(float frame_time) -> void {
    elapsed_time += frame_time;

    player.update(frame_time);
    update_miners(frame_time);
    update_nasties(frame_time);
    update_projectiles(frame_time);
}


auto static draw_world() -> void {
    //xc::renderer::begin();

    xc::renderer::clear({0.f, 0.f, 0.12f, 1.f});

    xc::renderer::set_camera(xc::vector2_t{SCREEN_CX, SCREEN_CY} - player.position);

    draw_miners();
    draw_nasties();
    draw_projectiles();
    player.draw();


    //xc::renderer::end();
}

auto main(int, char**) -> int {
    if (!xc::platform::initialize(WINDOW_X, WINDOW_Y)) return -1;
    if (!xc::renderer::initialize(SCREEN_X, SCREEN_Y)) return -1;
    if (!xc::sound::initialize()) return -1;

    initialize();

    while (running) {
        xc::platform::tick();

        // Update
        update(frame_time);

        // Draw
        xc::renderer::clear({0.f, 0.f, 0.f, 1.f});
        xc::renderer::begin();
        draw_world();
        draw_radar();
        xc::renderer::end();

        xc::renderer::tick();
    }

    uninitialize_projectiles();
    uninitialize_nasties();
    uninitialize_miners();
    player.destroy();

    xc::sound::uninitialize();
    xc::renderer::uninitialize();
    xc::platform::uninitialize();

    return 0;
}

//  Note to self--remember that textures are drawn from the center
// TODO-ish -- rotations happen around the texture center, not taking into account the current scale



// TODO:
// distribute miners
// radar
// enemies chase player
//
