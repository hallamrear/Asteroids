#pragma once
#include "Entity.h"
#include "Projectile.h"
#include <vector>

class Ship :
	public Entity
{
private:
	Texture*	 mAltTexture;
	int			 mAltTextureWidth;
	int			 mAltTextureHeight;
	float		 mMovementSpeed;
	float		 mRotationSpeed;
	bool		 mCanShoot;
	bool		 mIsThrusting;
	float		 mShootCooldown;
	Vector2f	 mForwardVector;
	
public:
	Ship(SDL_Renderer& renderer, std::string texture_path, Vector2f position, float rotation, float weight, float dragCoeff, float speedCap);
	virtual ~Ship() override;

	void Update(double deltaTime);
	void Render();

	void Reset();
	void MoveUpPressed();
	void MoveUpReleased();
	void MoveLeft();
	void MoveRight();
	void Shoot(std::vector<Projectile*>* projectile_vector);
};