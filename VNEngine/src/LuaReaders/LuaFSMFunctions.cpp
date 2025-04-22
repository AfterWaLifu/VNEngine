#include <VNEngine/LuaReaders/LuaFSMFunctions.hpp>

#include <VNEngine/StateMachine/MenuState.hpp>
#include <VNEngine/StateMachine/ReadingState.hpp>
#include <VNEngine/StateMachine/SaveLoad.hpp>
#include <VNEngine/StateMachine/StateMachine.hpp>

namespace VNEngine {
namespace LSM {

void Save(int num) { SaveLoad::save(num, SM_INSTANCE.currentDrawer(), SM_INSTANCE.topReaderState()); }

void Load(int num) {
    SM_INSTANCE.wipeStates();
    SM_INSTANCE.pushState(std::make_shared<ReadingState>());
    size_t pos = SaveLoad::load(num, SM_INSTANCE.currentDrawer(), SM_INSTANCE.topReaderState());
    if (pos == SIZE_MAX)
        return;
    SM_INSTANCE.setTopReaderPos(pos);
}

void ToMenu(std::string menuState) { SM_INSTANCE.changeState(std::make_shared<MenuState>(menuState)); }

void GameStart() { SM_INSTANCE.changeState(std::make_shared<ReadingState>()); }

void WipeStates() { SM_INSTANCE.wipeStates(); }

bool GetIfReadingExist() { return SM_INSTANCE.readingExists(); }

} // namespace LSM
} // namespace VNEngine