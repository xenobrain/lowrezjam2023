#pragma once

#include <engine/foundation.h>


namespace xc::renderer {
    struct font_t { void* info; std::uint8_t* buffer; int size; };
    struct texture_t { std::uint32_t id, w, h; };

    auto initialize(std::uint32_t width, std::uint32_t height) -> bool;
    auto uninitialize() -> void;

    auto begin() -> void;
    auto end() -> void;

    auto tick() -> void;

    auto clear(color_t color) -> void;
    auto swap() -> void;

    auto set_camera(vector2_t camera) -> void;

    // Shaders
    auto create_shader(char const* vertex_shader_path, char const* fragment_shader_path) -> std::uint32_t;
    auto destroy_shader(std::uint32_t shader) -> void;
    auto bind_shader(std::uint32_t shader) -> void;

    auto set_shader_uniform(std::uint32_t shader, char const* name, matrix4_t value) -> void;


    // Textures
    auto create_texture(char const* path) -> texture_t;
    auto destroy_texture(texture_t texture) -> void;
    auto bind_texture(texture_t texture) -> void;

    auto draw_texture(texture_t texture, rectangle_t region, vector2_t position, float angle, float scale) -> void;


    // Text
    auto create_font(char const* path) -> font_t;
    auto draw_text(char const* text, font_t font, vector2_t position) -> void;
} // namespace xc::renderer