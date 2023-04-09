#include "Text.h"
#include "vnepch.h"

#include <SDL2/SDL_ttf.h>

#include <stdlib.h>

#include "Core/Logger.h"

namespace VNEngine {

	Text::Text(vec4 geometry, std::wstring text, vec4u8 textColor, FontInfo fontInfo)
		: Widget()
	{
		m_Geometry = geometry;
		m_BackgroundColor = { 0,0,0,0 };
		m_Image = nullptr;

		m_TextTexture = nullptr;
		m_TextNativeGeometry = {0,0,0,0};
		m_TextDestination = { 0,0,0,0 };
		m_IndentHorizontal = 3;
		m_IndentVertical = 3;
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
		}
	}

	void Text::SetText(std::wstring text) {
		freeTexture();

		if (text.empty()) return;
		if (text != m_Text) {
			m_Text = L"";
			m_Text = text;
		}

		SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(
			m_Font.font, reinterpret_cast<Uint16 const*>(m_Text.c_str()),
			{m_TextColor.r,m_TextColor.g ,m_TextColor.b ,m_TextColor.a });

		if (!textSurface) {
			size_t length = m_Text.length();
			char* temp = (char*) malloc(length * 2);
			wcstombs_s(&length, temp, length*2, m_Text.c_str(), m_Text.size());
			VN_LOGS_WARNING("Can't create text label '" << temp << "'");
			return;
		}

		m_TextTexture = SDL_CreateTextureFromSurface(Widget::sRenderer, textSurface);
		if (!m_TextTexture) {
			size_t length = m_Text.length();
			char* temp = (char*)malloc(m_Text.length());
			wcstombs_s(&length, temp, length * 2, m_Text.c_str(), m_Text.size());
			VN_LOGS_WARNING("Can't create text texture '" << temp << "'");
			return;
		}

		m_TextNativeGeometry = {0,0, textSurface->w, textSurface->h};

		SetAlign(m_Alignment);

		SDL_FreeSurface(textSurface);
	}

	std::wstring Text::GetText() {
		return m_Text;
	}

	void Text::SetFont(FontInfo fontInfo) {
		m_Font.info = fontInfo;
		m_Font.font = TTF_OpenFont(m_Font.info.fontName, m_Font.info.fontSize);
		if (!m_Font.font) {
			VN_LOGS_ERROR("Can't load font '" <<
				fontInfo.fontName << "', check other errors or fonts files location");
			return;
		}
		if (!m_Text.empty()) SetText(m_Text);
	}

	FontInfo Text::GetFont() {
		return m_Font.info;
	}

	void Text::SetFontSize(int size) {
		m_Font.info.fontSize = size;
		SetFont(m_Font.info);
	}

	int Text::GetFontSize() {
		return m_Font.info.fontSize;
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
		SetText(m_Text);
	}

	vec4u8 Text::GetTextColor() {
		return m_TextColor;
	}

	void Text::TurnOnBack() {
		m_BackgroundTurned = true;
	}

	void Text::TurnOffBack() {
		m_BackgroundTurned = false;
	}

	void Text::SetBackgroundColor(vec4u8 color) {
		m_BackgroundColor = color;
	}

	vec4u8 Text::GetBackgroundColor() {
		return m_BackgroundColor;
	}

	void Text::SetAlign(Alignment alignment) {

		m_Alignment = alignment;

		m_TextDestination.w = m_Geometry.w >= m_TextNativeGeometry.w ? m_TextNativeGeometry.w : m_Geometry.w;
		m_TextDestination.h = m_Geometry.h >= m_TextNativeGeometry.h ? m_TextNativeGeometry.h : m_Geometry.h;

		if (m_Alignment & ALIGN_LEFT )
			m_TextDestination.x = m_Geometry.x + m_IndentHorizontal;
		else if (m_Alignment & ALIGN_RIGHT)
			m_TextDestination.x = m_Geometry.x + m_Geometry.w - m_TextNativeGeometry.w -
				m_IndentHorizontal;
		else
			m_TextDestination.x = m_Geometry.x + (m_Geometry.w - m_TextNativeGeometry.w) / 2;
		if (m_Alignment & ALIGN_UP)
			m_TextDestination.y = m_Geometry.y -
			(m_TextNativeGeometry.h - m_Font.info.fontSize) + m_IndentVertical;
		else if (m_Alignment & ALIGN_DOWN)	
			m_TextDestination.y = m_Geometry.y + m_Geometry.h - m_TextNativeGeometry.h +
			(m_TextNativeGeometry.h - m_Font.info.fontSize) - m_IndentVertical;
		else 
			m_TextDestination.y = m_Geometry.y + (m_Geometry.h - m_TextNativeGeometry.h) / 2;
	}

	Alignment Text::GetAlign() {
		return m_Alignment;
	}

	void Text::SetVerticalIndent(int indent) {
		m_IndentVertical = indent;
		SetAlign(m_Alignment);
	}

	void Text::SetHorizontalIndent(int indent) {
		m_IndentHorizontal = indent;
		SetAlign(m_Alignment);
	}

	int Text::GetVerticalIndent() {
		return m_IndentVertical;
	}

	int Text::GetHorizontalIndent() {
		return m_IndentHorizontal;
	}

	void Text::SetBackImage(std::string key) {
		m_Image = TM_INSTANCE.getTexture(key);
	}

	void Text::SetGeometry(vec4 geometry)
	{
		m_Geometry = geometry;
		SetAlign(m_Alignment);
	}

	vec4 Text::GetGeometry()
	{
		return m_Geometry;
	}

	void Text::Draw() {
		if (!m_IsShown) return;

		if (m_BackgroundTurned) {
			if (m_Image) {
				SDL_RenderCopy(Widget::sRenderer, m_Image->sdl_texture,
					nullptr, (SDL_Rect*)&m_Geometry);
			}
			else if (m_BackgroundColor.a != 0) {
				SDL_SetRenderDrawColor(Widget::sRenderer, m_BackgroundColor.r,
					m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
				SDL_RenderFillRect(Widget::sRenderer, (SDL_Rect*)&m_Geometry);
			}
		}

		SDL_RenderCopy(Widget::sRenderer, m_TextTexture,
			(SDL_Rect*)&m_TextNativeGeometry, (SDL_Rect*)&m_TextDestination);
	}

}