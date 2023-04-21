#include "Artist.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "Controls/InputHandler.h"
#include "Widgets/Widget.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <chrono>

namespace VNEngine {

	Rect GetTileRect(const Texture* t, const int tile, const std::string& key) {
		Rect source = { 0,0,1,1 };
		if (t->r * t->c > tile) {
			source.w = t->w / t->c;
			source.h = t->h / t->r;
			source.x = source.w * (tile%t->c);
			source.y = source.h * (tile/t->c);
		}
		else {
			VN_LOGS_WARNING("Attemp to get a nonexisting tile of texture '" << key << "'");
		}
		return source;
	}
	Rect GetTileRect(const Texture* t, const int row, const int collumn, const std::string& key) {
		Rect source = { 0,0,1,1 };
		if (t->r > row || t->c > collumn) {
			source.w = t->w / t->c;
			source.h = t->h / t->r;
			source.x = source.w * collumn;
			source.y = source.h * row;
		}
		else {
			VN_LOGS_WARNING("Attemp to get a nonexisting tile of texture '" << key << "'");
		}
		return source;
	}

	Artist::Artist(const std::string& title, int width, int height, bool fullscreen)
		: m_pWindow(nullptr), m_pRenderer(nullptr), m_DrawId(0), m_Background({}),
		WIDTH(width), HEIGHT(height)
	{

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
			VN_LOGS_ERROR("SDL initialization error");
			return;
		}

		int flags = 0;
		if (fullscreen) flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN;
		else flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

