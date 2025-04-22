#pragma once

#include <VNEngine/StateMachine/State.hpp>

#include <VNEngine/LuaReaders/InterfaceCreator.hpp>

namespace VNEngine {

class MenuState : public State {
public:
    MenuState(const std::string& screen);

    void redraw(const MenuState& state);

    void handle() override;
    void update() override;
    void render() override;

    bool onEnter() override;
    bool onExit() override;

    std::string stateId() const override;

private:
    std::string menuState_ = "menu";
    std::string screenToStart_;

    InterfaceCreator ic_;
};
} // namespace VNEngine
