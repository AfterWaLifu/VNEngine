#include "Button.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"
#include "Widgets/WidgetsManager.h"

namespace VNEngine {

	Button::Button(vec4 geometry, std::function<void(void)> onClick, std::wstring text,
		vec4u8 BackgroundColor, vec4u8 textColor, const std::string& fontKey)
		: Text(geometry, text, textColor, fontKey), m_OnClickLua(nullptr)
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

	Button::Button(const buttonState& bs) : Text(bs.ts), m_OnClickLua(nullptr)
	{
		m_DefaultBorder = bs.defaultborder;
		m_FocusBorder = bs.focusborder;
		m_Focused = false;
		if (bs.functionLua) m_OnClickLua = bs.functionLua;
		if (bs.function) m_OnClick = bs.function;
	}

	Button::~Button() {
		if (m_OnClickLua) delete m_OnClickLua;
	}

	void Button::Bind(std::function<void(void)> onClick) {
		if (onClick != nullptr) m_OnClick = onClick;
	}

	void Button::Bind(luabridge::LuaRef onClick) {
		if (onClick.isFunction()) m_OnClickLua = onClick;
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

		if (m_Focused && (m_OnClickLua || m_OnClick)) {
			if (IH_INSTANCE.getMouseButtonClicked(LEFT)) {
				if (m_OnClick) m_OnClick();
				if (m_OnClickLua) (*m_OnClickLua)();
			}
		}
	}

	void Button::Draw() {
		Text::Draw();
	}

	bool Button::Pressed()
	{
		return m_Focused && IH_INSTANCE.getMouseButtonClicked(LEFT);
	}

	bool Button::Focused() {
		return m_Focused;
	}

	void Button::SetBorderColor(vec4u8 color) {
		m_DefaultBorder = color;
	}
	
	void Button::SetFocusBorderColor(vec4u8 color) {
		m_FocusBorder = color;
	}

	vec4u8 Button::GetDefaultBorderColor() {
		return m_DefaultBorder;
	}
	
	vec4u8 Button::GetFocusBorderColor() {
		return m_FocusBorder;
	}
	
	buttonState Button::Dump() {
		textState t = Text::Dump();
		buttonState b = {
			t,
			m_DefaultBorder, m_FocusBorder, m_OnClickLua
		};
		return b;
	}
}

