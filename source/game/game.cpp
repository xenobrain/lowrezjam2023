// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <engine/platform.h>
#include <engine/renderer.h>

#include <game/constants.h>


extern bool running;


auto main(int, char**) -> int {
    if (!xc::platform::initialize(WINDOW_X, WINDOW_Y)) return -1;
    if (!xc::renderer::initialize(SCREEN_X, SCREEN_Y)) return -1;

    auto player_texture = xc::renderer::create_texture("../assets/player.png");
    auto spark_texture =  xc::renderer::create_texture("../assets/spark_01.png");

    while (running) {
        xc::platform::tick();

        xc::renderer::clear({0.f, 0.f, 0.12f, 1.f});
        xc::renderer::draw_texture(player_texture, {0u, 0u, player_texture.w, player_texture.h}, glm::vec2(32.f, 32.f), 0.f, 1.f);
        xc::renderer::draw_texture(spark_texture, {0,0,spark_texture.w, spark_texture.h}, glm::vec2(0.f, 0.f), 0.f, 1.f);

        xc::renderer::tick();
    }

    xc::renderer::destroy_texture(spark_texture);
    xc::renderer::destroy_texture(player_texture);

    xc::renderer::uninitialize();
    xc::platform::uninitialize();

    return 0;
}