#pragma once
#include "GameStates.h"
#include "Settings.h"

struct Testbed : GameState
{
	BoundingBox* sphere = nullptr;
	Asteroid* asteroid = nullptr;
	TextElement* text = nullptr;

	void Start() override
	{
		Vector2f centre = Settings::Get()->GetWindowCentre();
		sphere = new BoundingBox(centre, 52.0f, 25.0f);

		asteroid = new Asteroid(*Game::Renderer, 3);
		asteroid->SetPhysicsEnabled(false);
		asteroid->SetPosition(sphere->mOrigin + Vector2f(0.0f, 150.0f));

		text = new TextElement(Vector2f(centre.X, 100.0f));

		InputManager::Bind(IM_KEY_CODE::IM_KEY_W, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveUp(); });
		InputManager::Bind(IM_KEY_CODE::IM_KEY_A, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveLeft(); });
		InputManager::Bind(IM_KEY_CODE::IM_KEY_S, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveDown(); });
		InputManager::Bind(IM_KEY_CODE::IM_KEY_D, IM_KEY_STATE::IM_KEY_HELD, [this] { MoveRight(); });
	}

	void End() override
	{
		delete sphere;
		sphere = nullptr;
	}

	void Update(double DeltaTime) override
	{
		asteroid->SetRotation(asteroid->GetRotation() + 50.0f * (float)DeltaTime);
		asteroid->Update(DeltaTime);
		sphere->Update(DeltaTime);

		if (Collision_Detection::CheckCollision(asteroid->GetCollider(), sphere))
			text->SetString("colliding");
		else
			text->SetString("not colliding");

		text->Update(DeltaTime);
	}

	void Render(SDL_Renderer& renderer) override
	{
		asteroid->Render();
		sphere->Render(renderer);

		text->Render();
	}

	void MoveUp()
	{
		sphere->mOrigin += Vector2f(0.0f, 50.0f * (float)Game::DeltaTime);
	}
	void MoveDown()
	{
		sphere->mOrigin -= Vector2f(0.0f, 50.0f * (float)Game::DeltaTime);
	}

	void MoveLeft()
	{
		sphere->mOrigin -= Vector2f(50.0f * (float)Game::DeltaTime, 0.0f);
	}

	void MoveRight()
	{
		sphere->mOrigin += Vector2f(50.0f * (float)Game::DeltaTime, 0.0f);
	}

};

