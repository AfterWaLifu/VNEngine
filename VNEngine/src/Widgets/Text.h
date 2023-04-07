#pragma once

#include "Widgets/Widget.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

namespace VNEngine {

	struct FontInfo {
		const char* fontName;
		int fontSize;
	};

	class Text : public Widget {

		SDL_Texture* m_TextTexture;
		struct {
			FontInfo info;
			TTF_Font* font;
		} m_Font;
		
		vec4 m_TextNativeGeometry;
		vec4 m_TextDestination;

		vec4u8 m_TextColor;
		std::wstring m_Text;

		bool m_BackgroundTurned;

		void freeTexture();

	public:
		Text(vec4 geometry, std::wstring text = L"", vec4u8 textColor = {0,0,0,255},
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
		void TurnOnBack();
		void TurnOffBack();
		void SetBackgroundColor(vec4u8 color);
		vec4u8 GetBackgroundColor();
		void SetAlign(Alignment alignment);
		Alignment GetAlign();
		void SetBackImage();

		void Draw() override;
	};
}