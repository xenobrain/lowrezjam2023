#include <game/miner.h>


miner_t miners[MAX_MINERS];
std::uint32_t num_miners = MAX_MINERS;

auto uninitialize_miners() -> void {
    xc::renderer::destroy_texture(texture);
}

auto update_miners(float frame_time) -> void {
    auto static accumulator = 0.f;
    accumulator += frame_time;

    while (accumulator >= MINER_FRAME_TIME) {
        for (auto i = 0u; i < num_miners; ++i) {
            auto& miner = miners[i];
            miner.region.x = miner.region.x = (miner.region.x + miner.region.w) % texture.w;
        }

        accumulator -= MINER_FRAME_TIME;
    }

    for (auto i = 0u; i < num_miners; ++i) {
        auto& miner = miners[i];
        miner.angle += miner.angular_velocity;
        miner.angle = fmod(miner.angle, 360.f);
    }
}

auto draw_miners() -> void {
    for (auto i = 0u; i < num_miners; ++i) {
        auto& miner = miners[i];
        xc::renderer::draw_texture(texture, miner.region, miner.position, miner.angle, 1.f);
    }
}

auto initialize_miners() -> void {
    texture = xc::renderer::create_texture("../assets/spaceman.png");
    num_miners = MAX_MINERS;

    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> disX(-WORLD_X, WORLD_X);
    std::uniform_real_distribution<float> disY(-WORLD_Y, WORLD_Y);
    std::uniform_real_distribution<float> disW(0.001f, 0.9f);
    std::uniform_real_distribution<float> disA(0.f, 360.f);

    for (auto i = 0u; i < num_miners; ++i) {
        auto& miner = miners[i];

        miner.region = {0u, 0u, texture.w/MINER_FRAMES, texture.h};
        miner.position = {disX(gen), disY(gen)};
        miner.angular_velocity = disW(gen);
        miner.velocity = {0.f, 0.f};
        miner.angle = disA(gen);
    }
}
