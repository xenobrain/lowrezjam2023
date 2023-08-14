#pragma once

#include <engine/foundation.h>
#include <engine/renderer.h>
#include <game/constants.h>
#include <game/player.h>

struct nasty_t {
    virtual ~nasty_t() { xc::renderer::destroy_texture(texture); }

    virtual auto update(float frame_time) -> void;
    virtual auto draw() -> void;

    xc::rectangle_t region{};
    xc::renderer::texture_t texture{};
    xc::vector2_t position{}, velocity{};
    float angle{}, scale{1.f}, radius{8.f};
};

struct bil_nasty : nasty_t {
    bil_nasty();

    auto update(float frame_time) -> void final;
    auto chase_player(float frame_time) -> void;

    float accumulator{};
    std::int32_t behavior_timer{};
};

struct nasty_projectile_t {
    xc::vector2_t position, velocity;
    std::int32_t ticks;
};


auto initialize_nasties() -> void;
auto uninitialize_nasties() -> void;
auto update_nasties(float frame_time) -> void;
auto draw_nasties() -> void;

extern nasty_projectile_t nasty_projectiles[MAX_PROJECTILES];
extern std::uint32_t num_nasty_projectiles;

extern nasty_t* nasties[MAX_NASTIES];
extern std::uint32_t num_nasties;