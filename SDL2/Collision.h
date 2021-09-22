#pragma once
#include "PCH.h"

enum class COLLIDER_TYPE
{
	COLLIDER_UNKNOWN = -1,
	COLLIDER_AABB = 0,
	COLLIDER_SPHERE
};

class Collider
{
public:
	Vector2f mOrigin;
	COLLIDER_TYPE mType;

	Collider(Vector2f& origin) : mOrigin(origin), mType(COLLIDER_TYPE::COLLIDER_UNKNOWN) { }

	virtual void Update(double deltaTime) = 0;
	virtual void Render(SDL_Renderer& renderer) = 0;
};

class BoundingBox
	: public Collider
{
public:
	bool isColliding = false;

	Vector2f TopLeft;
	Vector2f BottomRight;
	Vector2f size;

	BoundingBox(Vector2f& position, int size_x, int size_y)
		: Collider(position)
	{
		mType = COLLIDER_TYPE::COLLIDER_AABB;

		size.X = size_x;
		size.Y = size_y;

		int size_half_x = size_x / 2;
		int size_half_y = size_y / 2;

		TopLeft = Vector2f(position.X - size_half_x, position.Y - size_half_y);
		BottomRight = Vector2f(position.X + size_half_x, position.Y + size_half_y);
	};

	virtual ~BoundingBox()
	{

	};

	inline void Update(double deltaTime)
	{
		TopLeft = Vector2f(mOrigin.X - (size.X / 2), mOrigin.Y - (size.Y / 2));
		BottomRight = Vector2f(mOrigin.X + (size.X / 2), mOrigin.Y + (size.Y / 2));
	}

	inline void Render(SDL_Renderer& renderer)
	{
		if(DRAW_COLLIDER)
		{
			SDL_Rect r;
			r.x = mOrigin.X - (size.X / 2);
			r.y = mOrigin.Y - (size.Y / 2);
			r.w = size.X;
			r.h = size.Y;

			if (isColliding)
				SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
			else
				SDL_SetRenderDrawColor(&renderer, 255, 255, 0, 255);

			SDL_RenderDrawRect(&renderer, &r);
		}
	};
};

class BoundingSphere
	: public Collider
{
public:
	int mRadius = 0;

	BoundingSphere(Vector2f& position, int radius)
		: Collider(position)
	{
		mType = COLLIDER_TYPE::COLLIDER_SPHERE;
		mRadius = radius;
	};

	virtual ~BoundingSphere()
	{
		
	};

	virtual void Update(double deltaTime)
	{
		
	}

	virtual void Render(SDL_Renderer& renderer)
	{
		if(DRAW_COLLIDER)
		{
			Vector2f point;
			for (double angle = 0; angle <= 2 * M_PI; angle += 0.5)
			{
				point.X = mOrigin.X + mRadius * cos(angle);
				point.Y = mOrigin.Y + mRadius * sin(angle);
				SDL_RenderDrawPoint(&renderer, point.X, point.Y);
			}
		}
	}

};

namespace Collision_Detection
{
	//Box to Box
	inline static bool CheckCollision_AABBvsAABB(BoundingBox& one, BoundingBox& two)
	{
		return ((one.TopLeft.X <= two.BottomRight.X && one.BottomRight.X >= two.TopLeft.X)
			   && 
			   (one.TopLeft.Y <= two.BottomRight.Y && one.BottomRight.Y >= two.TopLeft.Y));
	};

	//Box to Sphere
	inline static bool CheckCollision_AABBvsSPHERE(BoundingBox& one, BoundingSphere& two)
	{
		Vector2f corners[4] =
		{
			one.TopLeft,
			Vector2f(one.TopLeft.X, one.BottomRight.Y),
			one.BottomRight,
			Vector2f(one.BottomRight.X, one.TopLeft.Y)
		};

		//TODO : Finish

		//return dist <= two.mRadius * two.mRadius;
		return false;
	};

	//Sphere to Sphere
	inline static bool CheckCollision_SPHEREvsSPHERE(BoundingSphere& one, BoundingSphere& two)
	{
		double deltaXSquared = one.mOrigin.X - two.mOrigin.X; // calc. delta X
		deltaXSquared *= deltaXSquared; // square delta X

		double deltaYSquared = one.mOrigin.Y - two.mOrigin.Y; // calc. delta Y
		deltaYSquared *= deltaYSquared; // square delta Y

		// Calculate the sum of the radii, then square it
		double sumRadiiSquared = one.mRadius + two.mRadius;
		sumRadiiSquared *= sumRadiiSquared;

		if (deltaXSquared + deltaYSquared <= sumRadiiSquared)
		{
			// A and B are touching
			return true;
		}

		return false;
	};

	inline static bool CheckCollision(Collider* one, Collider* two)
	{
		if (one->mType == COLLIDER_TYPE::COLLIDER_AABB && two->mType == COLLIDER_TYPE::COLLIDER_AABB)
			return CheckCollision_AABBvsAABB(dynamic_cast<BoundingBox&>(*one), dynamic_cast<BoundingBox&>(*two));

		if (one->mType == COLLIDER_TYPE::COLLIDER_SPHERE && two->mType == COLLIDER_TYPE::COLLIDER_SPHERE)
			return CheckCollision_SPHEREvsSPHERE(dynamic_cast<BoundingSphere&>(*one), dynamic_cast<BoundingSphere&>(*two));

		if (one->mType == COLLIDER_TYPE::COLLIDER_AABB && two->mType == COLLIDER_TYPE::COLLIDER_SPHERE)
			return CheckCollision_AABBvsSPHERE(dynamic_cast<BoundingBox&>(*one), dynamic_cast<BoundingSphere&>(*two));

		if (one->mType == COLLIDER_TYPE::COLLIDER_SPHERE && two->mType == COLLIDER_TYPE::COLLIDER_AABB)
			return CheckCollision_AABBvsSPHERE(dynamic_cast<BoundingBox&>(*two), dynamic_cast<BoundingSphere&>(*one));

		throw;
	}
}