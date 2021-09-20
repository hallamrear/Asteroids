#include "PCH.h"
#include "Asteroid.h"

Asteroid::Asteroid(SDL_Renderer& renderer, int asteroidSize, Vector2f position, float rotation)
	: Entity(renderer, "", position, rotation, 10.0f, 0.0f, 50.0f)
{
	mColliderSize = 0;
	mAsteroidSize = asteroidSize;
	mPhysicsEnabled = true;
	mDragEnabled = false;

	switch (mAsteroidSize)
	{
	case 1:
		AssignTexture("Assets/smallAsteroid.bmp");
		mColliderSize = 32;
		break;

	case 2:
		AssignTexture("Assets/midAsteroid.bmp");
		mColliderSize = 64;
		break;

	case 3:
		AssignTexture("Assets/largeAsteroid.bmp");
		mColliderSize = 128;
		break;

	default:
		printf("Unsupported asteroid size: %d", asteroidSize);
		break;
	}

	if(mColliderSize != 0)
		mCollider = new BoundingBox(position, mColliderSize, mColliderSize);
}

Asteroid::~Asteroid()
{
	mAsteroidSize = NULL;
	mColliderSize = NULL;
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
			mCollider->mOrigin = mPosition;
			mCollider->Update(deltaTime);
		}
	}
}

void Asteroid::Draw()
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

int const Asteroid::GetAsteroidSize()
{
	return mAsteroidSize;
}