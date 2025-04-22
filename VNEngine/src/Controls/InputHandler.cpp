#include <VNEngine/Controls/InputHandler.hpp>

namespace VNEngine {

InputHandler::InputHandler()
    : isRunning_(true), mousePos_({0, 0}), mouseScrollAmount_(0), keyStates_(nullptr), windowResized_(false),
      textInputEnabled_(false), textInputText_("") {
    mouseButtonStates_ = {false, false, false};
}

InputHandler& InputHandler::instance() {

    if (!instance_) {
        instance_ = std::shared_ptr<InputHandler>(new InputHandler());
    }
    return *instance_;
}

const bool& InputHandler::isRunning() { return isRunning_; }

void InputHandler::update() {
    SDL_Event event;
    SDL_WaitEvent(&event);

    mouseScrollAmount_ = 0;

    switch (event.type) {
    case SDL_QUIT:
        isRunning_ = false;
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
        onKeyUp(event);
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

bool InputHandler::mouseButtonClicked(MouseButtons button) {
    if (mouseButtonStates_[button]) {
        mouseButtonStates_[button] = false;
        return true;
    } else
        return false;
}

bool InputHandler::mouseButtonHeld(MouseButtons button) { return mouseButtonStates_[button]; }

const vec2& InputHandler::mousePos() { return mousePos_; }

int32_t InputHandler::mouseScrollAmount() { return mouseScrollAmount_; }

bool InputHandler::isKeyPressed(const std::string& key) {
    if (keysPressed_.empty()) {
        return false;
    }

    auto scancode = Keys::getScancodeFromKey(key.c_str());
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        VN_LOGS_WARNING("Attemp to check non-existing button " << key);
        return false;
    }

    auto it = std::find(keysPressed_.begin(), keysPressed_.end(), scancode);
    if (it != keysPressed_.end()) {
        keysPressed_.erase(it);
        return true;
    }
    return false;
}

bool InputHandler::isKeyHeld(const std::string& key) {
    if (keysPressed_.empty()) {
        return false;
    }

    auto scancode = Keys::getScancodeFromKey(key.c_str());
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        VN_LOGS_WARNING("Attemp to check non-existing button " << key);
        return false;
    }

    auto it = std::find(keysPressed_.begin(), keysPressed_.end(), scancode);
    return it != keysPressed_.end();
}

bool InputHandler::windowResized() { return windowResized_; }

void InputHandler::setWindowResized(bool resized) { windowResized_ = resized; }

std::string& InputHandler::textInput() { return textInputText_; }

void InputHandler::setTextInputState(bool state) {
    textInputEnabled_ = state;
    textInputEnabled_ ? SDL_StartTextInput() : SDL_StopTextInput();
}

bool InputHandler::textInputState() { return textInputEnabled_; }

void InputHandler::exit() { isRunning_ = false; }

void InputHandler::onKeyDown(SDL_Event& event) {
    keyStates_ = (std::uint8_t*)SDL_GetKeyboardState(0);

    //	if just for pressed (widthout holding)
    auto it = std::find(keysPressed_.begin(), keysPressed_.end(), event.key.keysym.scancode);
    if (it != keysPressed_.end()) {
        keysPressed_.push_back(event.key.keysym.scancode);
    }

    // text input handling
    if (textInputEnabled_) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && textInputText_.length() > 0) {
            textInputText_.pop_back();
        } else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
            textInputText_ += SDL_GetClipboardText();
        }
    }
}

void InputHandler::onKeyUp(SDL_Event& event) {
    keyStates_ = (uint8_t*)SDL_GetKeyboardState(0);

    auto it = std::find(keysPressed_.begin(), keysPressed_.end(), event.key.keysym.scancode);
    if (it != keysPressed_.end()) {
        keysPressed_.erase(it);
    }
}

void InputHandler::onMouseMove(SDL_Event& event) {
    mousePos_.x = event.motion.x;
    mousePos_.y = event.motion.y;
}

void InputHandler::onMouseButtonUp(SDL_Event& event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        mouseButtonStates_[MouseButtons::LEFT] = false;
    }
    if (event.button.button == SDL_BUTTON_MIDDLE) {
        mouseButtonStates_[MouseButtons::MIDDLE] = false;
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
        mouseButtonStates_[MouseButtons::RIGHT] = false;
    }
}

void InputHandler::onMouseButtonDown(SDL_Event& event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        mouseButtonStates_[LEFT] = true;
    }
    if (event.button.button == SDL_BUTTON_MIDDLE) {
        mouseButtonStates_[MIDDLE] = true;
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
        mouseButtonStates_[RIGHT] = true;
    }
}

void InputHandler::onMouseWheelEvent(SDL_Event& event) { mouseScrollAmount_ = event.wheel.y; }

void InputHandler::onWindowEvent(SDL_Event& event) {
    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        windowResized_ = true;
    }
}

void InputHandler::onTextInput(SDL_Event& event) {

    auto holdingCtrl = SDL_GetModState() & KMOD_CTRL;
    auto pressingV = event.text.text[0] == 'v' || event.text.text[0] == 'V';

    if (holdingCtrl || pressingV) {
        textInputText_ += event.text.text;
    }
}

} // namespace VNEngine