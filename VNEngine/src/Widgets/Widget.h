#pragma once

#include <stdint.h>

#include "Widgets/Vectors.h"
#include "Graphic/TextureManager.h"

#include <SDL2/SDL.h>

namespace VNEngine {

	enum Alignment : uint8_t {
		ALIGN_LEFT		= 1 << 0,
		ALIGN_RIGHT		= 1 << 1,
		ALIGN_HCENTER	= 1 << 2,
		ALIGN_VCENTER	= 1 << 3,
		ALIGN_UP		= 1 << 4,
		ALIGN_DOWN		= 1 << 5
	};

	struct WindowData {
		SDL_Renderer* renderer;
		vec2* windowSizeCurr;
		vec2* windowSizePrev;
		vec2* windowSizeBase;
		vec4* backgrSizeCurr;
		vec4* backgrSizePrev;
		uint8_t* stretching;	// 1 full, 2 stretch
	};

	class Widget {
		
	protected:
		
		inline static WindowData sWD;

		vec4 m_Geometry;
		Texture* m_Image;
		vec4u8 m_BackgroundColor;
		Alignment m_Alignment;
		bool m_IsShown;
		bool m_BackgroundTurned;

		Widget() 
			: m_Geometry({}), m_Image(nullptr), m_BackgroundColor({}),
			  m_Alignment((Alignment)(ALIGN_HCENTER | ALIGN_VCENTER)), m_IsShown(true), m_BackgroundTurned(true) {}
		virtual ~Widget() = default;

	public:
		static void TurnOnWidgets(WindowData wd) {
			sWD.renderer = wd.renderer; 
			sWD.windowSizeCurr = wd.windowSizeCurr;
			sWD.windowSizePrev = wd.windowSizePrev;
			sWD.windowSizeBase = wd.windowSizeBase;
			sWD.backgrSizeCurr = wd.backgrSizeCurr;
			sWD.backgrSizePrev = wd.backgrSizePrev;
			sWD.stretching= wd.stretching;
		}

		virtual void Draw() = 0;

		virtual void SetGeometry(vec4 geometry) = 0;
	};
}