#include "PCH.h"
#include "MenuObject.h"

MenuObject::MenuObject(SDL_Renderer& renderer, std::string texture_path, Vector2f position, float rotation)
	: Entity(renderer, texture_path, position, rotation, 0.0f, 0.0f, 0.0f)
{
	mPhysicsEnabled = false;
	mDragEnabled = false;
	mCollider = new BoundingBox(position, (float)mTexture->Width, (float)mTexture->Height);
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

void MenuObject::Render()
{
	if(GetIsAlive())
	{
		SDL_Rect destRect{};
		destRect.w = mTexture->Width;
		destRect.h = mTexture->Height;
		destRect.x = (int)(mPosition.X - (destRect.w / 2.0f));
		destRect.y = (int)(mPosition.Y - (destRect.h / 2.0f));
		SDL_RenderCopyEx(&mRenderer, &mTexture->GetSDLTexture(), NULL, &destRect, mRotation, NULL, SDL_FLIP_NONE);

		if (mCollider)
			mCollider->Render(mRenderer);
	}
}
