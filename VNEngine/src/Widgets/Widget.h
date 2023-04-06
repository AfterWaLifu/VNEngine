#pragma once

#include <stdint.h>

#include "Widgets/Vectors.h"

#include <SDL2/SDL.h>

namespace VNEngine {

	enum Alignment : uint8_t {
		ALIGN_LEFT = 0,
		ALIGN_RIGHT = 1,
		ALIGN_CENTER = 2
	};

	class Widget {
		
	protected:
		
		inline static SDL_Renderer* sRenderer;

		vec4 m_Geometry;
		SDL_Texture* m_Image;
		vec4 m_BackgroundColor;
		Alignment m_Alignment;
		bool m_IsShown;

		Widget() 
			: m_Geometry({}), m_Image(nullptr), m_BackgroundColor({}),
			  m_Alignment((Alignment)0), m_IsShown(true) {}
		virtual ~Widget() = default;

	public:
		static void TurnOnWidgets(SDL_Renderer* renderer) {
			sRenderer = renderer;
		}

		virtual void Draw() = 0;
	};
}