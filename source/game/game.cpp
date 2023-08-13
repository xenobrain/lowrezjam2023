// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <engine/platform.h>
#include <engine/renderer.h>
#include <engine/sound.h>

#include <game/constants.h>


extern bool running;
extern float frame_time;

auto elapsed_time = 0.f;



struct nasty_t {
    virtual ~nasty_t() { xc::renderer::destroy_texture(texture); }

    virtual auto update(float frame_time) -> void {
        position = position + velocity;
    }

    virtual auto draw() -> void { xc::renderer::draw_texture(texture, region, position, angle, scale); }

    xc::rectangle_t region{};
    xc::renderer::texture_t texture{};
    xc::vector2_t position{}, velocity{};
    float angle{}, scale{1.f}, radius{8.f};
};

nasty_t* nasties[MAX_NASTIES];
auto num_nasties = 0u;


xc::renderer::texture_t projectile_texture;

struct projectile_t  {
    xc::vector2_t position, velocity;
    std::int32_t life;
} projectiles[MAX_PROJECTILES];
auto num_projectiles = 0u;

auto initialize_projectiles() {
    projectile_texture = xc::renderer::create_texture("../assets/projectile_1.png");
}

auto uninitialize_projectiles() {
    xc::renderer::destroy_texture(projectile_texture);
}

auto projectile_accumulator = 0.f;

auto update_projectiles(float frame_time) {
    projectile_accumulator += frame_time;

    while (projectile_accumulator >= 1.0f / 60.0f) {
        for (auto i = 0 ; i < num_projectiles; ++i) {
            auto projectile = &projectiles[i];

            if (projectile->life > 0) {
                projectile->life -= 1;
            }

            if (projectile->life <= 0) {
                projectiles[i] = projectiles[num_projectiles-1];
                num_projectiles -= 1u;
            }

            // Test collision
            for (auto i = 0u; i < num_nasties; ++i) {
                auto d = projectile->position - nasties[i]->position;
                auto l = std::sqrt(d.x * d.x + d.y * d.y);
                auto s = PROJECTILE_RADIUS + nasties[i]->radius;

                if (l < s) {
                    nasties[i] = nasties[num_nasties - 1];
                    num_nasties -= 1;
                }
            }
        }

        projectile_accumulator -= 1.0f / 60.0f;
    }

    // Integrate position
    for (auto i = 0u; i < num_projectiles; ++i) {
        auto projectile = &projectiles[i];
        projectile->position = projectile->position + projectile->velocity;
    }


}

auto draw_projectiles() {
    for (auto i = 0u ; i < num_projectiles; ++i) {
        auto projectile = &projectiles[i];
        xc::renderer::draw_texture(projectile_texture, {0u, 0u, 2u, 2u}, projectile->position, 0.f, 1.f);
    }
}


struct player_t {
    auto create() -> void {
        texture = xc::renderer::create_texture("../assets/player.png");
        position = xc::vector2_t{0.f, 0.f};
        velocity = xc::vector2_t{0.f, 0.f};
        angle = 0.f;

        fire_timeout = 0u;
        fire_accumulator = 0.f;
    }

    auto destroy() -> void {
        xc::renderer::destroy_texture(texture);
    }

    auto update(float frame_time) -> void {
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
    }

    auto fire(float frame_time) -> void {
        if (fire_timeout <= 0 && num_projectiles != MAX_PROJECTILES) {
            projectiles[num_projectiles].position = position;
            auto thrust = xc::vector2_t{std::cos(angle * DEG2RAD), -std::sin(angle * DEG2RAD)} * PROJECTILE_VELOCITY;
            projectiles[num_projectiles].velocity = velocity + thrust * frame_time;
            projectiles[num_projectiles].life = 360;
            num_projectiles += 1;
            fire_timeout = PLAYER_FIRE_TIMEOUT;
        }
    }

    auto draw() -> void {
        xc::renderer::draw_texture(texture, {0u, 0u, texture.w, texture.h}, position, angle, 1.f);
    }


