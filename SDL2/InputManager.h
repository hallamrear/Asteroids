#pragma once

enum class IM_KEY_CODE : int
{
	/*00*/ IM_KEY_1 = 0,
	/*01*/ IM_KEY_2,
	/*02*/ IM_KEY_3,
	/*03*/ IM_KEY_4,
	/*04*/ IM_KEY_5,
	/*05*/ IM_KEY_6,
	/*06*/ IM_KEY_7,
	/*07*/ IM_KEY_8,
	/*08*/ IM_KEY_9,
	/*09*/ IM_KEY_0,
	/*10*/ IM_KEY_UP_ARROW,
	/*11*/ IM_KEY_DOWN_ARROW,
	/*12*/ IM_KEY_LEFT_ARROW,
	/*13*/ IM_KEY_RIGHT_ARROW,
	/*14*/ IM_KEY_Q,
	/*15*/ IM_KEY_W,
	/*16*/ IM_KEY_E,
	/*17*/ IM_KEY_A,
	/*18*/ IM_KEY_S,
	/*19*/ IM_KEY_D,
	/*20*/ IM_KEY_Z,
	/*21*/ IM_KEY_X,
	/*22*/ IM_KEY_C,
	/*23*/ IM_KEY_SPACE,
};

class Key
{
private:
	std::function<void()> mFunction;
	IM_KEY_CODE mKeyCode;
	bool mState = false;

public:
	inline Key(IM_KEY_CODE keycode)
	{
		mKeyCode = keycode;
		mState = false;
		mFunction = nullptr;
	};

	inline IM_KEY_CODE	GetKeyCode()			{ return mKeyCode; };
	inline bool			GetState()				{ return mState; };
	inline void			SetState(bool state)	{ mState = state; };

	inline void Bind(std::function<void()> func)
	{
		if (func)
		{
			mFunction = func;
		}
	};

	inline void RunFunction()
	{
		if (mFunction)
			mFunction();
	};
};

class InputManager
{
private:
	const int mKeyCount = 24;
	bool mIsMouseDown;
	int mMousePositionX;
	int mMousePositionY;

	Key mKeyStates[24] =
	{
		/*00*/ IM_KEY_CODE::IM_KEY_1,
		/*01*/ IM_KEY_CODE::IM_KEY_2,
		/*02*/ IM_KEY_CODE::IM_KEY_3,
		/*03*/ IM_KEY_CODE::IM_KEY_4,
		/*04*/ IM_KEY_CODE::IM_KEY_5,
		/*05*/ IM_KEY_CODE::IM_KEY_6,
		/*06*/ IM_KEY_CODE::IM_KEY_7,
		/*07*/ IM_KEY_CODE::IM_KEY_8,
		/*08*/ IM_KEY_CODE::IM_KEY_9,
		/*09*/ IM_KEY_CODE::IM_KEY_0,
		/*10*/ IM_KEY_CODE::IM_KEY_UP_ARROW,
		/*11*/ IM_KEY_CODE::IM_KEY_DOWN_ARROW,
		/*12*/ IM_KEY_CODE::IM_KEY_LEFT_ARROW,
		/*13*/ IM_KEY_CODE::IM_KEY_RIGHT_ARROW,
		/*14*/ IM_KEY_CODE::IM_KEY_Q,
		/*15*/ IM_KEY_CODE::IM_KEY_W,
		/*16*/ IM_KEY_CODE::IM_KEY_E,
		/*17*/ IM_KEY_CODE::IM_KEY_A,
		/*18*/ IM_KEY_CODE::IM_KEY_S,
		/*19*/ IM_KEY_CODE::IM_KEY_D,
		/*20*/ IM_KEY_CODE::IM_KEY_Z,
		/*21*/ IM_KEY_CODE::IM_KEY_X,
		/*22*/ IM_KEY_CODE::IM_KEY_C,
		/*23*/ IM_KEY_CODE::IM_KEY_SPACE
	};


public:
	InputManager();
	~InputManager();

	void Update();
	int FindKey(IM_KEY_CODE keycode);
	void Bind(IM_KEY_CODE keycode, std::function<void()> func);
	void KeyUpdate(SDL_Keycode key, bool state);

	bool GetMouseDown();
	void SetMouseDown(bool state);
	void MouseUpdate(int x, int y);
	Vector2f GetMousePosition();
};

