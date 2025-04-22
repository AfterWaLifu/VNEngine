#pragma once

#include <VNEngine/StateMachine/State.hpp>

#include <memory>

struct lua_State;

namespace VNEngine {

class StateMachine {

public:
    static StateMachine& instance();

    void pushState(std::shared_ptr<State>&& state);
    void changeState(std::shared_ptr<State>&& state);
    void popState();
    void wipeStates();

    void update();
    void draw();

    bool readingExists();

    std::streampos topReaderPos();
    void setTopReaderPos(std::size_t pos);
    lua_State* topReaderState();

    std::string topStateID();
    Artist* currentDrawer();

private:
    std::vector<std::shared_ptr<State>> states_;

    static inline std::shared_ptr<StateMachine> instance_ = nullptr;
};

#define SM_INSTANCE StateMachine::instance()
} // namespace VNEngine