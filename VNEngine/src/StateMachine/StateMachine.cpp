#include <VNEngine/StateMachine/StateMachine.hpp>

#include <VNEngine/Audio/AudioPlayer.hpp>
#include <VNEngine/Core/Logger.hpp>
#include <VNEngine/StateMachine/MenuState.hpp>
#include <VNEngine/StateMachine/ReadingState.hpp>
#include <VNEngine/Widgets/WidgetsManager.hpp>

namespace VNEngine {

StateMachine& StateMachine::instance() {
    if (!instance_) {
        instance_ = std::make_shared<StateMachine>();
    }
    return *instance_;
}

void StateMachine::pushState(std::shared_ptr<State>&& state) {
    if (!states_.empty()) {
        WM_INSTANCE.SaveScreen();
        State::drawer_->SaveScreen();
        AP_INSTANCE.pushState();
    }

    states_.push_back(std::move(state));
    if (!(states_.back()->onEnter())) {
        VN_LOGS_WARNING("Something wrong on entering " << states_.back()->stateId() << " state");
    }
}

void StateMachine::changeState(std::shared_ptr<State>&& state) {
    if (!states_.empty()) {
        auto backState = states_.back();

        if (backState->stateId() == "menu" && state->stateId() == "menu") {
            auto menuState = std::static_pointer_cast<MenuState>(backState);

            menuState->redraw(*std::static_pointer_cast<MenuState>(state));
            return;
        }

        if (backState->onExit()) {
            auto&& temp = std::move(states_.back());
            // let it be removed (hopefuly)
            (void)temp;
            states_.pop_back();
        } else {
            VN_LOGS_WARNING("Something wrong on exiting " << backState->stateId() << " state");
        }
    }

    pushState(std::move(state));
}

void StateMachine::popState() {
    if (states_.empty()) {
        return;
    }

    auto&& backState = std::move(states_.back());

    if (!(backState->onExit())) {
        VN_LOGS_WARNING("Something wrong on exiting " << backState->stateId() << " state");
    } else {
        states_.pop_back();
    }

    WM_INSTANCE.PopScreen();
    State::drawer_->PopScreen();
    AP_INSTANCE.popState();
}

void StateMachine::wipeStates() {
    while (!states_.empty()) {
        popState();
    }
    WM_INSTANCE.WipeScreens();
    State::drawer_->WipeScreens();
    AP_INSTANCE.wipeStates();
}

void StateMachine::update() {
    if (states_.empty()) {
        return;
    }

    states_.back()->handle();
    states_.back()->update();
}

void StateMachine::draw() {
    if (states_.empty()) {
        return;
    }
    states_.back()->render();
}

bool StateMachine::readingExists() {
    if (states_.empty()) {
        return false;
    }

    for (auto s : states_) {
        if (s->stateId() == "reading") {
            return true;
        }
    }
    return false;
}

std::streampos StateMachine::topReaderPos() {
    std::streampos sp(0);

    auto it = states_.end();
    while (it != states_.begin() || (*it)->stateId() != "reading") {
        --it;
    }
    if ((*it)->stateId() == "reading") {
        auto r = std::static_pointer_cast<ReadingState>(*it);
        sp = r->GetReaderPos();
    }
    return sp;
}

void StateMachine::setTopReaderPos(size_t pos) {
    auto back = states_.end();
    while (back != states_.begin() || (*back)->stateId() != "reading") {
        --back;
    }
    if ((*back)->stateId() == "reading") {
        auto r = std::static_pointer_cast<ReadingState>(*back);
        r->SetReaderPos(pos);
    }
}

lua_State* StateMachine::topReaderState() {
    auto back = states_.end();
    while (back != states_.begin() || (*back)->stateId() != "reading") {
        --back;
    }
    if ((*back)->stateId() == "reading") {
        auto r = std::static_pointer_cast<ReadingState>(*back);
        return r->GetLState();
    } else {
        return nullptr;
    }
}

std::string StateMachine::topStateID() {
    if (states_.empty())
        return "";
    else
        return states_.back()->stateId();
}

Artist* StateMachine::currentDrawer() { return State::drawer_; }

} // namespace VNEngine