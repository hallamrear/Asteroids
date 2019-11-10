#include "PCH.h"
#include "Ship.h"

Ship::Ship(SDL_Renderer& renderer, std::string texture_path, Vector2f position, float rotation, float weight, float dragCoeff, float speedCap)
	: Entity(renderer, texture_path, position, rotation, weight, dragCoeff, speedCap)
{

}

Ship::~Ship()
{
	
}

void Ship::Update(double deltaTime)
{
	std::cout << "Vel X: " << mVelocity.X << ", Vel Y: " << mVelocity.Y << std::endl;

	mRotation += (float)deltaTime;
	if (mRotation > 360.0f)
		mRotation -= 360.0f;

	UpdatePhysics(deltaTime);
}

void Ship::Draw(SDL_Renderer& renderer)
{
	SDL_Rect destRect;
	destRect.w = 64;
	destRect.h = 64;
	destRect.x = mPosition.X - (destRect.w / 2);
	destRect.y = mPosition.Y - (destRect.h / 2);
	SDL_RenderCopyEx(&renderer, mTexture, NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);
}
