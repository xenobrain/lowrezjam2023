#pragma once

#include <engine/foundation.h>

namespace xc::renderer {
    struct texture_t { std::uint32_t id, w, h; };

    auto initialize(std::uint32_t width, std::uint32_t height) -> bool;
    auto uninitialize() -> void;
    auto tick() -> void;

    auto clear(color_t color) -> void;
    auto swap() -> void;


    // Shaders
    auto create_shader(char const* vertex_shader_path, char const* fragment_shader_path) -> std::uint32_t;
    auto destroy_shader(std::uint32_t shader) -> void;
    auto bind_shader(std::uint32_t shader) -> void;

    auto set_shader_uniform(std::uint32_t shader, char const* name, glm::mat4 value) -> void;


    // Textures
    auto create_texture(char const* path) -> texture_t;
    auto destroy_texture(texture_t texture) -> void;
    auto bind_texture(texture_t texture) -> void;

    auto draw_texture(texture_t texture, rectangle_t region, glm::vec2 position, float angle, float scale) -> void;

} // namespace xc::renderer