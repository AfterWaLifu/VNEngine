#include <iostream>

#include "VNEngine.h"

class Game : public VNEngine::App {
public:
	Game(VNEngine::AppInfo info) : VNEngine::App(info) { Load(); }
private:
	void Load() override {
		using namespace VNEngine;
		
		m_Drawer->AddTexture("b", "back.jpg");
		m_Drawer->SetStretchingState(FULLSCREENED);
		m_Drawer->SetBackground("b");
	}
};

int main(int argc, char* argv[]) {
	
	VNEngine::AppInfo info = { "VNApp", 1280, 720, false };
	Game game(info);
	game.Run();

	return 0;
}