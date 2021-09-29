#pragma once
class Settings
{
private:
	static Settings* mInstance;

	Settings();
	~Settings();

	bool	 mDrawColliders;
	int		 mAsteroidCount;
	Vector2f mWindowCentre;
	Vector2f mWindowDimensions;

public:

	static Settings* Get();
	bool GetDrawColliders() const;
	void SetDrawColliders(bool state);
	Vector2f GetWindowCentre() const;
	Vector2f GetWindowDimensions() const;
	void SetWindowDimensions(Vector2f dimensions);
	void SetAsteroidCount(int count);
	int GetAsteroidCount() const;
};

