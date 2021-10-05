#pragma once
#include "Asteroid.h"
#include "Game.h"
#include "GameStates.h"
#include "InputManager.h"
#include "MenuObject.h"
#include "TextElement.h"
#include "Settings.h"
#include "Ship.h"
#include "StateDirector.h"

struct PlayState : GameState
{
	int						 mPlayerLives = 0;
	int						 mAsteroidCount = 0;
	Ship*					 mShip = nullptr;
	TextElement*			 mScoreText = nullptr;
	std::vector<Asteroid*>	 mAsteroids;
	std::vector<MenuObject*> mLivesImages;
	std::vector<Projectile*> mProjectiles;
	BoundingBox*			 mWindowCollider = nullptr;

	~PlayState() override
	{
		
	}

	void Start() override
	{
		Vector2f centre = Settings::Get()->GetWindowCentre();

		mPlayerLives = 3;

		for (int i = 0; i < mPlayerLives; i++)
		{
			mLivesImages.push_back(new MenuObject(*Game::Renderer, "Assets/ship.bmp", Vector2f(0.0f,25.0f), 270.0f));
			mLivesImages.back()->SetPosition(Vector2f(centre.X + ((i - 1) * 35.0f), 25.0f));
		}

		Settings::Get()->SetPlayerScore(0);

		mScoreText = new TextElement(Vector2f(), "");

		mWindowCollider = new BoundingBox(
			Settings::Get()->GetWindowCentre(),
				   Settings::Get()->GetWindowDimensions().X,
				   Settings::Get()->GetWindowDimensions().Y);

		mShip = new Ship(*Game::Renderer, std::string("Assets/ship.bmp"), centre, 0.0f, 32.0f, 1.2f, 5.0f);
		mShip->SetPhysicsEnabled(true);
		mShip->SetDragEnabled(true);

		mScoreText->SetShowing(true);

		mShip->Reset();
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
		position.Y = Settings::Get()->GetWindowDimensions().Y / 12.0f;
		mScoreText->SetPosition(position);

		mAsteroidCount = Settings::Get()->GetAsteroidCount();

		InputManager::Bind(IM_KEY_CODE::IM_KEY_W, IM_KEY_STATE::IM_KEY_HELD, std::bind(&Ship::MoveUpPressed, mShip));
		InputManager::Bind(IM_KEY_CODE::IM_KEY_W, IM_KEY_STATE::IM_KEY_RELEASED, std::bind(&Ship::MoveUpReleased, mShip));
		InputManager::Bind(IM_KEY_CODE::IM_KEY_A, IM_KEY_STATE::IM_KEY_HELD, std::bind(&Ship::MoveLeft, mShip));
		InputManager::Bind(IM_KEY_CODE::IM_KEY_D, IM_KEY_STATE::IM_KEY_HELD, std::bind(&Ship::MoveRight, mShip));
		InputManager::Bind(IM_KEY_CODE::IM_KEY_SPACE, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Ship::Shoot, mShip, mProjectiles));

	};
	
	void Update(double DeltaTime) override
	{
		for (auto& itr : mLivesImages)
			itr->Update(DeltaTime);

		mScoreText->SetString(std::to_string(Settings::Get()->GetPlayerScore()));
		mScoreText->Update(DeltaTime);

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
					Settings::Get()->IncreasePlayerScore(1000);

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
			if (mAsteroids[i]->GetIsAlive() == false)
				continue;

			if(mShip->GetIsInvincible() == false)
			{
				if (Collision_Detection::CheckCollision(mShip->GetCollider(), mAsteroids[i]->GetCollider()))
				{
					mShip->SetAlive(false);
				}				
			}			

			mAsteroids[i]->Update(DeltaTime);
			WrapScreenEntity(*mAsteroids[i]);

		}

		if (mShip->GetIsAlive() == false)
		{
			if (mPlayerLives > 0)
			{
				mPlayerLives--;
				mLivesImages[mPlayerLives]->SetAlive(false);
				mShip->SetPosition(Settings::Get()->GetWindowCentre());
				mShip->Reset();
			}
			else
			{
				StateDirector::SetState(GameStateIdentifier::GAME_STATE_PLAYER_DEATH);
				return;
			}
		}

		//Cleanup any dead entites
		CleanupDeadEntites();
	};

	void Render(SDL_Renderer& renderer) override
	{
		for (auto& itr : mLivesImages)
			itr->Render();

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