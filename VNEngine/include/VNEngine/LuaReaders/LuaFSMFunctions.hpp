#pragma once

#include <string>

namespace VNEngine {
namespace LSM {

void Save(int num);
void Load(int num);

void ToMenu(std::string menuState);
void GameStart();
void WipeStates();
bool GetIfReadingExist();
} // namespace LSM
} // namespace VNEngine