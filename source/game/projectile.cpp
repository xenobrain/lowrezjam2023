#include <game/projectile.h>

auto static projectile_accumulator = 0.f;

projectile_t projectiles[MAX_PROJECTILES];
std::uint32_t num_projectiles = 0u;

auto initialize_projectiles() -> void {
    projectile_texture = xc::renderer::create_texture("../assets/projectile_1.png");
}

auto uninitialize_projectiles() -> void {
    xc::renderer::destroy_texture(projectile_texture);
}

auto update_projectiles(float frame_time) -> void {
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
            for (auto j = 0u; j < num_nasties; ++j) {
                auto d = projectile->position - nasties[j]->position;
                auto l = std::sqrt(d.x * d.x + d.y * d.y);
                auto s = PROJECTILE_RADIUS + nasties[j]->radius;

                if (l < s) {
                    nasties[j] = nasties[num_nasties - 1];
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

auto draw_projectiles() -> void {
    for (auto i = 0u ; i < num_projectiles; ++i) {
        auto projectile = &projectiles[i];
        xc::renderer::draw_texture(projectile_texture, {0u, 0u, 2u, 2u}, projectile->position, 0.f, 1.f);
    }
}
