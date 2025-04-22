#pragma once

#include <cstdint>

namespace VNEngine {

struct vec2 {
    std::int32_t x, y;
};

struct vec4 {
    std::int32_t x, y, w, h;
};

struct vec4u8 {
    uint8_t r, g, b, a;
};

} // namespace VNEngine