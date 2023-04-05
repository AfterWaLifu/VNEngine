#include <iostream>

#include "VNEngine.h"

class Game : public VNEngine::App {
public:
	Game(VNEngine::AppInfo info) : VNEngine::App(info) { Load(); }
private:
	void Load() override {
		m_Drawer->AddTexture("back", "back.jpg");
		m_Drawer->SetBackground("back");
		m_Drawer->SetStretchingState(VNEngine::STRETCHED);
		m_Widgets.push_back( (VNEngine::Widget*) new VNEngine::Text({0,0}, L"TEXT"));
	}
};

int main(int argc, char* argv[]) {
	
	VNEngine::AppInfo info = { "VNApp", 1280, 720, false };
	Game game(info);
	game.Run();

	return 0;
}