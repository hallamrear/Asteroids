#pragma once
#include "Collision.h"

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
	virtual void			Draw() = 0;

	//Getters
	inline Vector2f  const	GetPosition()			const { return mPosition; };
	inline float	 const	GetRotation()			const { return mRotation; };
	inline bool		 const	GetPhysicsEnabled()		const { return mPhysicsEnabled; };
	inline bool		 const	GetDragEnabled()		const { return mDragEnabled; };
	inline Collider* const	GetCollider()			const { return mCollider; };
	inline bool		 const	GetIsAlive()			const { return mIsAlive; };

	//Setters
	inline void		SetPosition(const Vector2f position)  { mPosition = position; };
	inline void		SetRotation(const float rotation)	  { mRotation = rotation; };
	inline void		SetPhysicsEnabled(const bool state)   { mPhysicsEnabled = state; };
	inline void		SetDragEnabled(const bool state)	  { mDragEnabled = state; };
	inline void		SetAlive(const bool state)			  { mIsAlive = state; };
};

