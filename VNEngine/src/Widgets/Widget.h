#pragma once

#include "Widgets/Vectors.h"

namespace VNEngine {

	class Widget {
		
	protected:
		Widget(vec4 geometry) {}
		virtual ~Widget() = 0;

		vec4 m_Geometry;
	};
}