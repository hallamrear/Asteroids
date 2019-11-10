#pragma once

class Entity
{
protected:
	SDL_Renderer&	mRenderer;
	SDL_Texture*	mTexture;

	//Weight of Entity in Kg.
	float mWeight;
	//Drag coefficient (default to sphere's value of 0.47)
	float mDragCoefficient;
	//Speed cap (u/s)
	float mSpeedCap;

	Vector2f mPosition;
	float mRotation;
	Vector2f mVelocity;
	Vector2f mAcceleration;
	Vector2f mNetForce;
	Vector2f mExternalForce;

	void UpdatePhysics(double deltaTime);

public:
	Entity(SDL_Renderer& renderer, std::string texture_path, Vector2f position, float rotation, float weight, float dragCoeff, float speedCap);
	virtual ~Entity() = 0;

	void AddForce(Vector2f force);

	virtual void Update(double deltaTime) = 0;
	virtual void Draw(SDL_Renderer& renderer) = 0;

	//Getters
	inline Vector2f const GetPosition() const { return mPosition; };
	inline float	const GetRotation() const { return mRotation; };
	//Setters
	inline void SetPosition(const Vector2f position) { mPosition = position; };
	inline void SetRotation(const float rotation)	 { mRotation = rotation; };
};

