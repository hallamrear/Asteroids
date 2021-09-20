#include "PCH.h"
#include "Ship.h"
#define SHIP_SHOOT_COOLDOWN (1.0f)

Ship::Ship(SDL_Renderer& renderer, std::string texture_path, Vector2f position, float rotation, float weight, float dragCoeff, float speedCap)
	: Entity(renderer, texture_path, position, rotation, weight, dragCoeff, speedCap)
{
	mCollider = new BoundingBox(position, 32, 32);

	mMovementSpeed = 250.0f;
	mCanShoot = true;
	mShootCooldown = SHIP_SHOOT_COOLDOWN;
}

Ship::~Ship()
{
	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void Ship::Update(double deltaTime)
{
	UpdatePhysics(deltaTime);
	mRotation = (float)(180.0 / M_PI) * atan2(mVelocity.Y, mVelocity.X) + 90.0f;

	if (mCollider)
	{
		mCollider->mOrigin = mPosition;	
		mCollider->Update(deltaTime);
	}

	if(mCanShoot == false)
	{
		mShootCooldown -= static_cast<float>(deltaTime);

		if(mShootCooldown <= 0.0f)
		{
			mCanShoot = true;
			mShootCooldown = SHIP_SHOOT_COOLDOWN;
		}
	}
}

void Ship::Draw()
{
	SDL_Rect destRect;
	destRect.w = mTextureSizeX;
	destRect.h = mTextureSizeY;
	destRect.x = (int)(mPosition.X - (destRect.w / 2));
	destRect.y = (int)(mPosition.Y - (destRect.h / 2));
	SDL_RenderCopyEx(&mRenderer, mTexture, NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);

	if (mCollider)
		mCollider->Draw(mRenderer);
}

void Ship::MoveUp()
{
	AddForce(Vector2f(0.0f, -mMovementSpeed));
};

void Ship::MoveDown()
{
	AddForce(Vector2f(0.0f, mMovementSpeed));
};

void Ship::MoveLeft()
{
	AddForce(Vector2f(-mMovementSpeed, 0.0f));
};

void Ship::MoveRight()
{
	AddForce(Vector2f(mMovementSpeed, 0.0f));
};

void Ship::Shoot(std::vector<Projectile*>* projectile_vector)
{
	if(mCanShoot)
	{
		Projectile* projectile = new Projectile(mRenderer, mPosition + (mVelocity.GetNormalized() * 15), mRotation, "Assets/projectile.bmp");
		projectile->AddForce(mVelocity + (mVelocity.GetNormalized() * 2 * projectile->GetProjectileSpeed()));
		projectile_vector->push_back(projectile);

		mCanShoot = false;
	}
}