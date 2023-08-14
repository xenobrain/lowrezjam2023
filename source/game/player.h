#pragma once

#include <engine/foundation.h>
#include <engine/platform.h>
#include <engine/renderer.h>
#include <game/projectile.h>
#include <game/constants.h>

struct player_t {
    auto create() -> void;
    auto destroy() -> void;
    auto update(float frame_time) -> void;
    auto fire(float frame_time) -> void;
    auto draw() -> void;

    float angle;
    float fire_accumulator;
    std::uint32_t fire_timeout;
    xc::renderer::texture_t texture;
    xc::vector2_t position, velocity;
};

extern player_t player;
