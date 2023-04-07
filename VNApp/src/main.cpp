#include <iostream>

#include "VNEngine.h"

class Game : public VNEngine::App {
public:
	Game(VNEngine::AppInfo info) : VNEngine::App(info) { Load(); }
private:
	void Load() override {
		m_Drawer->AddTexture("back", "back.jpg");
		m_Drawer->AddTexture("m", "masyunya.png");
		m_Drawer->SetBackground("back");
		m_Drawer->SetStretchingState(VNEngine::STRETCHED);
		VNEngine::Text* t1 = new VNEngine::Text({ 100,100,100,100 },
			L"HI", { 255,0,0,255 }, { "Roboto.ttf", 36 });
		t1->SetAlign(VNEngine::ALIGN_CENTER);
		t1->SetBackImage("m");
		m_Widgets.push_back(t1);
	}
};

int main(int argc, char* argv[]) {
	
	VNEngine::AppInfo info = { "VNApp", 1280, 720, false };
	Game game(info);
	game.Run();

	return 0;
}