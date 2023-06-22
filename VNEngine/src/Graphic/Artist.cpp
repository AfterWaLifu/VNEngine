#include "Artist.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "Controls/InputHandler.h"
#include "Widgets/Widget.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <chrono>

namespace VNEngine {

	vec4 GetTileRect(const Texture* t, const int tile, const std::string& key) {
		vec4 source = { 0,0,1,1 };
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
	vec4 GetTileRect(const Texture* t, const int row, const int collumn, const std::string& key) {
		vec4 source = { 0,0,1,1 };
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
		: m_pWindow(nullptr), m_pRenderer(nullptr), m_GameScreen(nullptr), m_DrawId(0), m_Background({}),
		m_WindowSize({width,height}), m_BaseWindowSize({ width,height }), m_PrevWindowSize({0,0}),
		m_PrevBackgroundSize({0,0,0,0}), m_Fullscreen(fullscreen), m_WindowTitle(title), m_Resizable(true)
	{

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
			VN_LOGS_ERROR("SDL initialization error");
			return;
		}

		m_pWindow = SDL_CreateWindow(
			m_WindowTitle.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			m_WindowSize.x, m_WindowSize.y,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
		);
		if (m_pWindow == nullptr) {
			VN_LOGS_ERROR("Window creation error");
			return;
		}

		m_GameScreen = SDL_CreateRGBSurfaceWithFormat(0, m_WindowSize.x, m_WindowSize.y, 32, SDL_PIXELFORMAT_ARGB8888);

		if (TTF_Init() != 0) {
			VN_LOGS_ERROR("Fonts can't be initialized");
			return;
		}

		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
		if (!m_pRenderer) {
			VN_LOGS_ERROR("WHO HAVE BROKEN MY RENDERER!?");
		}
		Widget::TurnOnWidgets({ m_pRenderer, &m_WindowSize,
			&m_PrevWindowSize, &m_BaseWindowSize, &m_Background.dest, &m_PrevBackgroundSize,
			(uint8_t*)&(m_Background.stretchState) });
		SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
		
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

		if (fullscreen) SetWindowFullscreen(fullscreen);

		VN_LOGS_INFO("Window creation succes");
	}

	Artist::~Artist() {
		m_Queue.clear();
		TextureManager::TextureManagerTerminate();
		SDL_FreeSurface(m_GameScreen);
		SDL_DestroyRenderer(m_pRenderer);
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();

		VN_LOGS_INFO("Window deleting succes");
	}

	void Artist::Perform() {
		if (IH_INSTANCE.getIfWindowResized()) 
			WindowResized();

		SDL_SetRenderDrawColor(m_pRenderer, m_Background.backgroundColor.r,
			m_Background.backgroundColor.g, m_Background.backgroundColor.b,
			m_Background.backgroundColor.a);
		SDL_RenderClear(m_pRenderer);

		if (m_Background.drawBackPic) {
			vec4 source = {0,0,m_Background.ptexture->w,m_Background.ptexture->h};
			SDL_RenderCopy(m_pRenderer, m_Background.ptexture->sdl_texture,
				(SDL_Rect*)(&source), (SDL_Rect*)&(m_Background.dest));
		}
		
		for (const std::pair<uint32_t, DrawnData>& pair : m_Queue) {
			auto d = &(pair.second);
			SDL_RenderCopy(m_pRenderer, d->texture->sdl_texture,
				(SDL_Rect*)(&d->source), (SDL_Rect*)(&d->destination));
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
		if (m_Background.ptexture) {
			m_PrevBackgroundSize = m_Background.dest;
			vec2 source = { m_Background.ptexture->w,m_Background.ptexture->h };
			switch (m_Background.stretchState) {
			case CENTERED:
				m_Background.dest = { 0,0, source.x,source.y};
				m_Background.dest.x = (m_WindowSize.x - m_Background.dest.w) / 2;
				m_Background.dest.y = (m_WindowSize.y - m_Background.dest.h) / 2;
				break;

			case STRETCHED:
				if (m_PrevWindowSize.x > m_WindowSize.x ||
					m_PrevWindowSize.y < m_WindowSize.y) {
					float imageAspectRation = (float)m_WindowSize.x / source.x;
					m_Background.dest.h = (int)round((float)source.y * imageAspectRation);
					m_Background.dest.w = m_WindowSize.x;
					m_Background.dest.x = 0;
					m_Background.dest.y = (m_WindowSize.y - m_Background.dest.h) / 2;
				}
				else {
					float imageAspectRation = (float)m_WindowSize.y / source.y;
					m_Background.dest.w = (int)round((float)source.x * imageAspectRation);
					m_Background.dest.h = m_WindowSize.y;
					m_Background.dest.x = (m_WindowSize.x - m_Background.dest.w) / 2;
					m_Background.dest.y = 0;
				}

				if (m_Background.dest.w > m_WindowSize.x) {
					float imageAspectRation = (float)m_WindowSize.x / source.x;
					m_Background.dest.h = (int)round((float)source.y * imageAspectRation);
					m_Background.dest.w = m_WindowSize.x;
					m_Background.dest.x = 0;
					m_Background.dest.y = (m_WindowSize.y - m_Background.dest.h) / 2;
				}
				else if (m_Background.dest.h > m_WindowSize.y) {
					float imageAspectRation = (float)m_WindowSize.y / source.y;
					m_Background.dest.w = (int)round((float)source.x * imageAspectRation);
					m_Background.dest.h = m_WindowSize.y;
					m_Background.dest.x = (m_WindowSize.x - m_Background.dest.w) / 2;
					m_Background.dest.y = 0;
				}
				break;

			case FULLSCREENED:
			default:
				m_Background.dest = { 0,0,m_WindowSize.x, m_WindowSize.y };
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

	uint32_t Artist::Draw(const std::string& key, int tileNum, vec4 destination) {
		FindFirstEmptyId();

		Texture* tempTexture = TM_INSTANCE.getTexture(key);
		if (tempTexture == nullptr) return UINT32_MAX;
		vec4 tempSourceRect = GetTileRect(tempTexture, tileNum, key);
		m_Queue[m_DrawId] = {tempTexture, tempSourceRect, destination};

		return m_DrawId;
	}
	uint32_t Artist::Draw(const std::string& key, int row, int collumn, vec4 destination) {
		FindFirstEmptyId();

		Texture* tempTexture = TM_INSTANCE.getTexture(key);
		if (tempTexture == nullptr) return UINT32_MAX;
		vec4 tempSourceRect = GetTileRect(tempTexture, row, collumn, key);
		m_Queue[m_DrawId] = { tempTexture, tempSourceRect, destination };

		return m_DrawId;
	}

	uint32_t Artist::DrawR(const std::string& key, int tileNum, vec4 destination) {
		FindFirstEmptyId();

		Texture* tempTexture = TM_INSTANCE.getTexture(key);
		if (tempTexture == nullptr) return UINT32_MAX;
		vec4 tempSourceRect = GetTileRect(tempTexture, tileNum, key);
		if (m_BaseWindowSize.x != m_WindowSize.x || m_BaseWindowSize.y != m_WindowSize.y) {
			float relatex, relatey;
			if (m_Background.stretchState == STRETCHED) {
				relatex = (float)m_Background.dest.w / (float)m_BaseWindowSize.x;
				relatey = (float)m_Background.dest.h / (float)m_BaseWindowSize.y;
				destination.x = m_Background.dest.x + (int)((float)destination.x * relatex);
				destination.y = m_Background.dest.y + (int)((float)destination.y * relatey);
				destination.w = (int)((float)destination.w * relatex);
				destination.h = (int)((float)destination.h * relatey);
			}
			else {
				relatex = (float)m_WindowSize.x / (float)m_BaseWindowSize.x;
				relatey = (float)m_WindowSize.y / (float)m_BaseWindowSize.y;
				destination.x = (int)((float)destination.x * relatex);
				destination.y = (int)((float)destination.y * relatey);
				destination.w = (int)((float)destination.w * relatex);
				destination.h = (int)((float)destination.h * relatey);
			}
		}
		m_Queue[m_DrawId] = { tempTexture, tempSourceRect, destination };

		return m_DrawId;
	}
	uint32_t Artist::DrawR(const std::string& key, int row, int collumn, vec4 destination) {
		FindFirstEmptyId();

		Texture* tempTexture = TM_INSTANCE.getTexture(key);
		if (tempTexture == nullptr) return UINT32_MAX;
		vec4 tempSourceRect = GetTileRect(tempTexture, row, collumn, key);
		if (m_BaseWindowSize.x != m_WindowSize.x || m_BaseWindowSize.y != m_WindowSize.y) {
			float relatex = (float)m_WindowSize.x / (float)m_BaseWindowSize.x;
			float relatey = (float)m_WindowSize.y / (float)m_BaseWindowSize.y;
			destination.x = (int)((float)destination.x * relatex);
			destination.y = (int)((float)destination.y * relatey);
			destination.w = (int)((float)destination.w * relatex);
			destination.h = (int)((float)destination.h * relatey);
		}
		m_Queue[m_DrawId] = { tempTexture, tempSourceRect, destination };

		return m_DrawId;
	}

	void Artist::StopDrawing(const std::string& key) {
		int counter = 0;

		for (std::pair<const uint32_t, DrawnData>& pair : m_Queue) {
			if (pair.second.texture->key == key) {
				++counter;
				m_Queue.erase(pair.first);
				break;
			}
		}
	}
	void Artist::StopDrawing(const uint32_t id) {
		auto find = m_Queue.find(id);
		if (find != m_Queue.end()) {
			m_Queue.erase(id);
		}
	}

	void Artist::WipeDrawing() {
		if (!(m_Queue.empty())) m_Queue.clear();
		m_Background.textureKey = "";
		m_Background.ptexture = nullptr;
		m_Background.drawBackPic = false;
		m_DrawId = 0;
	}

	void Artist::AddTexture(const std::string& key, const std::string& path, int rows, int collumns) {
		TM_INSTANCE.addTexture(key, path, rows, collumns);
	}

	void Artist::DeleteTexture(const std::string& key) {
		if (m_Queue.size()) {
			for (auto i = m_Queue.begin(); i != m_Queue.end(); ++i) {
				if (i->second.texture->key == key) m_Queue.erase(i);
				if (m_Queue.empty()) break;
			}
		}
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
		SDL_GetWindowSize(m_pWindow, &m_WindowSize.x, &m_WindowSize.y );
		if (m_PrevWindowSize.x == m_WindowSize.x && m_PrevWindowSize.y == m_WindowSize.y) return;
		SetStretchingState(m_Background.stretchState);
		ResizeTextures();
	}

	void Artist::ResizeTextures() {
		if (m_Queue.empty()) return;

		if (m_Background.stretchState == STRETCHED 
			&& m_Background.drawBackPic && m_Background.ptexture != nullptr) {

			float wratio = (float)m_Background.dest.w / (float)m_PrevBackgroundSize.w;
			float hratio = (float)m_Background.dest.h / (float)m_PrevBackgroundSize.h;
			if (wratio == 1.0f && hratio == 1.0f) return;

			for (auto texturePair = m_Queue.begin();
				texturePair != m_Queue.end();
				++texturePair) {
				DrawnData& dd = (*texturePair).second;

				vec2 coordsAtBack = {
					dd.destination.x - m_PrevBackgroundSize.x,
					dd.destination.y - m_PrevBackgroundSize.y
				};

				dd.destination.x = m_Background.dest.x + (int)round((float)coordsAtBack.x*wratio);
				dd.destination.y = m_Background.dest.y + (int)round((float)coordsAtBack.y*hratio);
				dd.destination.w = (int)round((float)dd.destination.w * wratio);
				dd.destination.h = (int)round((float)dd.destination.h * hratio);
			}
		}
		else {
			float verticalRation = (float)m_WindowSize.y / m_PrevWindowSize.y;
			float horizontRation = (float)m_WindowSize.x / m_PrevWindowSize.x;
			if (verticalRation == 1.0f && horizontRation == 1.0f) return;
			for (auto texturePair = m_Queue.begin();
				texturePair != m_Queue.end();
				++texturePair) {
				DrawnData& dd = (*texturePair).second;

				dd.destination.x = (int)round((float)dd.destination.x * horizontRation);
				dd.destination.y = (int)round((float)dd.destination.y * verticalRation);
				dd.destination.w = (int)round((float)dd.destination.w * horizontRation);
				dd.destination.h = (int)round((float)dd.destination.h * verticalRation);
			}
		}
	}
	
	void Artist::SetWindowResizable(bool resizable) {
		SDL_SetWindowResizable(m_pWindow, (SDL_bool)resizable);
		m_Resizable = resizable;
	}

	bool Artist::GetWindowResizable() {
		return m_Resizable;
	}

	void Artist::SetWindowSize(vec2 size) {
		m_PrevWindowSize = m_WindowSize;
		m_WindowSize = size;
		SDL_SetWindowSize(m_pWindow, m_WindowSize.x, m_WindowSize.y);
		IH_INSTANCE.setIfWindowResized(true);
	}
	
	vec2 Artist::GetWindowSize() {
		return m_WindowSize;
	}

	void Artist::SetBaseWinSize(vec2 size) {
		m_BaseWindowSize = size;
	}

	vec2 Artist::GetBaseWinSize() {
		return m_BaseWindowSize;
	}

	void Artist::SetWindowTitle(const std::string& title) {
		SDL_SetWindowTitle(m_pWindow, title.c_str());
		m_WindowTitle = title;
	}

	std::string Artist::GetWindowTitle() {
		return m_WindowTitle;
	}

	void Artist::SetWindowFullscreen(bool fullscreen) {
		m_Fullscreen = fullscreen;
		if (m_Fullscreen) {
			SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
			m_PrevWindowSize = m_WindowSize;
			IH_INSTANCE.setIfWindowResized(true);
		}
		else {
			if (m_PrevWindowSize.x == 0 && m_PrevWindowSize.y == 0) m_PrevWindowSize = m_BaseWindowSize;
			SDL_SetWindowFullscreen(m_pWindow, 0);
			SetWindowSize(m_PrevWindowSize);
		}
	}

	bool Artist::GetWindowFullscreen() {
		return m_Fullscreen;
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

		SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, m_WindowSize.x, m_WindowSize.y, 32, SDL_PIXELFORMAT_ARGB8888);
		SDL_RenderReadPixels(m_pRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
		IMG_SavePNG(surface, datetime.c_str());
		VN_LOGS_WARNING(SDL_GetError());
		SDL_FreeSurface(surface);
	}

	SDL_Surface* Artist::GetScreenshot(vec2 size) {
		SDL_Surface* rsurface = SDL_CreateRGBSurface(m_GameScreen->flags, size.x, size.y, 32,
			m_GameScreen->format->Rmask, m_GameScreen->format->Gmask, 
			m_GameScreen->format->Bmask, m_GameScreen->format->Amask);
		SDL_Rect rrr = { 0,0,size.x,size.y };
		SDL_FillRect(rsurface, &rrr, SDL_MapRGBA(rsurface->format, 0, 0, 0, 255));
		SDL_BlitScaled(m_GameScreen, nullptr, rsurface, nullptr);
		return rsurface;
	}
	
	void Artist::SaveScreen() {
		SDL_FreeSurface(m_GameScreen);
		m_GameScreen = SDL_CreateRGBSurfaceWithFormat(0, m_WindowSize.x, m_WindowSize.y, 32, SDL_PIXELFORMAT_ARGB8888);
		SDL_RenderReadPixels(m_pRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, m_GameScreen->pixels, m_GameScreen->pitch);
		m_Screens.push_back({ m_Background, m_Queue, m_DrawId });
		WipeDrawing();
	}

	void Artist::PopScreen() {
		if (m_Screens.empty()) return;
		WipeDrawing();
		screenToSave screen = m_Screens.back();
		m_Background = screen.b;
		m_Queue = screen.q;
		m_DrawId = screen.id;
		m_Screens.pop_back();
	}
	
	void Artist::WipeScreens() {
		WipeDrawing();
		if (m_Screens.empty()) return;
		m_Screens.clear();
	}
	
	Artist::dump Artist::Dumb() {
		dump d = {
			TM_INSTANCE.Dump(),
			{ m_Screens.back().b.backgroundColor ,
			m_Screens.back().b.textureKey ,
			m_Screens.back().b.drawBackPic ,
			m_Screens.back().b.colorChanged ,
			m_Screens.back().b.dest ,
			m_Screens.back().b.stretchState ,
			m_Screens.back().q, m_Screens.back().id}
		};
		return d;
	}
	
	void Artist::Load(const screen& s) {
		m_Background.backgroundColor = s.backgroundColor;
		m_Background.colorChanged = s.colorChanged;
		m_Background.dest = s.dest;
		m_Background.drawBackPic = s.drawBackPic;
		m_Background.ptexture = TM_INSTANCE.getTexture(s.textureKey);
		m_Background.stretchState = s.stretchState;
		m_Background.textureKey = s.textureKey;
		m_Queue = s.q;
	}
}