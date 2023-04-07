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
		VNEngine::Text* t1 = new VNEngine::Text({ 100,100,400,40 },
			L"TEXT", { 255,100,100,255 }, { "Roboto.ttf", 36 });
		t1->SetBackgroundColor({0,100,100,150});
		t1->SetAlign(VNEngine::ALIGN_CENTER);
		m_Widgets.push_back(t1);
		VNEngine::Text* t2 = new VNEngine::Text({ 100,200,400,40 },
			L"TEXT", { 100,100,255,255 }, { "Roboto.ttf", 14 });
		t2->SetBackgroundColor({0,0,0,0});
		t2->SetAlign(VNEngine::ALIGN_LEFT);
		m_Widgets.push_back(t2);
		VNEngine::Text* t3 = new VNEngine::Text({ 100,300,400,40 },
			L"TEXT", { 100,255,100,255 }, { "Roboto.ttf", 36 });
		t3->SetBackgroundColor({ 100,0,100,150 });
		t3->SetAlign(VNEngine::ALIGN_RIGHT);
		m_Widgets.push_back(t3);
	}
};

int main(int argc, char* argv[]) {
	
	VNEngine::AppInfo info = { "VNApp", 1280, 720, false };
	Game game(info);
	game.Run();

	return 0;
}