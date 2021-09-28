#include "PCH.h"
#include "Projectile.h"


Projectile::Projectile(SDL_Renderer& renderer, Vector2f position, float rotation, std::string texturePath)
	: Entity(renderer, texturePath, position, rotation, 10.0f, 0.5f, 35000.0f)
{
	mProjectileSpeed = 3000.f;

	mPhysicsEnabled = true;
	mDragEnabled = false;

	mCollider = new BoundingBox(position, mTexture->Width, mTexture->Height);
}

Projectile::~Projectile()
{
	mPhysicsEnabled = false;
	mDragEnabled = false;

	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void Projectile::Update(double deltaTime)
{
	if(mIsAlive)
	{
		UpdatePhysics(deltaTime);

		if (mCollider)
		{
			mCollider->mOrigin = mPosition;
			mCollider->Update(deltaTime);
		}
	}
}

void Projectile::Render()
{
	SDL_Rect destRect{};
	destRect.w = mTexture->Width;
	destRect.h = mTexture->Height;
	destRect.x = (int)(mPosition.X) - (destRect.w / 2.0f);
	destRect.y = (int)(mPosition.Y) - (destRect.h / 2.0f);
	SDL_RenderCopyEx(&mRenderer, &mTexture->GetSDLTexture(), NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);

	if (mCollider)
		mCollider->Render(mRenderer);
}

float const Projectile::GetProjectileSpeed()
{
	return mProjectileSpeed;
}
