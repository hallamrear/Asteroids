#include "PCH.h"
#include "InputManager.h"

InputManager::InputManager()
{
	mIsMouseDown = false;
	mMousePositionX = -1;
	mMousePositionY = -1;
}

InputManager::~InputManager()
{

}

void InputManager::Bind(IM_KEY_CODE keycode, std::function<void()> func)
{
	if (func)
	{
		for (int i = 0; i < mKeyCount; i++)
		{
			if (mKeyStates[i].GetKeyCode() == keycode)
			{
				mKeyStates[i].Bind(func);
				return;
			}
		}
	}
	else
		std::cout << "FAILED TO BIND FUNCTION TO KEYCODE REFERENCE <" << keycode << ">" << std::endl;
}

void InputManager::Update()
{
	for (int i = 0; i < mKeyCount; i++)
	{
		if (mKeyStates[i].GetState() == true)
		{
			mKeyStates[i].RunFunction();
		}
	}
}

int InputManager::FindKey(IM_KEY_CODE keycode)
{
	for (int i = 0; i < mKeyCount; i++)
	{
		if (mKeyStates[i].GetKeyCode() == keycode)
			return i;
	}
}

void InputManager::KeyUpdate(SDL_Keycode key, bool state)
{
	switch (key)
	{
	case SDLK_0:
		mKeyStates[FindKey(IM_KEY_0)].SetState(state);
		break;

	case SDLK_1:
		mKeyStates[FindKey(IM_KEY_1)].SetState(state);
		break;

	case SDLK_2:
		mKeyStates[FindKey(IM_KEY_2)].SetState(state);
		break;

	case SDLK_3:
		mKeyStates[FindKey(IM_KEY_3)].SetState(state);
		break;

	case SDLK_4:
		mKeyStates[FindKey(IM_KEY_4)].SetState(state);
		break;

	case SDLK_5:
		mKeyStates[FindKey(IM_KEY_5)].SetState(state);
		break;

	case SDLK_6:
		mKeyStates[FindKey(IM_KEY_6)].SetState(state);
		break;

	case SDLK_7:
		mKeyStates[FindKey(IM_KEY_7)].SetState(state);
		break;

	case SDLK_8:
		mKeyStates[FindKey(IM_KEY_8)].SetState(state);
		break;

	case SDLK_9:
		mKeyStates[FindKey(IM_KEY_9)].SetState(state);
		break;

	case SDLK_UP:
		mKeyStates[FindKey(IM_KEY_UP_ARROW)].SetState(state);
		break;

	case SDLK_DOWN:
		mKeyStates[FindKey(IM_KEY_DOWN_ARROW)].SetState(state);
		break;

	case SDLK_LEFT:
		mKeyStates[FindKey(IM_KEY_LEFT_ARROW)].SetState(state);
		break;

	case SDLK_RIGHT:
		mKeyStates[FindKey(IM_KEY_RIGHT_ARROW)].SetState(state);
		break;

	case SDLK_w:
		mKeyStates[FindKey(IM_KEY_W)].SetState(state);
		break;

	case SDLK_a:
		mKeyStates[FindKey(IM_KEY_A)].SetState(state);
		break;

	case SDLK_s:
		mKeyStates[FindKey(IM_KEY_S)].SetState(state);
		break;

	case SDLK_d:
		mKeyStates[FindKey(IM_KEY_D)].SetState(state);
		break;
	case SDLK_SPACE:
		mKeyStates[FindKey(IM_KEY_SPACE)].SetState(state);
		break;
	}
}

bool InputManager::GetMouseDown()
{
	return mIsMouseDown;
}

void InputManager::SetMouseDown(bool state)
{
	mIsMouseDown = state;
}

void InputManager::MouseUpdate(int x, int y)
{
	mMousePositionX = x;
	mMousePositionY = y;
}

Vector2f InputManager::GetMousePosition()
{
	return Vector2f((float)mMousePositionX, (float)mMousePositionY);
}
