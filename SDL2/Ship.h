#pragma once
#include "Entity.h"
class Ship :
	public Entity
{
private:

public:
	Ship(SDL_Renderer& renderer, std::string texture_path, Vector2f position, float rotation, float weight, float dragCoeff, float speedCap);
	virtual ~Ship();

	void Update(double deltaTime);
	void Draw(SDL_Renderer& renderer);
};

