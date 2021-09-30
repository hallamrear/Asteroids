#pragma once

enum class IM_KEY_STATE : int
{
	IM_KEY_PRESSED,
	IM_KEY_HELD,
	IM_KEY_RELEASED
};

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
	std::function<void()> mPressFunction;
	std::function<void()> mHeldFunction;
	std::function<void()> mReleaseFunction;

	IM_KEY_CODE mKeyCode;
	bool mPreviousState;
	bool mState = false;

public:
	Key(IM_KEY_CODE keycode)
	{
		mKeyCode = keycode;
		mState = false;
		mPreviousState = false;
		mPressFunction = nullptr;
		mReleaseFunction = nullptr;
	};

	IM_KEY_CODE	GetKeyCode()				 { return mKeyCode; }
	bool			GetState()					 { return mState; }
	void			SetState(bool state)		 { mState = state; }
	void			SetPreviousState(bool state) { mPreviousState = state; };
	bool			GetPreviousState()			 { return mPreviousState; }

	void Bind(IM_KEY_STATE state, std::function<void()> func)
	{
		switch (state)
		{
		case IM_KEY_STATE::IM_KEY_PRESSED:
			if (func)
			{
				mPressFunction = func;
			}
			break;

		case IM_KEY_STATE::IM_KEY_HELD:
			if(func)
			{
				mHeldFunction = func;
			}
			break;

		case IM_KEY_STATE::IM_KEY_RELEASED:
			if (func)
			{
				mReleaseFunction = func;
			}
			break;
		default:
			throw;
			break;
		}		
	};

	void RunOnPressFunction()
	{
		if (mPressFunction)
			mPressFunction();
	};

	void RunOnHeldFunction()
	{
		if (mHeldFunction)
			mHeldFunction();
	}

	void RunOnReleaseFunction()
	{
		if (mReleaseFunction)
			mReleaseFunction();
	}
};

class InputManager
{
private:
	static InputManager* mInstance;

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
	void Update_Impl();
	void Bind_Impl(IM_KEY_CODE keycode, IM_KEY_STATE keystate, std::function<void()> func);

	InputManager();
	~InputManager();
public:
	static InputManager* Get();
	static void Update();

	int FindKey(IM_KEY_CODE keycode);
	static void Bind(IM_KEY_CODE keycode, IM_KEY_STATE keystate, std::function<void()> func);
	void KeyUpdate(SDL_Keycode key, bool state);

	bool GetMouseDown();
	void SetMouseDown(bool state);
	void MouseUpdate(int x, int y);
	Vector2f GetMousePosition();
};

