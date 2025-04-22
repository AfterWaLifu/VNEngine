#pragma once

#include <string>

#include <VNEngine/Graphic/Artist.hpp>

namespace VNEngine {

class StateMachine;

class State {
    friend StateMachine;

public:
    virtual void handle() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    virtual bool onEnter() = 0;
    virtual bool onExit() = 0;

    virtual std::string stateId() const = 0;

    static void setStatesDrawer(Artist* artist) { drawer_ = artist; }

protected:
    static inline Artist* drawer_;
};

} // namespace VNEngine