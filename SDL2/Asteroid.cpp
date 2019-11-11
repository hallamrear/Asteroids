#include "PCH.h"
#include "Asteroid.h"

Asteroid::Asteroid(SDL_Renderer& renderer, std::string texture_path, int size, Vector2f position, float rotation)
	: Entity(renderer, texture_path, position, rotation, 10.0f, 0.0f, 50.0f)
{
	mSize = size;
	mPhysicsEnabled = true;
	mDragEnabled = false;
}

Asteroid::~Asteroid()
{

}

void Asteroid::Update(double deltaTime)
{
	UpdatePhysics(deltaTime);
	std::cout << "Ast Vel X: " << mVelocity.X << ", Ast Vel Y: " << mVelocity.Y << std::endl;
}

void Asteroid::Draw()
{
	SDL_Rect destRect;
	destRect.w = 64;
	destRect.h = 64;
	destRect.x = (int)(mPosition.X - (destRect.w / 2));
	destRect.y = (int)(mPosition.Y - (destRect.h / 2));
	SDL_RenderCopyEx(&mRenderer, mTexture, NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);
}