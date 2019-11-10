#pragma once

class Vector2f
{
public:

	float X;
	float Y;

	inline Vector2f()
	{
		X = 0.0f;
		Y = 0.0f;
	}

	inline Vector2f(float x, float y)
	{
		X = x;
		Y = y;
	}

	inline float GetMagnitude()
	{
		//Pythagoras
		return sqrtf(powf(X, 2.0) + powf(Y, 2.0));
	}

	inline Vector2f GetNormalized()
	{
		float magnitude = GetMagnitude();
		return Vector2f(X / magnitude, Y / magnitude);
	}

	//Operators

	Vector2f operator*(const float& scalar)
	{
		return Vector2f(this->X * scalar, this->Y * scalar);
	}

	Vector2f& operator+=(const Vector2f& rhs)
	{
		this->X += rhs.X;
		this->Y += rhs.Y;
		return *this;
	}

	Vector2f& operator-=(const Vector2f& rhs)
	{
		this->X -= rhs.X;
		this->Y -= rhs.Y;
		return *this;
	}

	bool operator==(const Vector2f& rhs)
	{
		if ((this->X == rhs.X) && (this->Y == rhs.Y))
			return true;
		else
			return false;
	}

	bool operator!=(const Vector2f& rhs)
	{
		if ((this->X == rhs.X) && (this->Y == rhs.Y))
			return false;
		else
			return true;
	}
};

