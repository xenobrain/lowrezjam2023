#include <game/radar.h>

xc::renderer::texture_t static radar_texture;
xc::renderer::texture_t static green_dot;
xc::renderer::texture_t static blue_dot;
xc::renderer::texture_t static red_dot;

auto initialize_radar() -> void {
    radar_texture = xc::renderer::create_texture("../assets/radar_bg.png");
    green_dot = xc::renderer::create_texture("../assets/green.png");
    blue_dot = xc::renderer::create_texture("../assets/blue.png");
    red_dot = xc::renderer::create_texture("../assets/red.png");
}

auto draw_radar() -> void {
    xc::renderer::set_camera(xc::vector2_t{0.f, 0.f});

    auto static constexpr radar_min = xc::vector2_t{SCREEN_CX - RADAR_WIDTH/2.f, 0.f};
    auto static constexpr radar_max = xc::vector2_t{SCREEN_CX + RADAR_WIDTH/2.f, RADAR_HEIGHT};

    for (auto i = 0u; i < num_miners; ++i) {
        auto& miner = miners[i];

        auto x = SCREEN_CX - RADAR_WIDTH/2.f + ((miner.position.x - player.position.x) + WORLD_X) / (WORLD_X * 2.f) * RADAR_WIDTH;
        auto y = ((miner.position.y - player.position.y) +  WORLD_Y) / (WORLD_Y * 2.f) * RADAR_HEIGHT;
        auto position = xc::vector2_t{xc::clamp(x, radar_min.x, radar_max.x), xc::clamp(y, radar_min.y, radar_max.y)};

        xc::renderer::draw_texture(green_dot, {0u, 0u, 1u, 1u}, position, 0.f, 1.f);
    }

    for (auto i = 0u; i < num_nasties; ++i) {
        auto nasty = nasties[i];

        auto x = SCREEN_CX - RADAR_WIDTH/2.f + ((nasty->position.x - player.position.x) + WORLD_X) / (WORLD_X * 2.f) * RADAR_WIDTH;
        auto y = ((nasty->position.y - player.position.y) + WORLD_Y) / (WORLD_Y * 2.f) * RADAR_HEIGHT;
        auto position = xc::vector2_t{xc::clamp(x, radar_min.x, radar_max.x), xc::clamp(y, radar_min.y, radar_max.y)};

        xc::renderer::draw_texture(red_dot, {0u, 0u, 1u, 1u}, position, 0.f, 1.f);
    }

    xc::renderer::draw_texture(radar_texture, {0u, 0u, radar_texture.w, radar_texture.h}, {SCREEN_CX, static_cast<float>(radar_texture.h/2.f)}, 0.f, 1.f);
    xc::renderer::draw_texture(blue_dot,  {0u, 0u, blue_dot.w, blue_dot.h}, {SCREEN_CX, RADAR_HEIGHT/2.f}, 0.f, 1.f);
}
