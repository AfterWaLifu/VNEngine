#include <iostream>

#include "VNEngine.h"

class Game : public VNEngine::App {
public:
	Game(VNEngine::AppInfo info) : VNEngine::App(info) { Load(); }
private:
	void Load() override {

		VNEngine::Button* b1 = new VNEngine::Button({ 100,100,100,100 }, []() {exit(0); });
		b1->SetFontSize(36);
		b1->SetText(L"EHEHE");
		b1->SetTextColor({255,0,0,255});
		b1->SetGeometry({ 300,100,400,100 });
		b1->SetAlign((VNEngine::Alignment)(VNEngine::ALIGN_UP | VNEngine::ALIGN_RIGHT));
		m_Widgets.push_back(b1);
	}
};

int main(int argc, char* argv[]) {
	
	VNEngine::AppInfo info = { "VNApp", 1280, 720, false };
	Game game(info);
	game.Run();

	return 0;
}