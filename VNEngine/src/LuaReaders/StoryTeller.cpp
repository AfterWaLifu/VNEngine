#include "StoryTeller.h"
#include "vnepch.h"

namespace VNEngine {

	StoryTeller::StoryTeller()
	{
	}

	StoryTeller::~StoryTeller() {
	}

	void StoryTeller::SetScriptPath(const std::string& path) {
		sScriptsPath = path;
	}

	std::string StoryTeller::GetScriptsPath() {
		return sScriptsPath;
	}

}