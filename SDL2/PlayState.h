#pragma once
#include "Asteroid.h"
#include "Game.h"
#include "GameStates.h"
#include "InputManager.h"
#include "TextElement.h"
#include "Settings.h"
#include "Ship.h"
#include "StateDirector.h"

struct PlayState : GameState
{
	TextElement*			 mScoreText = nullptr;
	int						 mAsteroidCount = 0;
	int						 mPlayerScore = 0;
	Ship*					 mShip = nullptr;
	std::vector<Asteroid*>	 mAsteroids;
	std::vector<Projectile*> mProjectiles;
	BoundingBox*			 mWindowCollider = nullptr;

	~PlayState() override
	{
		
	}

	void Start() override
	{
		mPlayerScore = 0;
		mScoreText = new TextElement(Vector2f(), "");

		mWindowCollider = new BoundingBox(
			     Settings::Get()->GetWindowCentre(),
			(int)Settings::Get()->GetWindowDimensions().X,
			(int)Settings::Get()->GetWindowDimensions().Y);

		Vector2f centre = Settings::Get()->GetWindowCentre();

		
		mShip = new Ship(*Game::Renderer, std::string("Assets/ship.bmp"), centre, 0.0f, 32.0f, 1.2f, 5.0f);
		mShip->SetPhysicsEnabled(true);
		mShip->SetDragEnabled(true);

		mPlayerScore = 0;
		mScoreText->SetShowing(true);

		mShip->Reset();
		mShip->SetAlive(true);
		mShip->SetPosition(centre);

		Vector2f val;
		for (int i = 0; i < Settings::Get()->GetAsteroidCount(); i++)
		{
			mAsteroids.push_back(
				new Asteroid(
					*Game::Renderer,
					3,
					Vector2f(
						centre.X + (float)GetRandomIntExcludingCentre(640, 640 - 200),
						centre.Y + (float)GetRandomIntExcludingCentre(640, 640 - 200)
					)));

			val.X = (float)GetRandomIntExcludingCentre(512, 256);
			val.Y = (float)GetRandomIntExcludingCentre(512, 256);

			mAsteroids.back()->AddForce(val);
		}

		Vector2f position;
		position.X = Settings::Get()->GetWindowDimensions().X / 2.0f;
		position.Y = Settings::Get()->GetWindowDimensions().Y / 4.0f;
		mScoreText->SetPosition(position);

		mAsteroidCount = Settings::Get()->GetAsteroidCount();

		InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_W, std::bind(&Ship::MoveUpPressed, mShip));
		InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_UP_ARROW, std::bind(&Ship::MoveUpPressed, mShip));
		//Add the release functions
		//InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_S, std::bind(&Ship::MoveDown, mShip));
		//InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_DOWN_ARROW, std::bind(&Ship::MoveDown, mShip));
		InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_A, std::bind(&Ship::MoveLeft, mShip));
		InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_LEFT_ARROW, std::bind(&Ship::MoveLeft, mShip));
		InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_D, std::bind(&Ship::MoveRight, mShip));
		InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_RIGHT_ARROW, std::bind(&Ship::MoveRight, mShip));
		InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_SPACE, std::bind(&Ship::Shoot, mShip, &mProjectiles));

	};
	
	void Update(double DeltaTime) override
	{
		mScoreText->SetString("Score: " + std::to_string(mPlayerScore));
		mScoreText->Update(DeltaTime);

		if (mShip->GetIsAlive() == false)
			StateDirector::SetState(GameStateIdentifier::GAME_STATE_PLAYER_DEATH);

		mShip->Update(DeltaTime);

		WrapScreenEntity(*mShip);


		for (auto& projectile : mProjectiles)
		{
			projectile->Update(DeltaTime);

			if (Collision_Detection::CheckCollision(mWindowCollider, projectile->GetCollider()) == false)
			{
				projectile->SetAlive(false);
			}

			for (int i = 0; i < mAsteroidCount; i++)
			{

				if (Collision_Detection::CheckCollision(projectile->GetCollider(), mAsteroids[i]->GetCollider()))
				{
					projectile->SetAlive(false);
					mAsteroids[i]->SetAlive(false);
					Vector2f position = mAsteroids[i]->GetPosition();
					Vector2f force;

					//todo : make a proper sound system
					std::string s = "Assets/Sounds/explode.wav";

					//PlaySound(s.c_str(), NULL, SND_FILENAME | SND_ASYNC);
					mAsteroidCount--;
					mPlayerScore += 1000;

					switch (mAsteroids[i]->GetAsteroidSize())
					{
						//Create 2 of size 2
					case 3:
						force.X = (float)GetRandomIntExcludingCentre(512, 256);
						force.Y = (float)GetRandomIntExcludingCentre(512, 256);
						mAsteroids.push_back(new Asteroid(*Game::Renderer, 2, position));
						mAsteroids.back()->AddForce(force * (float)mAsteroids[i]->GetAsteroidSize());

						force.X = (float)GetRandomIntExcludingCentre(512, 256);
						force.Y = (float)GetRandomIntExcludingCentre(512, 256);
						mAsteroids.push_back(new Asteroid(*Game::Renderer, 2, position));
						mAsteroids.back()->AddForce(force * (float)mAsteroids[i]->GetAsteroidSize());
						mAsteroidCount += 2;
						break;

						//Create 2 of size 1
					case 2:
						force.X = (float)GetRandomIntExcludingCentre(512, 256);
						force.Y = (float)GetRandomIntExcludingCentre(512, 256);
						mAsteroids.push_back(new Asteroid(*Game::Renderer, 1, position));
						mAsteroids.back()->AddForce(force * (float)mAsteroids[i]->GetAsteroidSize());

						force.X = (float)GetRandomIntExcludingCentre(512, 256);
						force.Y = (float)GetRandomIntExcludingCentre(512, 256);
						mAsteroids.push_back(new Asteroid(*Game::Renderer, 1, position));
						mAsteroids.back()->AddForce(force * (float)mAsteroids[i]->GetAsteroidSize());
						mAsteroidCount += 2;
						break;

						//Do not create any new ones
					case 1:
					default:
						break;
					}
				}
			}
		}

		for (size_t i = 0; i < mAsteroids.size(); i++)
		{
			if (Collision_Detection::CheckCollision(mShip->GetCollider(), mAsteroids[i]->GetCollider()))
			{
				mShip->SetAlive(false);
			}

			if (mAsteroids[i]->GetIsAlive() == false)
				continue;

			mAsteroids[i]->Update(DeltaTime);
			WrapScreenEntity(*mAsteroids[i]);

		}

		//Cleanup any dead entites
		CleanupDeadEntites();
	};

	void Render(SDL_Renderer& renderer) override
	{
		for (auto asteroid : mAsteroids)
			asteroid->Render();

		for (auto& projectile : mProjectiles)
			projectile->Render();

		mWindowCollider->Render(renderer);

		mScoreText->Render();
		mShip->Render();
	};

	void End() override
	{
		if(mWindowCollider)
		{
			delete mWindowCollider;
			mWindowCollider = nullptr;
		}

		//todo : Create unbind function for input manager

		//todo : destroy this
		mScoreText->SetShowing(false);



		for (size_t i = 0; i < mAsteroids.size(); i++)
		{
			delete mAsteroids[i];
			mAsteroids[i] = nullptr;
		}

		//todo : destroy projectiles
		mAsteroids.clear();

		mShip->SetAlive(false);

	}

	void WrapScreenEntity(Entity& entity)
	{
		Vector2f dimensions = Settings::Get()->GetWindowDimensions();

		if (entity.GetPosition().X > dimensions.X)
		{
			Vector2f pos = entity.GetPosition();
			pos.X -= dimensions.X;
			entity.SetPosition(pos);
		}
		else if (entity.GetPosition().X < 0.0f)
		{
			Vector2f pos = entity.GetPosition();
			pos.X += dimensions.X;
			entity.SetPosition(pos);
		}

		if (entity.GetPosition().Y > dimensions.Y)
		{
			Vector2f pos = entity.GetPosition();
			pos.Y -= dimensions.Y;
			entity.SetPosition(pos);
		}
		else if (entity.GetPosition().Y < 0.0f)
		{
			Vector2f pos = entity.GetPosition();
			pos.Y += dimensions.Y;
			entity.SetPosition(pos);
		}
	}


	void CleanupDeadEntites()
	{
		Asteroid* asteroid = nullptr;
		auto astr_itr = mAsteroids.begin();
		while (astr_itr != mAsteroids.end())
		{
			asteroid = *astr_itr._Ptr;
			if (asteroid->GetIsAlive() == false)
			{
				astr_itr = mAsteroids.erase(astr_itr);
			}
			else
				++astr_itr;
		}

		Projectile* projectile = nullptr;
		auto proj_itr = mProjectiles.begin();
		while (proj_itr != mProjectiles.end())
		{
			projectile = *proj_itr._Ptr;
			if (projectile->GetIsAlive() == false)
			{
				proj_itr = mProjectiles.erase(proj_itr);
			}
			else
				++proj_itr;
		}

		projectile = nullptr;
		asteroid = nullptr;
	}
};

