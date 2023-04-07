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

		VNEngine::Button* b1 = new VNEngine::Button({ 100,100,100,100 },
			[]() {exit(0); });
		b1->SetText(L"EXIT BUTTON");
		b1->SetFont({"Roboco.ttf", 36});
		m_Widgets.push_back(b1);
	}
};

int main(int argc, char* argv[]) {
	
	VNEngine::AppInfo info = { "VNApp", 1280, 720, false };
	Game game(info);
	game.Run();

	return 0;
}