#include "Button.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"

namespace VNEngine {

	Button::Button(vec4 geometry, std::function<void(void)> onClick, std::wstring text,
		vec4u8 BackgroundColor, vec4u8 textColor, FontInfo fontInfo)
		: Text(geometry, text, textColor, fontInfo)
	{
		Bind(onClick);
		m_BackgroundColor = BackgroundColor;
		m_DrawBorder = true;
		m_BorderColor = {
			(uint8_t)(m_BackgroundColor.r < 40 ? 255 - m_BackgroundColor.r : m_BackgroundColor.r - 40),
			(uint8_t)(m_BackgroundColor.g < 40 ? 255 - m_BackgroundColor.g : m_BackgroundColor.g - 40),
			(uint8_t)(m_BackgroundColor.b < 40 ? 255 - m_BackgroundColor.b : m_BackgroundColor.b - 40),
			255
		};
		m_DefaultBorder = m_BorderColor;
		m_FocusBorder = {
			(uint8_t)(m_BackgroundColor.r < 80 ? 255 - m_BackgroundColor.r : m_BackgroundColor.r - 80),
			(uint8_t)(m_BackgroundColor.g < 80 ? 255 - m_BackgroundColor.g : m_BackgroundColor.g - 80),
			(uint8_t)(m_BackgroundColor.b < 80 ? 255 - m_BackgroundColor.b : m_BackgroundColor.b - 80),
			255
		};
		m_Focused = false;
	}

	Button::~Button() {
	}

	void Button::Bind(std::function<void(void)> onClick) {
		if (onClick != nullptr) m_OnClick = onClick;
	}

	void Button::Check() {
		vec2 mousePos = IH_INSTANCE.getMousePos();

		if (mousePos.x >= m_Geometry.x &&
			mousePos.x < m_Geometry.x + m_Geometry.w &&
			mousePos.y > m_Geometry.y &&
			mousePos.y < m_Geometry.y + m_Geometry.h) {
			m_Focused = true;
			m_BorderColor = m_FocusBorder;
		}
		else { 
			m_Focused = false;
			m_BorderColor = m_DefaultBorder;
		}

		if (m_OnClick == nullptr) return;
		if (m_Focused && IH_INSTANCE.getMouseButtonState(InputHandler::LEFT))
			m_OnClick();
	}

	void Button::Draw() {
		Text::Draw();
	}

	bool Button::Pressed()
	{
		if (m_Focused && IH_INSTANCE.getMouseButtonState(InputHandler::LEFT)) {
			return true;
		}
		return false;
	}

	void Button::SetBorderColor(vec4u8 color) {
		m_DefaultBorder = color;
	}
	
	void Button::SetFocusBorderColor(vec4u8 color) {
		m_FocusBorder = color;
	}
	
	vec4u8 Button::GetFocusBorderColor() {
		return m_FocusBorder;
	}
}

