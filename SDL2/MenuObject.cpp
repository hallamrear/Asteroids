#include "PCH.h"
#include "MenuObject.h"

MenuObject::MenuObject(SDL_Renderer& renderer, std::string texture_path, Vector2f position, float rotation)
	: Entity(renderer, texture_path, position, rotation, 0.0f, 0.0f, 0.0f)
{
	mPhysicsEnabled = false;
	mDragEnabled = false;

	mCollider = new BoundingBox(position, mTextureSizeX, mTextureSizeY);
}

MenuObject::~MenuObject()
{

}

void MenuObject::Update(double deltaTime)
{
	UpdatePhysics(deltaTime);

	if (mCollider)
	{
		mCollider->mOrigin = mPosition;
		mCollider->Update(deltaTime);
	}
}

void MenuObject::Draw()
{
	if (mCollider)
		mCollider->Draw(mRenderer);

	SDL_Rect destRect;
	destRect.w = mTextureSizeX;
	destRect.h = mTextureSizeY;
	destRect.x = (int)(mPosition.X - (destRect.w / 2));
	destRect.y = (int)(mPosition.Y - (destRect.h / 2));
	SDL_RenderCopyEx(&mRenderer, mTexture, NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);
}
