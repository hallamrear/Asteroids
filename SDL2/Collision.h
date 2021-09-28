#pragma once
#include "PCH.h"
#include "Settings.h"

enum class COLLIDER_TYPE
{
	COLLIDER_UNKNOWN = -1,
	COLLIDER_AABB = 0,
	COLLIDER_SPHERE,
	COLLIDER_OBB
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
	Vector2f TopLeft;
	Vector2f BottomRight;
	Vector2f Size;

	BoundingBox(Vector2f position, int size_x, int size_y)
		: Collider(position)
	{
		mType = COLLIDER_TYPE::COLLIDER_AABB;

		Size.X = (float)size_x;
		Size.Y = (float)size_y;

		int size_half_x = size_x / 2;
		int size_half_y = size_y / 2;

		TopLeft = Vector2f(position.X - size_half_x, position.Y - size_half_y);
		BottomRight = Vector2f(position.X + size_half_x, position.Y + size_half_y);
	};

	virtual ~BoundingBox()
	{

	};

	virtual void Update(double deltaTime)
	{
		TopLeft = Vector2f(mOrigin.X - (Size.X / 2), mOrigin.Y - (Size.Y / 2));
		BottomRight = Vector2f(mOrigin.X + (Size.X / 2), mOrigin.Y + (Size.Y / 2));
	}

	virtual void Render(SDL_Renderer& renderer)
	{
		if(Settings::Get()->GetDrawColliders())
		{
			SDL_Rect r{};
			r.x = (int)(mOrigin.X - (Size.X / 2));
			r.y = (int)(mOrigin.Y - (Size.Y / 2));
			r.w = (int)(Size.X);
			r.h = (int)(Size.Y);

			SDL_SetRenderDrawColor(&renderer, 255, 255, 0, 255);
			SDL_RenderDrawRect(&renderer, &r);
		}
	};
};


class OrientedBoundingBox : public BoundingBox
{
protected:
	

	void CalculateRotations()
	{
		float rotationRadians = ConvertToRadians(Rotation);

		TopLeft = RotatePointAroundOrigin(Vector2f(mOrigin.X - (Size.X / 2), mOrigin.Y - (Size.Y / 2)), rotationRadians, mOrigin);
		BottomLeft = RotatePointAroundOrigin(Vector2f(mOrigin.X - (Size.X / 2), mOrigin.Y + (Size.Y / 2)), rotationRadians, mOrigin);
		TopRight = RotatePointAroundOrigin(Vector2f(mOrigin.X + (Size.X / 2), mOrigin.Y - (Size.Y / 2)), rotationRadians, mOrigin);
		BottomRight = RotatePointAroundOrigin(Vector2f(mOrigin.X + (Size.X / 2), mOrigin.Y + (Size.Y / 2)), rotationRadians, mOrigin);
	}

public:
	float Rotation;
	Vector2f TopRight;
	Vector2f BottomLeft;

	OrientedBoundingBox(Vector2f position, float rotation, int size_x, int size_y)
		: BoundingBox(position, size_x, size_y)
	{
		Rotation = rotation;
		mType = COLLIDER_TYPE::COLLIDER_OBB;
		CalculateRotations();
	}

	~OrientedBoundingBox()
	{
		Rotation = 0.0f;
	}

	virtual void Update(double deltaTime)
	{
		CalculateRotations();
	}

	virtual void Render(SDL_Renderer& renderer)
	{
		if (Settings::Get()->GetDrawColliders())
		{
			SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);

			//Todo : this is disgusting maybe make a vector2i
			//Top
			SDL_RenderDrawLine(&renderer, (int)TopLeft.X, (int)TopLeft.Y, (int)TopRight.X, (int)TopRight.Y);
			//Bot
			SDL_RenderDrawLine(&renderer, (int)BottomLeft.X, (int)BottomLeft.Y, (int)BottomRight.X, (int)BottomRight.Y);
			//Left
			SDL_RenderDrawLine(&renderer, (int)TopLeft.X, (int)TopLeft.Y, (int)BottomLeft.X, (int)BottomLeft.Y);
			//Right
			SDL_RenderDrawLine(&renderer, (int)TopRight.X, (int)TopRight.Y, (int)BottomRight.X, (int)BottomRight.Y);
		}
	}

	//Fills array with points of the box.
	void GetBoxAsPoints(Vector2f points[])
	{
		//BL;
		points[0] = BottomLeft;
		points[1] = BottomRight;
		points[2] = TopRight;
		points[3] = TopLeft;
	}
};

class BoundingSphere
	: public Collider
{
public:
	float mRadius = 0;

	BoundingSphere(Vector2f& position, float radius)
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
		if(Settings::Get()->GetDrawColliders())
		{
			Vector2f point;
			for (float angle = 0; angle <= 2 * M_PI; angle += 0.5)
			{
				point.X = mOrigin.X + mRadius * cos(angle);
				point.Y = mOrigin.Y + mRadius * sin(angle);
				SDL_RenderDrawPoint(&renderer, (int)point.X, (int)point.Y);
			}
		}
	}

};

namespace Collision_Detection
{
	bool CheckCollision(Collider* one, Collider* two);
}
