#pragma once
#include "Entity.h"
class Asteroid :
	public Entity
{
private:
	int mAsteroidSize;
	
public:
	Asteroid(SDL_Renderer& renderer, int asteroidSize, Vector2f position = Vector2f((float)(rand() % 1280), (float)(rand() % 720)), float rotation = (float)(rand() % 359));
	virtual ~Asteroid();

	void Update(double deltaTime);
	void Render();

	int const GetAsteroidSize();
};

