#include "PCH.h"
#include "Ship.h"
#include "Game.h"
#include "Helper.h"
#include "SDL2_gfxPrimitives.h"

#define SHIP_SHOOT_COOLDOWN (1.0f)
#define SHIP_INVINCIBLE_COOLDOWN (3.0f)

Ship::Ship(SDL_Renderer& renderer, std::string texture_path, Vector2f position, float rotation, float weight, float dragCoeff, float speedCap)
	: Entity(renderer, texture_path, position, rotation, weight, dragCoeff, speedCap)
{
	mIsThrusting = false;
	mCollider = new OrientedBoundingBox(position, mRotation, (float)mTexture->Width, (float)mTexture->Height);
	mMovementSpeed = 200.0f;
	mRotationSpeed = 125.0f;
	mCanShoot = true;
	mShootCooldown = SHIP_SHOOT_COOLDOWN;
	mIsInvincible = false;
	mInvincibleTimer = SHIP_INVINCIBLE_COOLDOWN;
	mAltTexture = TextureCache::GetTexture("Assets/altShip.bmp");
}

Ship::~Ship()
{
	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void Ship::SetIsInvincible(bool state)
{
	mIsInvincible = state;
	mInvincibleTimer = SHIP_INVINCIBLE_COOLDOWN;
}

bool Ship::GetIsInvincible()
{
	return mIsInvincible;
}

void Ship::Update(double deltaTime)
{
	if(GetIsAlive())
	{
		if(mIsInvincible)
		{
			if (mInvincibleTimer > 0.0f)
				mInvincibleTimer -= static_cast<float>(deltaTime);
			else
			{
				mIsInvincible = false;
				mInvincibleTimer = SHIP_INVINCIBLE_COOLDOWN;
			}
		}

		UpdatePhysics(deltaTime);

		float angle = ConvertToRadians(mRotation);
		mForwardVector.X = cos(angle);
		mForwardVector.Y = sin(angle);

		if (mCollider)
		{
			if (dynamic_cast<OrientedBoundingBox*>(mCollider))
			{
				auto collider = dynamic_cast<OrientedBoundingBox*>(mCollider);
				collider->Rotation = mRotation;
				collider = nullptr;
			}

			mCollider->mOrigin = mPosition;
			mCollider->Update(deltaTime);
		}

		if (mCanShoot == false)
		{
			mShootCooldown -= static_cast<float>(deltaTime);

			if (mShootCooldown <= 0.0f)
			{
				mCanShoot = true;
				mShootCooldown = SHIP_SHOOT_COOLDOWN;
			}
		}
	}
	
}

void Ship::Render()
{
	//todo : this render code is repeated alot it could probably be a function

	SDL_Rect destRect{};

	if (mIsInvincible)
	{
		SDL_Rect rect = {
			destRect.x = (int)(mPosition.X) - (destRect.w / 2),
			destRect.y = (int)(mPosition.Y) - (destRect.h / 2),
			destRect.w = mTexture->Width,
			destRect.h = mTexture->Height
		};

		int radius = 0;
		(mTexture->Width > mTexture->Height) ? radius = mTexture->Width : radius = mTexture->Height;
		radius /= 2; radius += 8;
		aacircleRGBA(&mRenderer, (short)mPosition.X, (short)mPosition.Y, (short)radius, 0, 64, 180, 160);
	}

	destRect.x = (int)(mPosition.X) - (destRect.w / 2);
	destRect.y = (int)(mPosition.Y) - (destRect.h / 2);

	if (mIsThrusting)
	{
		destRect.w = mAltTexture->Width;
		destRect.h = mAltTexture->Height;
		SDL_RenderCopyEx(&mRenderer, &mAltTexture->GetSDLTexture(), NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);
	}
	else
	{
		destRect.w = mTexture->Width;
		destRect.h = mTexture->Height;
		SDL_RenderCopyEx(&mRenderer, &mTexture->GetSDLTexture(), NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);
	}

	if (mCollider)
		mCollider->Render(mRenderer);
}

void Ship::Reset()
{
	SetAlive(true);
	SetIsInvincible(true);
	mVelocity = Vector2f();
	mAcceleration = Vector2f();
	mRotation = 0.0f;
}

void Ship::MoveUpPressed()
{
	mIsThrusting = true;
	AddForce(mForwardVector * mMovementSpeed);
};

void Ship::MoveUpReleased()
{
	mIsThrusting = false;
};

void Ship::MoveLeft()
{
	mRotation += (-mRotationSpeed * (float)Game::DeltaTime);
};

void Ship::MoveRight()
{
	mRotation += (mRotationSpeed * (float)Game::DeltaTime);
};

void Ship::Shoot(std::vector<Projectile*>& projectile_vector)
{
	if(mCanShoot)
	{
		if(mVelocity.X != 0.0f && mVelocity.Y != 0.0f)
		{
			Projectile* projectile = new Projectile(mRenderer, mPosition + (mForwardVector.GetNormalized() * 25), 0.0f, "Assets/projectile.bmp");
			projectile->AddForce(mVelocity + (mForwardVector.GetNormalized() * projectile->GetProjectileSpeed()));
			projectile->SetAlive(true);
			projectile_vector.push_back(projectile);

			//todo : make a proper sound system
			std::string s = "Assets/Sounds/laser.wav";
			//PlaySound(s.c_str(), NULL, SND_FILENAME | SND_ASYNC);

 			mCanShoot = false;
		}	
	}
}