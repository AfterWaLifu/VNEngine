#pragma once

#include <VNEngine/StateMachine/State.hpp>

#include <VNEngine/Widgets/Button.hpp>
#include <VNEngine/Widgets/Text.hpp>
#include <VNEngine/Widgets/TextBox.hpp>

#include <VNEngine/LuaReaders/StoryTeller.hpp>

namespace VNEngine {

class ReadingState : public State {

public:
    ReadingState(std::streampos startPos = std::streampos(0));
    ~ReadingState() = default;

    void handle() override;
    void update() override;
    void render() override;

    bool onEnter() override;
    bool onExit() override;

    std::string stateId() const override;

    std::streampos GetReaderPos();
    void SetReaderPos(std::streampos pos);
    lua_State* GetLState();

private:
    std::string readingState_ = "reading";
    bool loaded_;

    StoryTeller st_;
};

} // namespace VNEngine