#pragma once

auto static constexpr SCREEN_X = 64.f;
auto static constexpr SCREEN_Y = 64.f;
auto static constexpr SCREEN_SCALE = 10.f;
auto static constexpr SCREEN_CX = SCREEN_X / 2.f;
auto static constexpr SCREEN_CY = SCREEN_Y / 2.f;
auto static constexpr WINDOW_X = SCREEN_X * SCREEN_SCALE;
auto static constexpr WINDOW_Y = SCREEN_Y * SCREEN_SCALE;


auto static constexpr WORLD_X = SCREEN_X * 1.f;
auto static constexpr WORLD_Y = SCREEN_Y * 1.f;

auto static constexpr RADAR_WIDTH = 25.f;
auto static constexpr RADAR_HEIGHT = 15.f;

auto static constexpr PLAYER_RADIUS = 4.f;
auto static constexpr PLAYER_MAX_VELOCITY = 1.f;
auto static constexpr PLAYER_FIRE_TIMEOUT = 30u;


auto static constexpr MAX_PROJECTILES = 32u;
auto static constexpr PROJECTILE_RADIUS = 1.f;
auto static constexpr PROJECTILE_VELOCITY = 100.f;


auto static constexpr MAX_NASTIES = 32u;
auto static constexpr NASTY_SPAWN_DELAY = 3.f;
auto static constexpr NASTY_PROJECTILE_DELAY = 5.f;

auto static constexpr BIL_BEHAVIOR_SWITCH_TIME = 360;
auto static constexpr BIL_ACCELERATION = 20.f;
auto static constexpr BIL_RADIUS = 8.f;


auto static constexpr MAX_MINERS = 32u;
auto static constexpr MINER_FRAMES = 3u;
auto static constexpr MINER_RADIUS = 4.f;
auto static constexpr MINER_FRAME_TIME = 1.f;