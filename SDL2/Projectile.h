#pragma once
#include "Entity.h"
class Projectile :
	public Entity
{
private:
	float mProjectileSpeed;

public:
	Projectile(SDL_Renderer& renderer, Vector2f position, float rotation, std::string texturePath);
	~Projectile();

	void Update(double deltaTime);
	void Draw();

	float const GetProjectileSpeed();
};

