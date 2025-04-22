#include <VNEngine/StateMachine/MenuState.hpp>

#include <VNEngine/Audio/AudioPlayer.hpp>
#include <VNEngine/Controls/InputHandler.hpp>
#include <VNEngine/StateMachine/ReadingState.hpp>
#include <VNEngine/StateMachine/SaveLoad.hpp>
#include <VNEngine/StateMachine/StateMachine.hpp>
#include <VNEngine/Widgets/WidgetsManager.hpp>

namespace VNEngine {

MenuState::MenuState(const std::string& screen) : screenToStart_(screen) {}

void MenuState::handle() {
    if ((IH_INSTANCE.mouseButtonClicked(RIGHT) || IH_INSTANCE.isKeyPressed("escape")) && SM_INSTANCE.readingExists()) {
        SM_INSTANCE.popState();
    }
    if (IH_INSTANCE.isKeyPressed("f") ||
        ((IH_INSTANCE.isKeyHeld("lalt") || IH_INSTANCE.isKeyHeld("ralt")) && IH_INSTANCE.isKeyPressed("enter"))) {
        auto f = State::drawer_->GetWindowFullscreen();
        State::drawer_->SetWindowFullscreen(!f);
    }
}

void MenuState::update() {}

void MenuState::render() {}

void MenuState::redraw(const MenuState& state) {
    WM_INSTANCE.WipeWidgets();
    screenToStart_ = state.screenToStart_;
    ic_.Draw(screenToStart_);
}

bool MenuState::onEnter() {
    ic_.Draw(screenToStart_);
    return true;
}

bool MenuState::onExit() {
    WM_INSTANCE.WipeWidgets();
    AP_INSTANCE.stopMusic();
    drawer_->WipeDrawing();
    return true;
}

std::string MenuState::stateId() const { return menuState_; }

} // namespace VNEngine