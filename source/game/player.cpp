#include <game/player.h>
#include <game/miner.h>

player_t player;

auto player_t::create() -> void {
    texture = xc::renderer::create_texture("../assets/player.png");
    position = xc::vector2_t{0.f, 0.f};
    velocity = xc::vector2_t{0.f, 0.f};
    angle = 0.f;

    fire_timeout = 0u;
    fire_accumulator = 0.f;
}

auto player_t::destroy() -> void {
    xc::renderer::destroy_texture(texture);
}

auto player_t::update(float frame_time) -> void {
    auto static accumulator = 0.f;
    fire_accumulator += frame_time;

    while (fire_accumulator >= 1.0f / 60.0f) {
        fire_accumulator -= 1.0f / 60.0f;
        fire_timeout = fire_timeout > 0u ? fire_timeout - 1u : 0u;
    }

    if (xc::platform::key_down(key::A)) angle -= 100.f * frame_time;
    if (xc::platform::key_down(key::D)) angle += 100.f * frame_time;
    auto thrust = xc::vector2_t{std::cos(angle * DEG2RAD), -std::sin(angle * DEG2RAD)} * frame_time;

    if (xc::platform::key_down(key::W)) velocity = velocity + thrust;
    if (xc::platform::key_down(key::S)) velocity = velocity - thrust;

    if (xc::platform::key_down(key::SPACE)) fire(frame_time);

    velocity.x = xc::clamp(velocity.x, -PLAYER_MAX_VELOCITY, PLAYER_MAX_VELOCITY);
    velocity.y = xc::clamp(velocity.y, -PLAYER_MAX_VELOCITY, PLAYER_MAX_VELOCITY);

    position = position + velocity;
    position.x = xc::clamp(position.x, -WORLD_X, WORLD_X);
    position.y = xc::clamp(position.y, -WORLD_Y, WORLD_Y);
    if (position.x == -WORLD_X || position.x == WORLD_X) velocity.x = 0.f;
    if (position.y == -WORLD_Y || position.y == WORLD_Y) velocity.y = 0.f;

    // Test collision with miners
    for (auto i = 0u; i < num_miners; ++i) {
        auto& miner = miners[i];
        auto delta = position - miner.position;
        auto constexpr radii_sum = PLAYER_RADIUS + MINER_RADIUS;
        auto distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (distance < radii_sum) {
            miners[i] = miners[num_miners - 1];
            num_miners -= 1;
        }
    }
}

auto player_t::fire(float frame_time) -> void {
    if (fire_timeout <= 0 && num_projectiles != MAX_PROJECTILES) {
        projectiles[num_projectiles].position = position;
        auto thrust = xc::vector2_t{std::cos(angle * DEG2RAD), -std::sin(angle * DEG2RAD)} * PROJECTILE_VELOCITY;
        projectiles[num_projectiles].velocity = velocity + thrust * frame_time;
        projectiles[num_projectiles].life = 360;
        num_projectiles += 1;
        fire_timeout = PLAYER_FIRE_TIMEOUT;
    }
}

auto player_t::draw() -> void {
    xc::renderer::draw_texture(texture, {0u, 0u, texture.w, texture.h}, position, angle, 1.f);
}
