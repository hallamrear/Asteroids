#pragma once
#include "Collision.h"
#include "GameStateDirector.h"

class Entity
{
protected:
	int						mTextureSizeX;
	int						mTextureSizeY;
	SDL_Renderer&			mRenderer;
	SDL_Texture*			mTexture;
	Collider*				mCollider;
	bool					mIsAlive;
	bool					mPhysicsEnabled;
	bool					mDragEnabled;
	float					mMass;				//Weight of Entity (kg)
	float					mDragCoefficient;	//Drag coefficient (default to sphere's value of 0.47)
	float					mSpeedCap;			//Speed cap (u/s)
	Vector2f				mPosition;
	float					mRotation;
	Vector2f				mVelocity;
	Vector2f				mAcceleration;
	Vector2f				mNetForce;
	Vector2f				mExternalForce;
	void					UpdatePhysics(double deltaTime);
	const SDL_Renderer&		GetRendererReference();

public:
	Entity(
		SDL_Renderer& renderer,
		std::string texture_path,
		Vector2f position,
		float rotation,
		float weight,
		float dragCoeff,
		float speedCap);

	virtual					~Entity() = 0;

	void					AddForce(Vector2f force);
	void					AddForce(float X = 0.0f, float Y = 0.0f);
	void					AssignTexture(std::string texture_path);

	virtual void			Update(double deltaTime) = 0;
	virtual void			Render() = 0;

	//Getters
	Vector2f  const	GetPosition()		const { return mPosition; };
	float	  const	GetRotation()		const { return mRotation; };
	bool	  const	GetPhysicsEnabled()	const { return mPhysicsEnabled; };
	bool	  const	GetDragEnabled()	const { return mDragEnabled; };
	Collider* const	GetCollider()		const { return mCollider; };
	bool	  const	GetIsAlive()		const { return mIsAlive; };

	//Setters
	virtual void	SetPosition(const Vector2f position)  { mPosition = position; };
	virtual void	SetRotation(const float rotation)	  { mRotation = rotation; };
	virtual void	SetPhysicsEnabled(const bool state)   { mPhysicsEnabled = state; };
	virtual void	SetDragEnabled(const bool state)	  { mDragEnabled = state; };
	virtual void	SetAlive(const bool state)			  { mIsAlive = state; };
};

