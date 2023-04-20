#pragma once

#include <string>

#include "Graphic/Artist.h"

namespace VNEngine {

	class State {
		static inline Artist* s_pDrawer;
	public:
		virtual void Handle() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		
		virtual bool onEnter() = 0;
		virtual bool onExit() = 0;

		virtual std::string GetStateId() const = 0;

		static void InitDrawingForStates(Artist* artist) { s_pDrawer = artist; }
	};
}