#pragma once

#include <VNEngine/StateMachine/State.hpp>

#include <VNEngine/Widgets/Button.hpp>
#include <VNEngine/Widgets/Text.hpp>
#include <VNEngine/Widgets/TextBox.hpp>

#include <VNEngine/LuaReaders/StoryTeller.hpp>

namespace VNEngine {

class ReadingState : public State {

    std::string m_ReadingState = "reading";
    bool m_Loaded;

    StoryTeller m_ST;

public:
    ReadingState(std::streampos startPos = std::streampos(0));
    ~ReadingState() = default;

    void Handle() override;
    void Update() override;
    void Render() override;

    bool onEnter() override;
    bool onExit() override;

    std::string GetStateId() const override;

    std::streampos GetReaderPos();
    void SetReaderPos(std::streampos pos);
    lua_State* GetLState();
};

} // namespace VNEngine