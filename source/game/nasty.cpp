#include <engine/renderer.h>
#include <game/nasty.h>

#include <random>

nasty_t* nasties[MAX_NASTIES];
std::uint32_t num_nasties = 0u;

xc::renderer::texture_t static nasty_projectile_texture;
nasty_projectile_t nasty_projectiles[MAX_PROJECTILES];
std::uint32_t num_nasty_projectiles = 0u;


std::random_device rd;
std::default_random_engine gen(rd());
std::uniform_real_distribution<float> disX(-WORLD_X, WORLD_X);
std::uniform_real_distribution<float> disY(-WORLD_Y, WORLD_Y);
std::uniform_real_distribution<float> disV(-10.f, 10.f);

auto nasty_t::update(float frame_time) -> void {
    position = position + velocity * frame_time;
}

auto nasty_t::draw() -> void { xc::renderer::draw_texture(texture, region, position, angle, scale); }

bil_nasty::bil_nasty() {
    behavior_timer = BIL_BEHAVIOR_SWITCH_TIME;
    texture = xc::renderer::create_texture("../assets/bil.png");
    region = {0u, 0u, texture.w, texture.h};
    velocity = {disV(gen), disV(gen)};
}

auto bil_nasty::update(float frame_time) -> void {
    accumulator += frame_time;
    while (accumulator >= 1.0f / 60.0f) {
        behavior_timer -= 1;

        if (behavior_timer <= 0) {
            chase_player(frame_time);
            behavior_timer = BIL_BEHAVIOR_SWITCH_TIME;
        }

        accumulator -= 1.0f / 60.0f;
    }

    nasty_t::update(frame_time);
}

auto bil_nasty::chase_player(float frame_time) -> void {
    // direction to player
    auto displacement = player.position - position;
    auto distance = std::sqrt(displacement.x * displacement.x + displacement.y * displacement.y);

    auto speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    auto time_to_intercept = distance / speed;
    auto future_player_position = player.position + player.velocity * time_to_intercept;

    auto direction = xc::normalize(future_player_position - position);
    auto intercept_velocity = direction * speed;

    velocity = intercept_velocity;
}

auto initialize_nasties() -> void {
    // spawn a bil
    auto bil = new bil_nasty{};
    bil->position = xc::vector2_t{32.f, 0.f};

    nasties[num_nasties] = bil;
    num_nasties += 1;

    nasty_projectile_texture = xc::renderer::create_texture("../assets/projectile_green.png");
}

auto uninitialize_nasties() -> void {
    for (auto i = 0u; i < num_nasties; ++i) delete nasties[i];
}

auto update_nasties(float frame_time) -> void {
    auto static spawn_accumulator = 0.f;
    spawn_accumulator += frame_time;

    while (spawn_accumulator >= NASTY_SPAWN_DELAY) {
        if (num_nasties < MAX_NASTIES) {
            auto bil = new bil_nasty();

            float x = disX(gen);
            float y = disY(gen);
            float distance = std::sqrt((x - player.position.x) * (x - player.position.x) + (y - player.position.y) * (y - player.position.y));
            while (distance < 42.f) {
                x = disX(gen);
                y = disY(gen);
                distance = std::sqrt((x - player.position.x) * (x - player.position.x) + (y - player.position.y) * (y - player.position.y));
            }

            bil->position = {x, y};

            nasties[num_nasties] = bil;
            num_nasties += 1;
        }

        spawn_accumulator -= NASTY_SPAWN_DELAY;
    }


    auto static fire_accumulator = 0.f;
    fire_accumulator += frame_time;

    while (fire_accumulator >= NASTY_PROJECTILE_DELAY) {
        fire_accumulator -= NASTY_PROJECTILE_DELAY;

        nasty_t* nearest;
        auto closest = WORLD_X;

        for (auto i = 0u; i < num_nasties; ++i) {
            if (num_nasty_projectiles == MAX_PROJECTILES) {
                num_nasty_projectiles -= 1;
                nasty_projectiles[0] = nasty_projectiles[num_nasty_projectiles];
            }

            auto nasty = nasties[i];
            auto direction = xc::normalize(player.position - nasty->position);
            auto distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (distance < closest) {
                closest = distance;
                nearest = nasty;
            }

            direction = xc::normalize(player.position - nearest->position);
            nasty_projectiles[num_nasty_projectiles].position = nearest->position;
            nasty_projectiles[num_nasty_projectiles].velocity = direction * 10.f;
            num_nasty_projectiles += 1;
        }
    }

    for (auto i = 0u; i < num_nasties; ++i) {
        nasties[i]->update(frame_time);
    }

    for (auto i = 0u; i < num_nasty_projectiles; ++i) {
        auto& projectile = nasty_projectiles[i];
        projectile.position = projectile.position + projectile.velocity * frame_time;
    }
}

auto draw_nasties() -> void {
    for (auto i = 0u; i < num_nasties; ++i) nasties[i]->draw();

    for (auto i = 0u; i < num_nasty_projectiles; ++i) {
        auto& projectile = nasty_projectiles[i];
        xc::renderer::draw_texture(nasty_projectile_texture,
                                   {0u, 0u, nasty_projectile_texture.w, nasty_projectile_texture.h},
                                   projectile.position,
                                   0.f,
                                   1.f);
    }
}
