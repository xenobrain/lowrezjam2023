#pragma once

#include <engine/foundation.h>
#include <engine/platform.h>
#include <engine/renderer.h>
#include <game/projectile.h>
#include <game/constants.h>

#include <random>

xc::renderer::texture_t static texture;

struct miner_t {
    xc::vector2_t position, velocity;
    float angle, angular_velocity;
    xc::rectangle_t region;
};

extern miner_t miners[MAX_MINERS];
extern std::uint32_t num_miners;

auto initialize_miners() -> void;
auto uninitialize_miners() -> void;
auto update_miners(float frame_time) -> void;
auto draw_miners() -> void;
