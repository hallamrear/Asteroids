#pragma once
#include "Game.h"
#include "GameStates.h"
#include "MenuObject.h"

struct DeathState :
    GameState
{
	BoundingBox* mWindowCollider = nullptr;
	BoundingBox* mMouseCollider = nullptr;
	TextElement* mScoreTextElement = nullptr;
	TextElement* mGenericTextDeathElement = nullptr;

	~DeathState() override = default;

	void Start() override
	{
		Vector2f centre = Settings::Get()->GetWindowCentre();

		mMouseCollider = new BoundingBox(centre, 2, 2);
		mWindowCollider = new BoundingBox(centre, Settings::Get()->GetWindowDimensions().X, Settings::Get()->GetWindowDimensions().Y);

		mScoreTextElement = new TextElement(Vector2f(centre.X, (centre.Y / 3) - 25), "Final score: " + std::to_string(Settings::Get()->GetPlayerScore()));
		mGenericTextDeathElement = new TextElement(Vector2f(centre.X, (centre.Y / 3)), "You died!", 0.0f, 24.0f);
	}

	void End() override
	{
		if (mMouseCollider)
		{
			delete mMouseCollider;
			mMouseCollider = nullptr;
		}

		if (mWindowCollider)
		{
			delete mWindowCollider;
			mWindowCollider = nullptr;
		}
	}

	void Update(double DeltaTime) override
	{
		mMouseCollider->Update(DeltaTime);
		mWindowCollider->Update(DeltaTime);
		mScoreTextElement->Update(DeltaTime);
		mGenericTextDeathElement->Update(DeltaTime);

		if (InputManager::Get()->GetMouseDown())
		{
			if (Collision_Detection::CheckCollision(mWindowCollider, mMouseCollider))
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
		mWindowCollider->Render(renderer);
		mScoreTextElement->Render();
		mGenericTextDeathElement->Render();
	}
};

