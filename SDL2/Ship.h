#pragma once
#include "Entity.h"
#include "Projectile.h"
#include <vector>

class Ship :
	public Entity
{
private:
	float mMovementSpeed;
	bool  mCanShoot;
	float mShootCooldown;
	
public:
	Ship(SDL_Renderer& renderer, std::string texture_path, Vector2f position, float rotation, float weight, float dragCoeff, float speedCap);
	virtual ~Ship();

	void Update(double deltaTime);
	void Render();

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void Shoot(std::vector<Projectile*>* projectile_vector);
};