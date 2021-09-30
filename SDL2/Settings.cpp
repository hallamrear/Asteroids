#include "PCH.h"
#include "Settings.h"

Settings* Settings::mInstance = nullptr;

Settings::Settings()
{
	mMaxLogMessages = 10;
	mDrawLog = false;
	mPlayerScore = 0;
	mWindowDimensions = Vector2f(1280.0f, 720.0f);
	mWindowCentre = Vector2f(mWindowDimensions.X / 2.0f, mWindowDimensions.Y / 2.0f);
	mDrawColliders = false;
	mAsteroidCount = 5;
	mGravityDirection = Vector2f(0.0f, 9.81f);
}

Settings::~Settings()
{
	mDrawColliders = false;
	mAsteroidCount = 0;
}

Settings* Settings::Get()
{
	if (!mInstance)
		mInstance = new Settings();

	return mInstance;
}

bool Settings::GetDrawLog() const
{
	return mDrawLog;
}

void Settings::SetDrawLog(bool state)
{
	mDrawLog = state;
}

Vector2f Settings::GetWindowCentre() const
{
	return mWindowCentre;
}

Vector2f Settings::GetWindowDimensions() const
{
	return mWindowDimensions;
}

bool Settings::GetDrawColliders() const
{
	return mDrawColliders;
}

void Settings::SetDrawColliders(bool state)
{
	mDrawColliders = state;
}

int  Settings::GetAsteroidCount() const
{
	return mAsteroidCount;
}

Vector2f Settings::GetGravityDirection() const
{
	return mGravityDirection;
}

void Settings::SetGravityDirection(Vector2f direction)
{
	mGravityDirection = direction;
}

int Settings::GetPlayerScore() const
{
	return mPlayerScore;
}

void Settings::SetPlayerScore(int score)
{
	mPlayerScore = score;
}

void Settings::IncreasePlayerScore(int amountToIncrease)
{
	mPlayerScore += amountToIncrease;
}

void Settings::SetMaxLogMessages(int count)
{
	mMaxLogMessages = count;
}

int Settings::GetMaxLogMessages() const
{
	return mMaxLogMessages;
}

void Settings::SetAsteroidCount(int count)
{
	mAsteroidCount = count;
}

void Settings::SetWindowDimensions(Vector2f dimensions)
{
	mWindowDimensions = dimensions;
	mWindowCentre = Vector2f(mWindowDimensions.X / 2.0f, mWindowDimensions.Y / 2.0f);
}
