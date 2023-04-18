#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <stdint.h>

#include "Graphic/TextureManager.h"
#include "Widgets/Vectors.h"
#include "Widgets/Widget.h"

namespace VNEngine{

	//it will be replaced w/ vec4 I guess
	typedef SDL_Rect Rect;

	/// Everything needed to draw a texture (*will be changed w/ adding animations)
	struct DrawnData {
		Texture* texture;
		Rect source;
		Rect destination;
	};

	/// Stretching states enum
	enum Stretching : uint8_t {
		CENTERED,
		FULLSCREENED,
		STRETCHED
	};

	/**
	* @brief Class that draws
	* Creates a window and draws into in what you tell him to.
	*/
	class Artist {
	private:
		SDL_Window* m_pWindow;
		SDL_Renderer* m_pRenderer;

		int WIDTH, HEIGHT;

		std::unordered_map<uint32_t, DrawnData> m_Queue;	/// Queue on drawing (data and indexes)
		uint32_t m_DrawId;									/// First empty ID to draw

		void FindFirstEmptyId();	/// Finds value for m_DrawId
		void WindowResized();		/// Manages window resizing

		/// Knows everything about your window background
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
		/**
		* @brief Constructor of the (Performance) Artist
		* @param	title		Window title you want
		* @param	width		Window width
		* @param	height		Window height
		* @param	fullscreen	True if you wanna fullscreen
		*/
		Artist(const std::string& title, int width, int height, bool fullscreen);
		~Artist();	/// Artist destructor (sorry, artist)

		/**
		* @brief Performing drawing of everything (almost)
		* Cleaning window, drawing back and everything on queue.
		* btw checking if window resized
		*/
		void Perform();
		void DrawAkaFinale();	/// Closing drawing process

		void SetBackground(vec4u8 color = { 0, 0, 0, 255 });
		void SetBackground(const std::string& key);
		void SetStretchingState(Stretching state);
		void SetUsingBackgroundPic(bool picture = true);	/// True for picture, false for color

		vec4u8		GetBackgroundColor();
		std::string	GetBackgroundPic();
		Stretching	GetStretchingState();
		bool		GetDrawingPictureOrColor();	/// True for picture, false for color

		/**
		* @brief Adding sprite to the queue on drawing
		* @return ID of drawn texture, so if you use a lot of the same texture, you can control it easily
		*/
		uint32_t Draw(const std::string& key, int tileNum = 0, Rect destination = {0,0,100,100});
		uint32_t Draw(const std::string& key, int row = 0, int collumn = 0, Rect destination = { 0,0,100,100 });
		void StopDrawing(const std::string& key);
		/**
		* @brief Removing sprite from the queue on drawing
		* @param id It's ID you get after Draw func
		*/
		void StopDrawing(const uint32_t id);

		void AddTexture(const std::string& key, const std::string& path, int rows = 1, int collumns = 1);
		void DeleteTexture(const std::string& key);

		void SetWindowResizable(bool resizable = true);
	};
}