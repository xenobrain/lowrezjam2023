#pragma once

#include <engine/foundation.h>


namespace xc::platform {
    auto initialize(std::uint32_t width, std::uint32_t height) -> bool;
    auto uninitialize() -> void;

    auto tick() -> void;
} // namespace xc::platform
