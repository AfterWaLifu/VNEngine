#include <VNEngine/LuaReaders/InterfaceCreator.hpp>

#include <VNEngine/Core/Logger.hpp>
#include <VNEngine/LuaReaders/LuaWidgetFunctions.hpp>
#include <VNEngine/LuaReaders/StoryTeller.hpp>
#include <VNEngine/Widgets/WidgetsManager.hpp>

#include <LuaBridge/LuaBridge.h>

#define lb luabridge

namespace VNEngine {

InterfaceCreator::InterfaceCreator() {
    RegisterInterfaceFunctions();
    RegisterSettingsFunctons();
    RegisterFSMFunctuins();
}

InterfaceCreator::~InterfaceCreator() {}

void InterfaceCreator::Draw(const std::string& what) {
    WM_INSTANCE.WipeWidgets();

    if (luaL_dofile(L, (StoryTeller::GetScriptsPath() + "look.lua").c_str())) {
        VN_LOGS_WARNING("Can't open 'look.lua' in a scripts folder");
        VN_LOGS_WARNING(lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    lb::LuaRef lr = lb::getGlobal(L, what.c_str());
    if (lr.isFunction()) {
        lr();
    } else {
        VN_LOGS_WARNING("Error on looking for name below in the 'look.lua'" << what);
    }
}
} // namespace VNEngine