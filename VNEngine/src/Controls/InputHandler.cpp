#include "InputHandler.h"
#include "vnepch.h"

namespace VNEngine {

	InputHandler::InputHandler(bool* isRunning)
		: m_isRunningPointer(isRunning), m_keystates(nullptr),
		mousePos({0,0}), windowResized(false)
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
			InputHandler(nullptr);
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
		case SDL_KEYDOWN:
			onKeyDown();
			break;
		case SDL_WINDOWEVENT:
			onWindowEvent(event);
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
		return mousePos;
	}

	bool InputHandler::isKeyDown(SDL_Scancode key)
	{
		if (m_keystates == nullptr) return false;

		return !!(m_keystates[key]);
	}

	bool InputHandler::getIfWindowResized() {
		if (windowResized) {
			windowResized = false;
			return true;
		}
		return false;
	}

	void InputHandler::onKeyDown() {
		m_keystates = (uint8_t*)SDL_GetKeyboardState(0);
	}

	void InputHandler::onMouseMove(SDL_Event& event) {
		mousePos.x = event.motion.x;
		mousePos.y = event.motion.y;
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

	void InputHandler::onWindowEvent(SDL_Event& event) {
		if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
			windowResized = true;
		}
	}

}