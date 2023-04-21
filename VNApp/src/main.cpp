#include <iostream>

#include "VNEngine.h"

class Game : public VNEngine::App {
public:
	Game(VNEngine::AppInfo info) : VNEngine::App(info) { Load(); }
private:
	void Load() override {
		using namespace VNEngine;

		m_Drawer->AddTexture("back","back.jpg");
		m_Drawer->AddTexture("m", "masyunya.png");
		m_Drawer->Draw("m", 0, {600,50,320,180});
		m_Drawer->SetBackground("m");

		Button* b = new Button({100,100,200,80}, nullptr, L"BUTTON");
		b->SetFontSize(40);
		b->SetFocusBorderColor({ 0,0,0,255 });

		TextBox* tb = new TextBox({ 100,200,200,80 });
		tb->SetFontSize(40);
		tb->SetBackgroundColor({200,200,200,255});

		WM_INSTANCE.AddWidget("b", b);
		WM_INSTANCE.AddWidget("tb", tb);
	}
};

int main(int argc, char* argv[]) {
	
	VNEngine::AppInfo info = { "VNApp", 1280, 720, false };
	Game game(info);
	game.Run();

	return 0;
}