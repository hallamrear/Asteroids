#pragma once
#include <random>

//Works equally each direction from 0
//e.g. GetRandomIntExcludingCentre(256, 128)
//		can return between -256 -> -128 & 128 -> 256
inline int GetRandomIntExcludingCentre(int upperBound, int lowerBound)
{
	int value = rand() % upperBound;

	if (value < lowerBound)
		value = (upperBound - value) * -1;

	return value;
}

inline float ConvertToRadians(float degrees)
{
	return degrees * (float)(M_PI / 180.0);
}

inline float ConvertToDegrees(float radians)
{
	return radians * (float)(180.0 / M_PI);
}

//todo : make reference to store in point rather than returning
inline Vector2f RotatePointAroundOriginRadians(Vector2f point, float rotation, Vector2f origin)
{
	float s = sin(rotation);
	float c = cos(rotation);

	// translate point back to origin:
	point.X -= origin.X;
	point.Y -= origin.Y;

	// rotate point
	float xnew = point.X * c - point.Y * s;
	float ynew = point.X * s + point.Y * c;

	// translate point back:
	Vector2f value;
	value.X = xnew + origin.X;
	value.Y = ynew + origin.Y;
	return value;
}

inline Vector2f RotatePointAroundOriginDegrees(Vector2f point, float rotation, Vector2f origin)
{
	return RotatePointAroundOriginRadians(point, ConvertToRadians(rotation), origin);
}
