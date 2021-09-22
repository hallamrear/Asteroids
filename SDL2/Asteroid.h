#pragma once
#include "Entity.h"
class Asteroid :
	public Entity
{
private:
	int mAsteroidSize;
	int mColliderSize;
	
public:
	Asteroid(SDL_Renderer& renderer, int asteroidSize, Vector2f position = Vector2f(rand() % 1280, rand() % 720), float rotation = (rand() % 359));
	virtual ~Asteroid();

	void Update(double deltaTime);
	void Render();

	int const GetAsteroidSize();
};

