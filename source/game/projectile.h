#pragma once

#include <engine/foundation.h>
#include <engine/renderer.h>
#include <game/constants.h>
#include <game/nasty.h>
#include "nasty.h"

xc::renderer::texture_t static projectile_texture;

struct projectile_t  {
    xc::vector2_t position, velocity;
    std::int32_t life;
};

extern projectile_t projectiles[MAX_PROJECTILES];
extern std::uint32_t num_projectiles;

auto initialize_projectiles() -> void;
auto uninitialize_projectiles() -> void;
auto update_projectiles(float frame_time) -> void;
auto draw_projectiles() -> void;