#include "Text.h"
#include "vnepch.h"

#include <SDL2/SDL_ttf.h>

#include <stdlib.h>

#include "Core/Logger.h"

namespace VNEngine {

	Text::Text(vec2 coords, std::wstring text, vec4u8 textColor, FontInfo fontInfo)
		: Widget()
	{
		m_Geometry = {coords.x,coords.y,0,0};
		m_BackgroundColor = { 0,0,0,0 };

		m_TextTexture = nullptr;
		SetFont(fontInfo);
		m_TextColor = textColor;

		if (!text.empty()) {
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

	void Text::SetText(std::wstring text) {
		freeTexture();

		m_Text.clear();
		m_Text = text;

		SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(
			m_Font.font, reinterpret_cast<Uint16 const*>(m_Text.c_str()),
			{m_TextColor.r,m_TextColor.g ,m_TextColor.b ,m_TextColor.a });

		if (!textSurface) {
			char* temp = (char*) malloc(m_Text.length());
			wcstombs_s(nullptr, temp, m_Text.length(), m_Text.c_str(), m_Text.length() * sizeof(wchar_t));
			VN_LOGS_WARNING("Can't create text label '" << temp << "'");
			return;
		}

		m_TextTexture = SDL_CreateTextureFromSurface(Widget::sRenderer, textSurface);
		if (!m_TextTexture) {
			char* temp = (char*)malloc(m_Text.length());
			wcstombs_s(nullptr, temp, m_Text.length(), m_Text.c_str(), m_Text.length() * sizeof(wchar_t));
			VN_LOGS_WARNING("Can't create text texture '" << temp << "'");
			return;
		}

		m_Geometry.w = textSurface->w;
		m_Geometry.h = textSurface->h;

		SDL_FreeSurface(textSurface);
	}

	std::wstring Text::GetText() {
		return m_Text;
	}

	void Text::SetFont(FontInfo fontInfo) {
		m_Font.info = fontInfo;
		m_Font.font = TTF_OpenFont(m_Font.info.fontName, m_Font.info.fontSize);
		if (!m_Font.font) VN_LOGS_ERROR("Can't load font '" <<
			fontInfo.fontName << "', check other errors or fonts files location");
		if (!m_Text.empty()) SetText(m_Text);
	}

	FontInfo Text::GetFont() {
		return m_Font.info;
	}

	void Text::Show() {
		m_IsShown = true;
	}

	void Text::Hide() {
		m_IsShown = false;
	}

	bool Text::IsItShown()
	{
		return m_IsShown;
	}

	void Text::SetTextColor(vec4u8 color) {
		m_TextColor = color;
	}

	vec4u8 Text::GetTextColor() {
		return m_TextColor;
	}

	void Text::Draw() {
		if (m_IsShown) {
			SDL_RenderCopy(Widget::sRenderer, m_TextTexture,
						nullptr, (SDL_Rect*)&m_Geometry);
		}
	}

}