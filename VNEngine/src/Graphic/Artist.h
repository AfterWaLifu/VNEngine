#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <stdint.h>

#include "Graphic/TextureManager.h"
#include "Widgets/Vectors.h"
#include "Widgets/Widget.h"

namespace VNEngine{

	typedef SDL_Rect Rect;

	struct DrawnData {
		Texture* texture;
		Rect source;
		Rect destination;
	};

	enum Stretching : uint8_t {
		CENTERED,
		FULLSCREENED,
		STRETCHED
	};

	class Artist {
	private:
		SDL_Window* m_pWindow;
		SDL_Renderer* m_pRenderer;

		int WIDTH, HEIGHT;

		std::unordered_map<uint32_t, DrawnData> m_Queue;
		uint32_t m_DrawId;

		void FindFirstEmptyId();
		void WindowResized();

		struct {
			vec4u8 backgroundColor;
			Texture* ptexture;
			std::string textureKey;
			bool drawBackPic;
			bool colorChanged;
			Rect dest;
			
			Stretching stretchState;
		} m_Background;

	public:
		Artist(const std::string& title, int width, int height, bool fullscreen);
		~Artist();

		void Perform();
		void DrawAkaFinale();

		void SetBackground(vec4u8 color = { 0, 0, 0, 255 });
		void SetBackground(const std::string& key);
		void SetStretchingState(Stretching state);
		void SetUsingBackgroundPic(bool picture = true);

		vec4u8		GetBackgroundColor();
		std::string	GetBackgroundPic();
		Stretching	GetStretchingState();
		bool		GetDrawingPictureOrColor();

		uint32_t Draw(const std::string& key, int tileNum = 0, Rect destination = {0,0,100,100});
		uint32_t Draw(const std::string& key, int row = 0, int collumn = 0, Rect destination = { 0,0,100,100 });
		void StopDrawing(const std::string& key);
		void StopDrawing(const uint32_t id);

		void AddTexture(const std::string& key, const std::string& path, int rows = 1, int collumns = 1);
		void DeleteTexture(const std::string& key);

		void SetWindowResizable(bool resizable = true);
	};
}