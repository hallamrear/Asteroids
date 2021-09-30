#pragma once
class Settings
{
private:
	static Settings* mInstance;

	Settings();
	~Settings();

	int		 mMaxLogMessages;
	int		 mPlayerScore;
	bool	 mDrawColliders;
	int		 mAsteroidCount;
	bool	 mDrawLog;
	Vector2f mWindowCentre;
	Vector2f mWindowDimensions;
	Vector2f mGravityDirection;

public:

	static Settings* Get();
	bool GetDrawLog() const;
	void SetDrawLog(bool state);
	bool GetDrawColliders() const;
	void SetDrawColliders(bool state);
	Vector2f GetWindowCentre() const;
	Vector2f GetWindowDimensions() const;
	Vector2f GetGravityDirection() const;
	void SetGravityDirection(Vector2f direction);
	void SetWindowDimensions(Vector2f dimensions);
	void SetAsteroidCount(int count);
	int GetAsteroidCount() const;
	int GetPlayerScore() const;
	void SetPlayerScore(int score);
	void IncreasePlayerScore(int amountToIncrease);
	void SetMaxLogMessages(int count);
	int GetMaxLogMessages() const;
};

