#include "App.h"
#include "vnepch.h"

namespace VNEngine {

	App::App(AppInfo& defaultAppInfo)
		:m_Window(nullptr), m_WorkResult(0)
	{
		
		m_Window = std::make_unique<Window>(
			defaultAppInfo.WindowTitle,
			defaultAppInfo.Width, defaultAppInfo.Height,
			defaultAppInfo.Fullscreen
		);
	}

	App::~App() {
		
	}
	
	int App::Run() {
		m_WorkResult += Update();
		m_WorkResult += HandleEvents();
		m_WorkResult += Draw();
		return m_WorkResult;
	}

	int App::Update()
	{
		return 0;
	}

	int App::HandleEvents()
	{
		return 0;
	}

	int App::Draw()
	{
		return 0;
	}
}