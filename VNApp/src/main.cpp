#include <iostream>

#include "VNEngine.h"

class Game : public VNEngine::App {
public:
	Game(VNEngine::AppInfo info) : VNEngine::App(info) { Load(); }
private:
	void Load() override {
		using namespace VNEngine;
		Text* t = new Text({100,100,100,40}, L"Text", {255,255,255,255});
		Button* b = new Button({100,200,100,40}, [](){}, L"Button");
		TextBox* tb = new TextBox({100,300,100,40}, L"TextBox");
		m_WM.AddWidget("text", t);
		m_WM.AddWidget("button", b);
		m_WM.AddWidget("textbox", tb);
		m_WM.RemoveWidget(WidgetsManager::WIDGET_TEXT, "text");
	}
};

int main(int argc, char* argv[]) {
	
	VNEngine::AppInfo info = { "VNApp", 1280, 720, false };
	Game game(info);
	game.Run();

	return 0;
}