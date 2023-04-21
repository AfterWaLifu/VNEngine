#include "InputHandler.h"
#include "vnepch.h"

namespace VNEngine {

	InputHandler::InputHandler(bool* isRunning)
		: m_isRunningPointer(isRunning), m_keystates(nullptr),
		m_mousePos({0,0}), m_windowResized(false), m_keyHolding(false),
		m_textInputEnabled(false), m_textInputText(""),m_mouseScrollAmount(0)
	{
		for (int i = 0; i < 3; i++) m_mouseButtonStates.push_back(false);
	}
	
	InputHandler::~InputHandler() {
	}

	InputHandler& InputHandler::Instance()
	{
		if (s_pInstance) return *s_pInstance;
		else {
			VN_LOGS_ERROR("Attemp to use Texture Manager before initialization, please init it correctly");
			InputHandlerInit(nullptr);
		}
		return *s_pInstance;
	}

	void InputHandler::InputHandlerInit(bool* isRunning) {
		if (isRunning == nullptr) return;
		s_pInstance ? 0 : s_pInstance = new InputHandler(isRunning);
	}
	
	void InputHandler::Update() {
		SDL_Event event;
		SDL_PollEvent(&event);
		m_mouseScrollAmount ? m_mouseScrollAmount = 0 : 0;

		switch (event.type) {
		case SDL_QUIT:
			*m_isRunningPointer = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonDown(event);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonUp(event);
			break;
		case SDL_MOUSEMOTION:
			onMouseMove(event);
			break;
		case SDL_MOUSEWHEEL:
			onMouseWheelEvent(event);
			break;
		case SDL_KEYDOWN:
			onKeyDown(event);
			break;
		case SDL_KEYUP:
			onKeyUp();
			break;
		case SDL_WINDOWEVENT:
			onWindowEvent(event);
			break;
		case SDL_TEXTINPUT:
			onTextInput(event);
			break;
		default:
			break;
		}
	}

	bool InputHandler::getMouseButtonState(MouseButtons button)
	{
		return m_mouseButtonStates[button];
	}

	const vec2& InputHandler::getMousePos()
	{
		return m_mousePos;
	}

	int32_t InputHandler::getMouseScrollAmount() {
		return m_mouseScrollAmount;
	}

	bool InputHandler::isKeyDown(std::string key)
	{
		using namespace Keys;
		auto scancode = getScancodeFromKey(key.c_str());
		if (scancode == SDL_SCANCODE_UNKNOWN) {
			VN_LOGS_WARNING("Attemp to check non-existing button " << key);
			return false;
		}
		if (m_keystates == nullptr) return false;
		if (m_keyHolding && (
			scancode != SDL_SCANCODE_LCTRL &&
			scancode != SDL_SCANCODE_RCTRL &&
			scancode != SDL_SCANCODE_LALT  &&
			scancode != SDL_SCANCODE_RALT  &&
			scancode != SDL_SCANCODE_LSHIFT&&
			scancode != SDL_SCANCODE_RSHIFT
			)) return false;

		return !!(m_keystates[K[key]]);
	}

	bool InputHandler::getIfWindowResized() {
		return m_windowResized;
	}

	void InputHandler::setIfWindowResized(bool resized) {
		m_windowResized = resized;
	}

	std::string* InputHandler::getTextInput()
	{
		return &m_textInputText;
	}

	void InputHandler::setTextInput(bool state) {
		m_textInputEnabled = state;
		m_textInputEnabled ? SDL_StartTextInput() : SDL_StopTextInput();
	}

	void InputHandler::onKeyDown(SDL_Event& event) {
		m_keystates = (uint8_t*)SDL_GetKeyboardState(0);
		m_keyHolding = true;

		if (m_textInputEnabled) {
			if (event.key.keysym.sym == SDLK_BACKSPACE && m_textInputText.length() > 0)
			{
				m_textInputText.pop_back();
			}
			else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			{
				m_textInputText += SDL_GetClipboardText();
			}
		}
	}

	void InputHandler::onKeyUp() {
		m_keyHolding = false;
	}

	void InputHandler::onMouseMove(SDL_Event& event) {
		m_mousePos.x = event.motion.x;
		m_mousePos.y = event.motion.y;
	}

	void InputHandler::onMouseButtonUp(SDL_Event& event) {
		if (event.button.button == SDL_BUTTON_LEFT) m_mouseButtonStates[LEFT] = false;
		if (event.button.button == SDL_BUTTON_MIDDLE) m_mouseButtonStates[MIDDLE] = false;
		if (event.button.button == SDL_BUTTON_RIGHT) m_mouseButtonStates[RIGHT] = false;
	}

	void InputHandler::onMouseButtonDown(SDL_Event& event) {
		if (event.button.button == SDL_BUTTON_LEFT) m_mouseButtonStates[LEFT] = true;
		if (event.button.button == SDL_BUTTON_MIDDLE) m_mouseButtonStates[MIDDLE] = true;
		if (event.button.button == SDL_BUTTON_RIGHT) m_mouseButtonStates[RIGHT] = true;
	}

	void InputHandler::onMouseWheelEvent(SDL_Event& event) {
		m_mouseScrollAmount = event.wheel.y;
	}

	void InputHandler::onWindowEvent(SDL_Event& event) {
		if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
			m_windowResized = true;
		}
	}

	void InputHandler::onTextInput(SDL_Event& event) {
		if (!(SDL_GetModState() & KMOD_CTRL && (event.text.text[0] == 'v' ||
			event.text.text[0] == 'V')))
		{
			m_textInputText += event.text.text;
		}
	}

}