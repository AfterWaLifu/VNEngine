#include <VNEngine/LuaReaders/LuaFSMFunctions.hpp>

#include <VNEngine/StateMachine/MenuState.hpp>
#include <VNEngine/StateMachine/ReadingState.hpp>
#include <VNEngine/StateMachine/SaveLoad.hpp>
#include <VNEngine/StateMachine/StateMachine.hpp>

namespace VNEngine {
namespace LSM {

void Save(int num) { SaveLoad::Save(num, SM_INSTANCE.GetCurrentDrawer(), SM_INSTANCE.GetTopReaderState()); }

void Load(int num) {
    SM_INSTANCE.WipeStates();
    SM_INSTANCE.PushState(new ReadingState());
    size_t pos = SaveLoad::Load(num, SM_INSTANCE.GetCurrentDrawer(), SM_INSTANCE.GetTopReaderState());
    if (pos == SIZE_MAX)
        return;
    SM_INSTANCE.SetTopReaderPos(pos);
}

void ToMenu(std::string menuState) { SM_INSTANCE.ChangeState(new MenuState(menuState)); }

void GameStart() { SM_INSTANCE.ChangeState(new ReadingState()); }

void WipeStates() { SM_INSTANCE.WipeStates(); }

bool GetIfReadingExist() { return SM_INSTANCE.isThereAReading(); }

} // namespace LSM
} // namespace VNEngine