#pragma once
#include "Entity.h"
class Asteroid :
	public Entity
{
private:
	int mSize;

public:
	Asteroid(SDL_Renderer& renderer, std::string texture_path, int size, Vector2f position, float rotation);
	virtual ~Asteroid();

	void Update(double deltaTime);
	void Draw();
};

