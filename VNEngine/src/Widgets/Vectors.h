#pragma once

#include <stdint.h>

namespace VNEngine {

	struct vec2 {
		int x, y;
	};

	union vec3 {
		struct { int x, y, z; };
		struct { int r, g, b; };
	};

	union vec4 {
		struct { int x, y, w, h; };
		struct { int r, g, b, a; };
	};

	struct vec4u8 {
		uint8_t r, g, b, a;
	};
}