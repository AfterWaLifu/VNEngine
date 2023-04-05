#pragma once

namespace VNEngine {

	union vec3 {
		struct { int x, y, z; };
		struct { int r, g, b; };
	};

	union vec4 {
		struct { int x, y, z, w; };
		struct { int r, g, b, a; };
	};

	struct vec4u8 {
		uint8_t r, g, b, a;
	};
}