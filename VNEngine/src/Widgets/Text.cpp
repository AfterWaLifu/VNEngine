#include "Text.h"
#include "vnepch.h"

#include <SDL2/SDL_ttf.h>

namespace VNEngine {

	Text::Text(vec2 coords, const wchar_t* text, SDL_Color textColor, FontInfo fontInfo)
		: Widget()
	{
		m_Geometry = {coords.x,coords.y,0,0};
		m_BackgroundColor = { 0,0,0,0 };
		m_Image = nullptr;

		m_TextTexture = nullptr;
		m_Font = TTF_OpenFont(fontInfo.fontName, fontInfo.fontSize);
		m_TextColor = textColor;

		if (text && text[0] != '\0') {
			SetText(text);
		}
	}

	Text::~Text() {
		freeTexture();
	}
	
	void Text::freeTexture() {
		if (m_TextTexture) {
			SDL_DestroyTexture(m_TextTexture);
			m_TextTexture = nullptr;
			m_Geometry = {};
		}
	}

	void Text::SetText(const wchar_t* text) {
		freeTexture();

		SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(
			m_Font, reinterpret_cast<Uint16 const*>(text), 
			m_TextColor);

		m_TextTexture = SDL_CreateTextureFromSurface(Widget::sRenderer, textSurface);

		m_Geometry.w = textSurface->w;
		m_Geometry.h = textSurface->h;

		SDL_FreeSurface(textSurface);
	}

	void Text::Draw() {
		SDL_RenderCopy(Widget::sRenderer, m_TextTexture, 
			nullptr, (SDL_Rect*)&m_Geometry);
	}

}