#include "Artist.h"
#include "vnepch.h"

#include "Core/Logger.h"

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
		: m_pWindow(nullptr), m_pRenderer(nullptr), m_TextureManager(nullptr),
		m_DrawId(0), m_Background({})
	{

		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			VN_LOGS_ERROR("SDL initialization error");
			return;
		}

		int flags = 0;
		if (fullscreen) flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
		else flags = SDL_WINDOW_SHOWN;

		m_pWindow = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height,
			flags
		);
		if (m_pWindow == nullptr) {
			VN_LOGS_ERROR("Window creation error");
			return;
		}

		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
		
		m_TextureManager = new TextureManager(m_pRenderer);
		m_Queue.clear();

		m_Background = {
			{0,0,0,255},
			nullptr,
			"",
			false,
			Stretching::FULLSCREENED
		};

		VN_LOGS_INFO("Window creation succes");
	}

	Artist::~Artist() {
		m_Queue.clear();
		delete m_TextureManager;
		SDL_DestroyRenderer(m_pRenderer);
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();

		VN_LOGS_INFO("Window deleting succes");
	}

	void Artist::Perform() {
		SDL_SetRenderDrawColor(m_pRenderer, m_Background.backgroundColor.r,
			m_Background.backgroundColor.g, m_Background.backgroundColor.b, m_Background.backgroundColor.a);
		SDL_RenderClear(m_pRenderer);
		
		for (const std::pair<uint32_t, DrawnData>& pair : m_Queue) {
			auto d = &(pair.second);
			SDL_RenderCopy(m_pRenderer, d->texture->sdl_texture,
				&d->source, &d->destination);
		}

		SDL_RenderPresent(m_pRenderer);
	}

	void Artist::SetBackground(vec4u8 color) {
		m_Background.backgroundColor = color;
	}

	void Artist::SetBackground(const std::string& key) {
		m_Background.ptexture = nullptr;
		m_Background.ptexture = m_TextureManager->getTexture(key);
		if (m_Background.ptexture == nullptr) { 
			VN_LOGS_WARNING("Attemp to make a non added pic a " <<
				"background '" << key <<"'");
			return; 
		}
		m_Background.textureKey = key;
	}

	void Artist::SetStretchingState(Stretching state) {
		m_Background.stretchState = state;
	}

	void Artist::SetDrawingPicture(bool picture) {
		m_Background.pictureOrColor = picture;
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
		return m_Background.pictureOrColor;
	}

	uint32_t Artist::Draw(const std::string& key, int tileNum, Rect destination) {
		FindFirstEmptyId();

		Texture* tempTexture = m_TextureManager->getTexture(key);
		if (tempTexture == nullptr) return UINT32_MAX;
		Rect tempSourceRect = GetTileRect(tempTexture, tileNum, key);
		m_Queue[m_DrawId] = {tempTexture, tempSourceRect, destination};

		return m_DrawId;
	}
	uint32_t Artist::Draw(const std::string& key, int row, int collumn, Rect destination) {
		FindFirstEmptyId();

		Texture* tempTexture = m_TextureManager->getTexture(key);
		if (tempTexture == nullptr) return UINT32_MAX;
		Rect tempSourceRect = GetTileRect(tempTexture, row, collumn, key);
		m_Queue[m_DrawId] = { tempTexture, tempSourceRect, destination };

		return m_DrawId;
	}

	void Artist::StopDrawing(const std::string& key) {
		int counter = 0;
		Texture* goalTexture = m_TextureManager->getTexture(key);
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
		m_TextureManager->addTexture(key, path, rows, collumns);
	}

	void Artist::DeleteTexture(const std::string& key) {
		m_TextureManager->delTexture(key);
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
}