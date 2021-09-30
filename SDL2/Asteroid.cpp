#include "PCH.h"
#include "Asteroid.h"


Asteroid::Asteroid(SDL_Renderer& renderer, int asteroidSize, Vector2f position, float rotation)
	: Entity(renderer, "", position, rotation, 10.0f, 0.0f, 50.0f)
{
	mAsteroidSize = asteroidSize;
	mPhysicsEnabled = true;
	mDragEnabled = false;

	float colliderRadius = -1;

	switch (mAsteroidSize)
	{
	case 1:
		AssignTexture("Assets/smallAsteroid.bmp");
		colliderRadius = 16.0f;
		break;

	case 2:
		AssignTexture("Assets/midAsteroid.bmp");
		colliderRadius = 32.0f;
		break;

	case 3:
		AssignTexture("Assets/largeAsteroid.bmp");
		colliderRadius = 64.0f;
		break;

	default:
		printf("Unsupported asteroid size: %d", asteroidSize);
		throw;
		break;
	}

	if (mTexture != nullptr)
		mCollider = new BoundingSphere(position, colliderRadius);
}

Asteroid::~Asteroid()
{
	mAsteroidSize = NULL;
	mPhysicsEnabled = false;
	mDragEnabled = false;

	if(mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void Asteroid::Update(double deltaTime)
{
	if(mIsAlive)
	{
		UpdatePhysics(deltaTime);

		if (mCollider)
		{
			if(dynamic_cast<OrientedBoundingBox*>(mCollider))
			{
				auto collider = dynamic_cast<OrientedBoundingBox*>(mCollider);
				collider->Rotation = mRotation;
				collider = nullptr;
			}

			mCollider->mOrigin = mPosition;
			mCollider->Update(deltaTime);
		}
	}
}

void Asteroid::Render()
{
	assert(mTexture != nullptr);

	SDL_Rect destRect{};
	destRect.w = mTexture->Width;
	destRect.h = mTexture->Height;
	destRect.x = (int)(mPosition.X) - (destRect.w / 2);
	destRect.y = (int)(mPosition.Y) - (destRect.h / 2);
	SDL_RenderCopyEx(&mRenderer, &mTexture->GetSDLTexture(), NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);

	if (mCollider)
		mCollider->Render(mRenderer);
}

int const Asteroid::GetAsteroidSize()
{
	return mAsteroidSize;
}