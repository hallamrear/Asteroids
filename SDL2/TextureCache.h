#pragma once
#include <unordered_map>

struct SDL_Texture;

class Texture
{
private:
	SDL_Texture* mTexture;
	//Creates the sdl texture and returns the address that will be stored in the map.
	void Create(std::string texture_path);

public:
	Texture(std::string texture_path);
	~Texture();

	int Width;
	int Height;
	SDL_Texture& GetSDLTexture();
};

class TextureCache
{
private:
	TextureCache() = default;

	static TextureCache* mInstance;
	std::unordered_map<std::string, Texture*> mTextures;

	Texture* GetTexture_Internal(const std::string& texture_path);

public:
	static TextureCache* Get();
	static Texture* GetTexture(const std::string& texture_path);

	~TextureCache();
};

