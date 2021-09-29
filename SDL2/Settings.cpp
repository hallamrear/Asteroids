#include "PCH.h"
#include "Settings.h"

Settings* Settings::mInstance = nullptr;

Settings::Settings()
{
	mWindowDimensions = Vector2f(1280.0f, 720.0f);
	mWindowCentre = Vector2f(mWindowDimensions.X / 2.0f, mWindowDimensions.Y / 2.0f);
	mDrawColliders = false;
	mAsteroidCount = 5;
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


Vector2f Settings::GetWindowCentre() const { return mWindowCentre; }
Vector2f Settings::GetWindowDimensions() const { return mWindowDimensions; }
bool Settings::GetDrawColliders() const { return mDrawColliders; }
void Settings::SetDrawColliders(bool state) { mDrawColliders = state; }
int  Settings::GetAsteroidCount() const { return mAsteroidCount; }
void Settings::SetAsteroidCount(int count) { mAsteroidCount = count; }

void Settings::SetWindowDimensions(Vector2f dimensions)
{
	mWindowDimensions = dimensions;
	mWindowCentre = Vector2f(mWindowDimensions.X / 2.0f, mWindowDimensions.Y / 2.0f);
}
