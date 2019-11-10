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
	UpdatePhysics(deltaTime);
	std::cout << "Vel X: " << mVelocity.X << ", Vel Y: " << mVelocity.Y << std::endl;
	mRotation = (float)(180.0 / M_PI) * atan2(mVelocity.Y, mVelocity.X) + 90.0f;
}

void Ship::Draw()
{
	SDL_Rect destRect;
	destRect.w = 60;
	destRect.h = 70;
	destRect.x = (int)(mPosition.X - (destRect.w / 2));
	destRect.y = (int)(mPosition.Y - (destRect.h / 2));
	SDL_RenderCopyEx(&mRenderer, mTexture, NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);
}