		m_pWindow = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WIDTH, HEIGHT,
			flags
		);
		if (m_pWindow == nullptr) {
			VN_LOGS_ERROR("Window creation error");
			return;
		}

		if (TTF_Init() != 0) {
			VN_LOGS_ERROR("Fonts can't be initialized");
			return;
		}

		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
		if (!m_pRenderer) {
			VN_LOGS_ERROR("WHO HAVE BROKEN MY RENDERER!?");
		}
		Widget::TurnOnWidgets(m_pRenderer);
		SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
		
		TextureManager::TextureManagerInit(m_pRenderer);
		m_Queue.clear();

		SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);

		m_Background = {
			{0,0,0,255},
			nullptr,
			"",
			false,
			false,
			{-1,-1,1,1},
			Stretching::STRETCHED
		};

		VN_LOGS_INFO("Window creation succes");
	}

	Artist::~Artist() {
		m_Queue.clear();
		TextureManager::TextureManagerTerminate();
		SDL_DestroyRenderer(m_pRenderer);
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();

		VN_LOGS_INFO("Window deleting succes");
	}

	void Artist::Perform() {
		if (IH_INSTANCE.getIfWindowResized()) WindowResized();

		SDL_SetRenderDrawColor(m_pRenderer, m_Background.backgroundColor.r,
			m_Background.backgroundColor.g, m_Background.backgroundColor.b,
			m_Background.backgroundColor.a);
		SDL_RenderClear(m_pRenderer);

		if (m_Background.drawBackPic) {
			Rect source = {0,0,m_Background.ptexture->w,m_Background.ptexture->h};
			SDL_RenderCopy(m_pRenderer, m_Background.ptexture->sdl_texture,
				&source, &(m_Background.dest));
		}
		
		for (const std::pair<uint32_t, DrawnData>& pair : m_Queue) {
			auto d = &(pair.second);
			SDL_RenderCopy(m_pRenderer, d->texture->sdl_texture,
				&d->source, &d->destination);
		}
	}

	void Artist::DrawAkaFinale() {
		SDL_RenderPresent(m_pRenderer);
		IH_INSTANCE.setIfWindowResized(false);
	}

	void Artist::SetBackground(vec4u8 color) {
		m_Background.backgroundColor = color;
	}

	void Artist::SetBackground(const std::string& key) {
		m_Background.ptexture = nullptr;
		m_Background.ptexture = TM_INSTANCE.getTexture(key);
		if (m_Background.ptexture == nullptr) { 
			VN_LOGS_WARNING("Attemp to make a non added pic a " <<
				"background '" << key <<"'");
			return; 
		}
		m_Background.textureKey = key;
		m_Background.drawBackPic = true;;

		SetStretchingState(GetStretchingState());
	}

	void Artist::SetStretchingState(Stretching state) {
		m_Background.stretchState = state;
		if (m_Background.ptexture != nullptr) {
			vec2 source = { m_Background.ptexture->w,m_Background.ptexture->h };
			switch (m_Background.stretchState) {
			case CENTERED:
				m_Background.dest = { 0,0, source.x,source.y};
				m_Background.dest.x = (WIDTH - m_Background.dest.w) / 2;
				m_Background.dest.y = (HEIGHT - m_Background.dest.h) / 2;
				break;
			case STRETCHED:
				if (HEIGHT / source.y > WIDTH / source.x) {
					float imageAspectRation = (float)WIDTH / source.x;
					m_Background.dest.h = (int)round((float)source.y * imageAspectRation);
					m_Background.dest.w = WIDTH;
					m_Background.dest.x = 0;
					m_Background.dest.y = (HEIGHT - m_Background.dest.h) / 2;
				}
				else {
					float imageAspectRation = (float)HEIGHT / source.y;
					m_Background.dest.w = (int)round((float)source.x * imageAspectRation);
					m_Background.dest.h = HEIGHT;
					m_Background.dest.x = (WIDTH - m_Background.dest.w) / 2;
					m_Background.dest.y = 0;
				}
				break;
			case FULLSCREENED:
			default:
				m_Background.dest = { 0,0,WIDTH, HEIGHT };
			}
		}
	}

	void Artist::SetUsingBackgroundPic(bool picture) {
		m_Background.drawBackPic = picture;
	}

	vec4u8 Artist::GetBackgroundColor() {
		return m_Background.backgroundColor;
	}

	std::string Artist::GetBackgroundPic()
	{
		return m_Background.textureKey;
	}

	Stretching Artist::GetStretchingState()
	{
		return m_Background.stretchState;
	}

	bool Artist::GetDrawingPictureOrColor()
	{
		return m_Background.drawBackPic;
	}

	uint32_t Artist::Draw(const std::string& key, int tileNum, Rect destination) {
		FindFirstEmptyId();

		Texture* tempTexture = TM_INSTANCE.getTexture(key);
		if (tempTexture == nullptr) return UINT32_MAX;
		Rect tempSourceRect = GetTileRect(tempTexture, tileNum, key);
		m_Queue[m_DrawId] = {tempTexture, tempSourceRect, destination};

		return m_DrawId;
	}
	uint32_t Artist::Draw(const std::string& key, int row, int collumn, Rect destination) {
		FindFirstEmptyId();

		Texture* tempTexture = TM_INSTANCE.getTexture(key);
		if (tempTexture == nullptr) return UINT32_MAX;
		Rect tempSourceRect = GetTileRect(tempTexture, row, collumn, key);
		m_Queue[m_DrawId] = { tempTexture, tempSourceRect, destination };

		return m_DrawId;
	}

	void Artist::StopDrawing(const std::string& key) {
		int counter = 0;
		Texture* goalTexture = TM_INSTANCE.getTexture(key);
		if (goalTexture == nullptr) return;

		for (const std::pair<uint32_t, DrawnData>& pair : m_Queue) {
			if (pair.second.texture == goalTexture) {
				++counter;
				m_Queue.erase(pair.first);
			}
		}

		if (counter == 0) {
			VN_LOGS_WARNING("While stopping drawing by key '" << key <<
				"', not a single texture stopped drawing");
		}
		else if (counter > 1) {
			VN_LOGS_WARNING("While stopping drawing by key '" << key <<
						"', has been stoped " << counter << " textures");
		}
	}
	void Artist::StopDrawing(const uint32_t id) {
		auto find = m_Queue.find(id);
		if (find != m_Queue.end()) {
			m_Queue.erase(id);
		}
		else {
			VN_LOGS_WARNING("Attemp to stop drawing a non-drawn texture w/ id '" << id << "'");
		}
	}

	void Artist::AddTexture(const std::string& key, const std::string& path, int rows, int collumns) {
		TM_INSTANCE.addTexture(key, path, rows, collumns);
	}

	void Artist::DeleteTexture(const std::string& key) {
		TM_INSTANCE.delTexture(key);
	}

	void Artist::FindFirstEmptyId() {
		if (m_Queue.empty()) m_DrawId = 0;

		uint32_t guess = 0;
		auto search = m_Queue.find(guess);

		while (search != m_Queue.end()) {
			++guess;
			search = m_Queue.find(guess);
		}

		m_DrawId = guess;
	}

	void Artist::WindowResized() {
		SDL_GetWindowSize(m_pWindow, &WIDTH, &HEIGHT );
		SetStretchingState(GetStretchingState());
		SetStretchingState(m_Background.stretchState);
	}
	
	void Artist::SetWindowResizable(bool resizable) {
		SDL_SetWindowResizable(m_pWindow, (SDL_bool)resizable);
	}

	void Artist::SetWindowSize(vec2 size) {
		WIDTH = size.x;
		HEIGHT = size.y;
		SDL_SetWindowSize(m_pWindow, WIDTH, HEIGHT);
		IH_INSTANCE.setIfWindowResized(true);
	}
	
	vec2 Artist::GetWindowSize() {
		return {WIDTH, HEIGHT};
	}

	void Artist::SetWindowTitle(std::string title) {
		SDL_SetWindowTitle(m_pWindow, title.c_str());
	}
	
	void Artist::SaveScreenshot() {

		std::string datetime = "screenshot ";
		std::time_t t = std::time(nullptr);
		char mbstr[100];
		tm localtime;
		localtime_s(&localtime,&t);
		std::strftime(mbstr, sizeof(mbstr), "%d.%m.%y %H'%M'%S", &localtime);
		datetime += mbstr;
		datetime += ".png";

		SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, WIDTH, HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
		SDL_RenderReadPixels(m_pRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
		IMG_SavePNG(surface, datetime.c_str());
		VN_LOGS_WARNING(SDL_GetError());
		SDL_FreeSurface(surface);
	}
}