#include "PCH.h"
#include "Entity.h"

Entity::Entity(SDL_Renderer& renderer, std::string texture_path, Vector2f position, float rotation, float weight, float dragCoeff, float speedCap)
	: mRenderer(renderer)
{
	mPosition = position;
	mRotation = rotation;
	mMass = weight;
	mDragCoefficient = dragCoeff;
	mSpeedCap = speedCap;

	mVelocity = Vector2f();
	mAcceleration = Vector2f();
	mNetForce = Vector2f();
	mExternalForce = Vector2f();

	if (texture_path != "")
	{
		// Load image as SDL_Surface
		SDL_Surface* surface = IMG_Load(texture_path.c_str());
		// SDL_Surface is just the raw pixels
		// Convert it to a hardware-optimzed texture so we can render it
		mTexture = SDL_CreateTextureFromSurface(&mRenderer, surface);
		// Don't need the orignal texture, release the memory
		SDL_FreeSurface(surface);

		if (mTexture == nullptr)
			std::cout << "Failed to load texture <" << texture_path << "> error : " << SDL_GetError() << std::endl;
	}
}

Entity::~Entity()
{
	if (mTexture)
		SDL_DestroyTexture(mTexture);
}

void Entity::AddForce(Vector2f force)
{
	mExternalForce += force;
}

void Entity::UpdatePhysics(double deltaTime)
{
	if (mPhysicsEnabled)
	{
		///Drag
		Vector2f dragForce;
		dragForce.X = -mDragCoefficient * mVelocity.X;
		dragForce.Y = -mDragCoefficient * mVelocity.Y;
		mNetForce += dragForce;

		///External
		//No need for gravity
		//mNetForce += (9.81f * mWeight);
		mNetForce += mExternalForce;

		///Acceleration
		mAcceleration = Vector2f(mNetForce.X / mMass, mNetForce.Y / mMass);

		///Update Position
		mVelocity += mAcceleration * (float)deltaTime;
		mPosition += mVelocity;

		///Speed Cap
		//Capping at 15u/s - X
		if (mVelocity.X > mSpeedCap)
			mVelocity.X = mSpeedCap;
		else if (mVelocity.X < -mSpeedCap)
			mVelocity.X = -mSpeedCap;
		//Capping at 15u/s - Y
		if (mVelocity.Y > mSpeedCap)
			mVelocity.Y = mSpeedCap;
		else if (mVelocity.Y < -mSpeedCap)
			mVelocity.Y = -mSpeedCap;

		mNetForce = Vector2f();
		mExternalForce = Vector2f();
	}
}