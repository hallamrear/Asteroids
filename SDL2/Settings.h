#pragma once
class Settings
{
private:
	static Settings* mInstance;

	Settings();
	~Settings();

	bool	mDrawColliders = false;
	int		mAsteroidCount = 4;

public:

	static Settings* Get();
	bool GetDrawColliders() const;
	void SetDrawColliders(bool state);

	void SetAsteroidCount(int count);
	int GetAsteroidCount() const;
};

