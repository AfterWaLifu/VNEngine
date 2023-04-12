#include <iostream>

#include "VNEngine.h"

class Game : public VNEngine::App {
public:
	Game(VNEngine::AppInfo info) : VNEngine::App(info) { Load(); }
private:
	void Load() override {
		VNEngine::TextBox* tb1 = new VNEngine::TextBox({ 300,100,300,50 });
		VNEngine::TextBox* tb2 = new VNEngine::TextBox({ 300,200,300,50 });
		m_Widgets.push_back(tb1);
		m_Widgets.push_back(tb2);
	}
};

int main(int argc, char* argv[]) {
	
	VNEngine::AppInfo info = { "VNApp", 1280, 720, false };
	Game game(info);
	game.Run();

	return 0;
}