#pragma once

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace VNEngine {
	namespace LSM {

		void Save(int num);
		void Load(int num);

		void ToMenu(std::string menuState);
		void GameStart();
		void WipeStates();
		bool GetIfReadingExist();
	}
}