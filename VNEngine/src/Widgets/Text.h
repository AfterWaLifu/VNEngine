#pragma once

#include "Widgets/Widget.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct FontInfo {
	const char* fontName;
	int fontSize;
};

namespace VNEngine {

	class Text : public Widget {

		SDL_Texture* m_TextTexture;
		TTF_Font* m_Font;

		SDL_Color m_TextColor;

		void freeTexture();

	public:
		Text(vec2 coords, const wchar_t* text = L"", SDL_Color textColor = {0,0,0,255},
			FontInfo fontInfo = { "Roboto.ttf", 16 });
		~Text();

		void SetText(const wchar_t* text);

		void Draw() override;
	};
}