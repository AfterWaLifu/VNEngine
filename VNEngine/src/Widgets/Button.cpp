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
		m_Focused = false;
	}

	Button::~Button() {
		Text::~Text();
	}

	void Button::Bind(std::function<void(void)> onClick) {
		if (onClick != nullptr) m_OnClick = onClick;
	}

	void Button::Check() {
		vec2 mousePos = IH_INSTANCE.getMousePos();

		if (mousePos.x >= m_Geometry.x &&
			mousePos.x < m_Geometry.x + m_Geometry.w &&
			mousePos.y > m_Geometry.y &&
			mousePos.y < m_Geometry.y + m_Geometry.h)
			m_Focused = true;
		else m_Focused = false;

		if (m_OnClick == nullptr) return;
		if (m_Focused && IH_INSTANCE.getMouseButtonState(InputHandler::LEFT))
			m_OnClick();
	}

	void Button::Draw() {
		Text::Draw();
	}
}

