#pragma once

#include <SDL2/SDL_scancode.h>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include <VNEngine/Controls/Keys.hpp>
#include <VNEngine/Widgets/Vectors.hpp>

namespace VNEngine {

// NOT "enum class" because is being used to access std::array
enum MouseButtons : uint8_t { LEFT = 0, MIDDLE = 1, RIGHT = 2 };

// Singletone handler for either keyboard & mouth
class InputHandler {

public:
    static InputHandler& instance();

    const bool& isRunning();
    void update();

    bool mouseButtonClicked(MouseButtons button);
    bool mouseButtonHeld(MouseButtons button);
    const vec2& mousePos();

    // returns amount of pixels scrolled (positive=up, negative=down)
    std::int32_t mouseScrollAmount();

    // true if pressed AND not held
    bool isKeyPressed(const std::string& key);

    // TODO: check is it truly "only"?
    // true ONLY when held
    bool isKeyHeld(const std::string& key);

    void setWindowResized(bool resized);
    bool windowResized();

    std::string& textInput();

    void setTextInputState(bool state);
    bool textInputState();

    void exit();

private:
    InputHandler();

    static inline std::shared_ptr<InputHandler> instance_ = nullptr;

    bool isRunning_;
    std::array<bool, 3> mouseButtonStates_;
    vec2 mousePos_;
    std::int32_t mouseScrollAmount_;
    std::uint8_t* keyStates_;
    std::vector<SDL_Scancode> keysPressed_;
    bool windowResized_;
    bool textInputEnabled_;
    std::string textInputText_;

    void onKeyDown(SDL_Event& event);
    void onKeyUp(SDL_Event& event);
    void onMouseMove(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);
    void onMouseButtonDown(SDL_Event& event);
    void onMouseWheelEvent(SDL_Event& event);
    void onWindowEvent(SDL_Event& event);
    void onTextInput(SDL_Event& event);
};

#define IH_INSTANCE InputHandler::instance()
} // namespace VNEngine