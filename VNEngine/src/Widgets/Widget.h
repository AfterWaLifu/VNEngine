#pragma once

#include "Widgets/Vectors.h"

#include <SDL2/SDL.h>

namespace VNEngine {

	class Widget {
		
	protected:
		
		inline static SDL_Renderer* sRenderer;

		vec4 m_Geometry;
		SDL_Texture* m_Image;
		vec4 m_BackgroundColor;

		Widget() 
			: m_Geometry({}), m_Image(nullptr), m_BackgroundColor({}) {}
		virtual ~Widget() = default;

	public:
		static void TurnOnWidgets(SDL_Renderer* renderer) {
			sRenderer = renderer;
		}

		virtual void Draw() = 0;
	};
}