#include "PCH.h"
#include "Settings.h"

Settings* Settings::mInstance = nullptr;

Settings::Settings()
{
	mDrawColliders = false;
	mAsteroidCount = 0;
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


bool Settings::GetDrawColliders() const { return mDrawColliders; }
void Settings::SetDrawColliders(bool state) { mDrawColliders = state; }
int  Settings::GetAsteroidCount() const { return mAsteroidCount; }
void Settings::SetAsteroidCount(int count) { mAsteroidCount = count; }