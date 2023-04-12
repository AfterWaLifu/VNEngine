#include "TextBox.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"

namespace VNEngine {

#ifdef _WIN32
#include <Windows.h>
	std::wstring cvt(const std::string& str) {
		if (str.empty())
			return std::wstring();

		size_t charsNeeded = MultiByteToWideChar(CP_UTF8, 0,
			str.data(), (int)str.size(), NULL, 0);

		std::vector<wchar_t> buffer(charsNeeded);
		int charsConverted = MultiByteToWideChar(CP_UTF8, 0,
			str.data(), (int)str.size(), &buffer[0], (int)buffer.size());

		return std::wstring(&buffer[0], charsConverted);
	}
#else
	std::wstring cvt(const std::string& str) {
		if (str.empty())
			return std::wstring();
		return std::wstring(str.begin(), str.end());
	}
#endif

	TextBox::TextBox(vec4 geometry, std::wstring text, vec4u8 textColor, FontInfo fontInfo)
		:Text(geometry, text, textColor, fontInfo)
	{
		m_BackgroundColor = { 220,220,220,255 };
		SetAlign((Alignment)(ALIGN_VCENTER | ALIGN_LEFT));
		s_ActiveTextBox = this;

		IH_INSTANCE.setTextInput(true);

		s_InputString ? 0 : s_InputString = IH_INSTANCE.getTextInput();
	}

	TextBox::~TextBox() {
		IH_INSTANCE.setTextInput(false);
		Text::~Text();
	}

	void TextBox::Check() {
		vec2 mousePos = IH_INSTANCE.getMousePos();

		if (IH_INSTANCE.getMouseButtonState(InputHandler::LEFT) &&
			mousePos.x >= m_Geometry.x &&
			mousePos.x < m_Geometry.x + m_Geometry.w &&
			mousePos.y > m_Geometry.y &&
			mousePos.y < m_Geometry.y + m_Geometry.h) {
			s_ActiveTextBox = this;
			*s_InputString = m_CurrentString;
			return;
		}
		
		if (this == s_ActiveTextBox &&
			m_CurrentString != *s_InputString) {
			m_CurrentString = *s_InputString;
			s_ActiveTextBox->SetText(cvt(m_CurrentString));
		}
	}

	void TextBox::Draw() {
		Text::Draw();

		if (s_ActiveTextBox != this) {
			SDL_SetRenderDrawColor(Widget::sRenderer, m_TextColor.r, m_TextColor.g, m_TextColor.b, m_TextColor.a);
			SDL_RenderDrawLine(Widget::sRenderer,
				m_TextDestination.x + m_TextDestination.w + 1,
				m_TextDestination.y + 1,
				m_TextDestination.x + m_TextDestination.w + 1,
				m_TextDestination.y + m_TextDestination.h + 1
			);
		}

		uint64_t time = SDL_GetTicks64();
		if ((time - m_LineTimer) > 500) {
			m_LineTimer = time;
			m_LineEnabled = !m_LineEnabled;
		}
		if (m_LineEnabled) {
			SDL_SetRenderDrawColor(Widget::sRenderer, m_TextColor.r, m_TextColor.g, m_TextColor.b, m_TextColor.a);
			SDL_RenderDrawLine(Widget::sRenderer,
				m_TextDestination.x + m_TextDestination.w + 1,
				m_TextDestination.y + 1,
				m_TextDestination.x + m_TextDestination.w + 1,
				m_TextDestination.y + m_TextDestination.h + 1
			);
		}
	}
}