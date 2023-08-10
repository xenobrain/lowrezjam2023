#pragma once

#include <cstddef>
#include <cstdint>

#include <glm/glm.hpp>


namespace xc {
    struct color_t { float r, g, b, a; };
    struct rectangle_t { std::uint32_t x, y, w, h; };
} // namespace xc