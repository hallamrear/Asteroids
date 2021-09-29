#pragma once
#include "GameStates.h"
#include "Collision.h"
#include "Game.h"
#include "Settings.h"
#include "MenuObject.h"

struct MainMenuState :
	GameState
{
	std::vector<MenuObject*> mMenuEntites;
	BoundingBox* mMouseCollider = nullptr;

	void Start()
	{
		mMouseCollider = new BoundingBox(Settings::Get()->GetWindowCentre(), 2, 2);

		float x = Settings::Get()->GetWindowDimensions().X / 2.0f;
		float y = Settings::Get()->GetWindowDimensions().Y / 3.0f;

		mMenuEntites.emplace_back(new MenuObject(*Game::Renderer, "Assets/title.png", Vector2f(x, y), 0.0f));
		mMenuEntites.emplace_back(new MenuObject(*Game::Renderer, "Assets/play.png", Vector2f(x, y * 2), 0.0f));
	}

	void End() override
	{
		if (mMouseCollider)
		{
			delete mMouseCollider;
			mMouseCollider = nullptr;
		}

		if (mMenuEntites.size() != 0)
		{
			for (auto& menuEntity : mMenuEntites)
			{
				delete menuEntity;
				menuEntity = nullptr;
			}

			mMenuEntites.clear();
		}
	}

	void Update(double DeltaTime) override
	{
		mMouseCollider->mOrigin = InputManager::Get()->GetMousePosition();
		mMouseCollider->Update(DeltaTime);

		for (auto* itr : mMenuEntites)
		{
			itr->Update(DeltaTime);
		}

		//todo : add supprt for explosion particles

		if (InputManager::Get()->GetMouseDown() == true)
		{
			if (Collision_Detection::CheckCollision(mMouseCollider, mMenuEntites.back()->GetCollider()))
			{
				StateDirector::SetState(GameStateIdentifier::GAME_STATE_RUNNING);
			}
		}

	}

	void Render(SDL_Renderer& renderer) override
	{
		mMouseCollider->Render(renderer);

		for (auto& itr : mMenuEntites)
		{
			itr->Render();
		}
	}
};