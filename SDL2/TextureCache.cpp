#include "PCH.h"
#include "TextureCache.h"
#include <assert.h>

#include "Game.h"
#include "Log.h"

TextureCache* TextureCache::mInstance = nullptr;

TextureCache::~TextureCache()
{
	if(mTextures.empty())
	{
		for(auto& itr : mTextures)
		{
			delete itr.second;
			itr.second = nullptr;
		}

		mTextures.clear();
	}
}

Texture* TextureCache::GetTexture_Internal(const std::string& texture_path)
{
	auto itr = mTextures.find(texture_path);

	if(itr == mTextures.end())
	{
		mTextures.insert(std::make_pair(texture_path, new Texture(texture_path)));
		itr = mTextures.find(texture_path);
	}

	assert(itr->second != nullptr);

	return itr->second;
}

TextureCache* TextureCache::Get()
{
	if (mInstance)
		return mInstance;
	else
		mInstance = new TextureCache();

	return mInstance;
}

Texture* TextureCache::GetTexture(const std::string& texture_path)
{
	return Get()->GetTexture_Internal(texture_path);
}

Texture::Texture(std::string texture_path)
{
	Create(texture_path);
}

Texture::~Texture()
{
	Width = NULL;
	Height = NULL;
	if(mTexture)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;

	}
}

SDL_Texture& Texture::GetSDLTexture()
{
	return *mTexture;
}

void Texture::Create(std::string texture_path)
{
	assert(mTexture == nullptr);

	if (texture_path != "")
	{
		if (mTexture)
			SDL_DestroyTexture(mTexture);

		// Load image as SDL_Surface
		SDL_Surface* surface = IMG_Load(texture_path.c_str());
		if (surface == nullptr)
			Log::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load surface <" + texture_path + "> error : " + SDL_GetError() + "\n"));

		assert(surface != nullptr);

		// SDL_Surface is just the raw pixels
		// Convert it to a hardware-optimzed texture so we can render it
		mTexture = SDL_CreateTextureFromSurface(Game::Renderer, surface);
		if(mTexture == nullptr)
			Log::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load texture <" + texture_path + "> error : " + SDL_GetError() + "\n"));

		assert(mTexture != nullptr); 
		
		SDL_QueryTexture(mTexture, NULL, NULL, &Width, &Height);
		// Don't need the orignal texture, release the memory
		SDL_FreeSurface(surface);

		if (mTexture == nullptr)
			Log::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load texture <" + texture_path + "> error : " + SDL_GetError() + "\n"));

		assert(mTexture != nullptr);
	}
}