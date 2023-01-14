#include <iostream>

#include "VNEngine.h"

int main(int argc, char* argv[]) {
	
	VNEngine::AppInfo info = { "VNApp", 1280, 720, false };
	VNEngine::App* app = new VNEngine::App(info);
	app->Run();
	delete app;

	return 0;
}