#include "LuaFSMFunctions.h"
#include "vnepch.h"

#include "StateMachine/StateMachine.h"
#include "StateMachine/SaveLoad.h"
#include "StateMachine/MenuState.h"
#include "StateMachine/ReadingState.h"

namespace VNEngine {
	namespace LSM {
		
		void Save(int num) {
			SaveLoad::Save(num, SM_INSTANCE.GetCurrentDrawer());
		}

		void Load(int num) {
			SM_INSTANCE.WipeStates();
			int pos = SaveLoad::Load(num, SM_INSTANCE.GetCurrentDrawer());
			if (pos < 0) return;
			SM_INSTANCE.PushState(new ReadingState(pos));
		}

		void ToMenu(std::string menuState) {
			SM_INSTANCE.ChangeState(new MenuState(menuState));
		}

		void GameStart() {
			SM_INSTANCE.ChangeState(new ReadingState());
		}

		void WipeStates() {
			SM_INSTANCE.WipeStates();
		}

		bool GetIfReadingExist() {
			return SM_INSTANCE.isThereAReading();
		}

	}
}