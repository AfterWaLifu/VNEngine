#pragma once

#include <string>

namespace VNEngine {

	class State {

	public:
		virtual void Handle() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		
		virtual bool onEnter() = 0;
		virtual bool onExit() = 0;

		virtual std::string GetStateId() const = 0;
	};
}