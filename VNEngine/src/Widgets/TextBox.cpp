#include "TextBox.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"

namespace VNEngine {

	TextBox::TextBox(vec4 geometry, std::wstring text, uint32_t maxCharNumber,
		vec4u8 textColor, const std::string& fontKey)
		:Text(geometry, text, textColor, fontKey)
	{
		m_BackgroundColor = { 220,220,220,255 };
		SetAlign((Alignment)(ALIGN_VCENTER | ALIGN_LEFT));
		s_ActiveTextBox = this;

		m_DrawBorder = true;
		m_BorderColor = {
			(uint8_t)(m_BackgroundColor.r < 40 ? 255 - m_BackgroundColor.r : m_BackgroundColor.r - 40),
			(uint8_t)(m_BackgroundColor.g < 40 ? 255 - m_BackgroundColor.g : m_BackgroundColor.g - 40),
			(uint8_t)(m_BackgroundColor.b < 40 ? 255 - m_BackgroundColor.b : m_BackgroundColor.b - 40),
			255
		};

		m_LineEnabled = true;
		m_LineTimer = SDL_GetTicks64();
		m_MaxCharNumber = maxCharNumber;

		IH_INSTANCE.setTextInputState(true);

		s_InputString ? 0 : s_InputString = IH_INSTANCE.getTextInput();
	}

	TextBox::TextBox(const textboxState& tbs) : Text(tbs.ts)
	{
		SetValue(tbs.currentString);
		SetMaxCharNumber(tbs.maxchar);

		m_LineEnabled = true;
		m_LineTimer = SDL_GetTicks64();

		s_ActiveTextBox = this;

		IH_INSTANCE.setTextInputState(true);
		s_InputString ? 0 : s_InputString = IH_INSTANCE.getTextInput();
	}

	TextBox::~TextBox() {
		IH_INSTANCE.setTextInputState(false);
	}

	void TextBox::Check() {
		if (IH_INSTANCE.getMouseButtonState(LEFT)) {
			vec2 mousepos = IH_INSTANCE.getMousePos();

			if (mousepos.x >= m_Geometry.x &&
				mousepos.y >= m_Geometry.y &&
				mousepos.x <= m_Geometry.x + m_Geometry.w &&
				mousepos.y <= m_Geometry.y + m_Geometry.h) {
				
				s_ActiveTextBox = this;

				if (IH_INSTANCE.getTextInputState() == false) IH_INSTANCE.setTextInputState(true);

				if (s_InputString) {
					m_CurrentString.empty() ? *s_InputString = "" : *s_InputString = m_CurrentString;
				}
			}
		}

		if (this == s_ActiveTextBox && s_InputString) {
			s_InputString->empty() ? m_CurrentString = "" : m_CurrentString = *s_InputString;
			s_ActiveTextBox->SetText(m_CurrentString);
		}

		if (m_CurrentString.length() > m_MaxCharNumber) {
			s_InputString->pop_back();
			m_CurrentString.pop_back();
		}

		/*
		vec2 mousePos = IH_INSTANCE.getMousePos();

		if (IH_INSTANCE.getMouseButtonState(LEFT) &&
			mousePos.x >= m_Geometry.x &&
			mousePos.x < m_Geometry.x + m_Geometry.w &&
			mousePos.y > m_Geometry.y &&
			mousePos.y < m_Geometry.y + m_Geometry.h) {
			s_ActiveTextBox = this;
			if (IH_INSTANCE.getTextInputState() == false) IH_INSTANCE.setTextInputState(true);
			if (!s_InputString) return;
			if (m_CurrentString.empty()) *s_InputString = "";
			else *s_InputString = m_CurrentString;
			return;
		}
		
		if (this == s_ActiveTextBox && !m_CurrentString.empty() &&
			m_CurrentString != *s_InputString &&
			m_Text.length() < m_MaxCharNumber) {
			m_CurrentString = *s_InputString;
			s_ActiveTextBox->SetText(cvt(m_CurrentString));
		}

		if (m_Text.length() >= m_MaxCharNumber) {
			s_InputString->pop_back();
		}
		*/
	}

	void TextBox::Draw() {
		Text::Draw();

		if (s_ActiveTextBox != this) {
			SDL_SetRenderDrawColor(Widget::sWD.renderer, m_TextColor.r, m_TextColor.g, m_TextColor.b, m_TextColor.a);
			SDL_RenderDrawLine(Widget::sWD.renderer,
				m_TextDestination.x + m_TextDestination.w + 1,
				m_TextDestination.y + 1,
				m_TextDestination.x + m_TextDestination.w + 1,
				m_TextDestination.y + m_TextDestination.h + 1
			);
		}

		uint64_t time = SDL_GetTicks64();
		if ((time - m_LineTimer) > 300) {
			m_LineTimer = time;
			m_LineEnabled = !m_LineEnabled;
		}
		if (m_LineEnabled) {
			SDL_SetRenderDrawColor(Widget::sWD.renderer, m_TextColor.r, m_TextColor.g, m_TextColor.b, m_TextColor.a);
			SDL_RenderDrawLine(Widget::sWD.renderer,
				m_TextDestination.x + m_TextDestination.w + 1,
				m_TextDestination.y + 1,
				m_TextDestination.x + m_TextDestination.w + 1,
				m_TextDestination.y + m_TextDestination.h + 1
			);
		}
	}

	std::string TextBox::Value() {
		return m_CurrentString;
	}

	void TextBox::SetValue(const std::string& str) {
		m_CurrentString = str;
		if (s_ActiveTextBox == this) *s_InputString = str;
		SetText(str);
	}

	void TextBox::SetMaxCharNumber(uint32_t quantity) {
		m_MaxCharNumber = quantity;
		if (m_CurrentString.length() > quantity) {
			m_CurrentString = m_CurrentString.substr(0, quantity);
			SetText(cvt(m_CurrentString));
		}
	}
	
	uint32_t TextBox::GetMaxCharNumber() {
		return m_MaxCharNumber;
	}
	
	textboxState TextBox::Dump() {
		textboxState tbs = {
			Text::Dump(),
			m_CurrentString, m_MaxCharNumber
		};
		return tbs;
	}
}