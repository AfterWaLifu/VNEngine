#pragma once

#include "Widgets/Widget.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

struct FontInfo {
	const char* fontName;
	int fontSize;
};

namespace VNEngine {

	class Text : public Widget {

		SDL_Texture* m_TextTexture;
		struct {
			FontInfo info;
			TTF_Font* font;
		} m_Font;

		vec4u8 m_TextColor;
		std::wstring m_Text;

		void freeTexture();

	public:
		Text(vec2 coords, std::wstring text = L"", vec4u8 textColor = {0,0,0,255},
			FontInfo fontInfo = { "Roboto.ttf", 16 });
		~Text();

		void SetText(std::wstring text);
		std::wstring GetText();
		void SetFont(FontInfo fontInfo);
		FontInfo GetFont();
		void Show();
		void Hide();
		bool IsItShown();
		void SetTextColor(vec4u8 color);
		vec4u8 GetTextColor();

		void Draw() override;
	};
}