    float angle;
    float fire_accumulator;
    std::uint32_t fire_timeout;
    xc::renderer::texture_t texture;
    xc::vector2_t position, velocity;
} static player;


struct bil_nasty : nasty_t {
    bil_nasty() {
        behavior_timer = BIL_BEHAVIOR_SWITCH_TIME;
        texture = xc::renderer::create_texture("../assets/bil.png");
        region = {0u, 0u, texture.w, texture.h};
        velocity = {0.1f, 0.1f};
    }

    auto update(float frame_time) -> void final {
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

    auto chase_player(float frame_time) -> void {
        // direction to player
        auto displacement = player.position - position;
        auto distance = std::sqrt(displacement.x * displacement.x + displacement.y * displacement.y);

        auto speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        auto time_to_intercept = distance / speed + 0.00001f;
        auto future_player_position = player.position + player.velocity * time_to_intercept;

        auto direction = xc::normalize(future_player_position - position);
        auto intercept_velocity = direction * speed;

        velocity = intercept_velocity * frame_time;
    }

    float accumulator{};
    std::int32_t behavior_timer{};
};




auto initialize_nasties() {
    // spawn a bil
    auto bil = new bil_nasty{};
    bil->position = xc::vector2_t{32.f, 0.f};

    nasties[num_nasties] = bil;
    num_nasties += 1;
}

auto uninitialize_nasties() -> void {
    for (auto i = 0u; i < num_nasties; ++i) delete nasties[i];
}

auto update_nasties(float frame_time) -> void {
    for (auto i = 0u; i < num_nasties; ++i) nasties[i]->update(frame_time);
}

auto draw_nasties() -> void {
    for (auto i = 0u; i < num_nasties; ++i) nasties[i]->draw();
}

xc::renderer::texture_t radar_texture;

auto initialize_radar() -> void {
    radar_texture = xc::renderer::create_texture("../assets/radar_bg.png");
}

auto draw_radar() -> void {
    xc::renderer::set_camera(xc::vector2_t{0.f, 0.f});
    xc::renderer::draw_texture(radar_texture, {0u, 0u, radar_texture.w, radar_texture.h}, {SCREEN_CX, static_cast<float>(radar_texture.h/2.f)}, 0.f, 1.f);
}

auto static update(float frame_time) -> void {
    elapsed_time += frame_time;

    player.update(frame_time);
    update_nasties(frame_time);
    update_projectiles(frame_time);
}


auto static draw_world() -> void {
    //xc::renderer::begin();

    xc::renderer::clear({0.f, 0.f, 0.12f, 1.f});

    xc::renderer::set_camera(xc::vector2_t{SCREEN_CX, SCREEN_CY} - player.position);

    draw_nasties();
    draw_projectiles();
    player.draw();
    //xc::renderer::end();
}



auto main(int, char**) -> int {
    if (!xc::platform::initialize(WINDOW_X, WINDOW_Y)) return -1;
    if (!xc::renderer::initialize(SCREEN_X, SCREEN_Y)) return -1;
    if (!xc::sound::initialize()) return -1;

    player.create();
    initialize_radar();
    initialize_nasties();
    initialize_projectiles();

    auto spark_texture = xc::renderer::create_texture("../assets/spark_01.png");
    auto spaceman_texture = xc::renderer::create_texture("../assets/spaceman.png");


    while (running) {
        xc::platform::tick();

        // Update
        update(frame_time);

        // Draw
        draw_world();

        xc::renderer::draw_texture(spark_texture, {0u, 0u, spark_texture.w, spark_texture.h}, {0.f, 0.f}, 0.f, 1.f);
        xc::renderer::draw_texture(spaceman_texture, {0u, 0u, spaceman_texture.w, spaceman_texture.h}, {32.f, 16.f}, 0.f, 1.f);


        draw_radar();

        xc::renderer::tick();
    }

    uninitialize_projectiles();
    uninitialize_nasties();
    player.destroy();

    xc::sound::uninitialize();
    xc::renderer::uninitialize();
    xc::platform::uninitialize();

    return 0;
}

//  Note to self--remember that textures are drawn from the center
