#include <VNEngine/StateMachine/ReadingState.hpp>

#include <VNEngine/Audio/AudioPlayer.hpp>
#include <VNEngine/Controls/InputHandler.hpp>
#include <VNEngine/LuaReaders/InterfaceCreator.hpp>
#include <VNEngine/LuaReaders/StoryTeller.hpp>
#include <VNEngine/StateMachine/MenuState.hpp>
#include <VNEngine/StateMachine/StateMachine.hpp>
#include <VNEngine/Widgets/WidgetsManager.hpp>

namespace VNEngine {

ReadingState::ReadingState(std::streampos startPos) : loaded_(false) {
    if (startPos != 0) {
        loaded_ = true;
        SetReaderPos(startPos);
    }
}

void ReadingState::handle() {
    if (IH_INSTANCE.mouseButtonClicked(RIGHT) || IH_INSTANCE.isKeyPressed("escape")) {
        SM_INSTANCE.pushState(std::make_shared<MenuState>("save"));
    }

    if (IH_INSTANCE.mouseButtonClicked(LEFT) || IH_INSTANCE.isKeyPressed("space") ||
        IH_INSTANCE.isKeyPressed("enter")) {

        if (WM_INSTANCE.GetHiden()) {
            WM_INSTANCE.SetHiden(false);
        } else {
            st_.Go();
        }

        if (st_.GetCompleted()) {
            SM_INSTANCE.wipeStates();
            SM_INSTANCE.changeState(std::make_shared<MenuState>("mainmenu"));
            return;
        }
    }

    if (IH_INSTANCE.isKeyHeld("lctrl") || IH_INSTANCE.isKeyHeld("rctrl")) {
        st_.SetSkip(true);
    }

    if (IH_INSTANCE.isKeyPressed("h")) {
        WM_INSTANCE.SetHiden(!(WM_INSTANCE.GetHiden()));
    }

    if (IH_INSTANCE.isKeyPressed("f") ||
        ((IH_INSTANCE.isKeyHeld("lalt") || IH_INSTANCE.isKeyHeld("ralt")) && IH_INSTANCE.isKeyPressed("enter"))) {
        auto f = State::drawer_->GetWindowFullscreen();
        State::drawer_->SetWindowFullscreen(!f);
    }

    if (IH_INSTANCE.isKeyPressed("s")) {
        State::drawer_->SaveScreenshot();
    }
}

void ReadingState::update() {}

void ReadingState::render() {}

bool ReadingState::onEnter() {
    if (!loaded_) {
        InterfaceCreator ic;
        ic.Draw("game");
    }
    st_.DoFile("game.lua");

    return true;
}

bool ReadingState::onExit() {
    WM_INSTANCE.WipeWidgets();
    AP_INSTANCE.stopMusic();
    drawer_->WipeDrawing();
    return true;
}

std::string ReadingState::stateId() const { return readingState_; }

std::streampos ReadingState::GetReaderPos() { return st_.GetCurrentPos(); }

void ReadingState::SetReaderPos(std::streampos pos) { st_.SetCurrentPos(pos); }

lua_State* ReadingState::GetLState() { return st_.getl(); }
} // namespace VNEngine