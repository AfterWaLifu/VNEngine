#pragma once

#include <lua.hpp>

#include <VNEngine/LuaReaders/LuaSetsFunctions.hpp>
#include <VNEngine/LuaReaders/LuaWidgetFunctions.hpp>

namespace VNEngine {

class LuaReader {
protected:
    lua_State* L;

    void RegisterInterfaceFunctions();
    void RegisterSettingsFunctons();
    void RegisterFSMFunctuins();

public:
    LuaReader();
    virtual ~LuaReader();
};
} // namespace VNEngine