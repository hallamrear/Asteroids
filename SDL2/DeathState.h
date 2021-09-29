#pragma once
#include "Game.h"
#include "GameStates.h"
#include "MenuObject.h"

struct DeathState :
    GameState
{
	MenuObject* mDeathTextObject = nullptr;
	BoundingBox* mMouseCollider = nullptr;

	~DeathState() = default;

	void Start() override
	{
		mMouseCollider = new BoundingBox(Settings::Get()->GetWindowCentre(), 2, 2);
		mDeathTextObject = new MenuObject(*Game::Renderer, "Assets/death.png", Settings::Get()->GetWindowCentre(), 0.0f);
	}

	void End() override
	{
		if (mMouseCollider)
		{
			delete mMouseCollider;
			mMouseCollider = nullptr;
		}

		if (mDeathTextObject)
		{
			delete mDeathTextObject;
			mDeathTextObject = nullptr;
		}
	}

	void Update(double DeltaTime) override
	{
		mDeathTextObject->Update(DeltaTime);

		if (InputManager::Get()->GetMouseDown())
		{
			if (Collision_Detection::CheckCollision(mDeathTextObject->GetCollider(), mMouseCollider))
			{
				StateDirector::SetState(GameStateIdentifier::GAME_STATE_MAIN_MENU);

				//todo : add exit
				//todo : add functions to reset the ship and recreate all the asteroids
				//todo : add naturally spawning asteroids (use inverse collision with screen windows for setalive() and start them one pixel into the screen flying towards teh center
			}
		}
	}

	void Render(SDL_Renderer& renderer) override
	{
		mMouseCollider->Render(renderer);
		mDeathTextObject->Render();
	}
};

