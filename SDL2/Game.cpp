#include "PCH.h"
#include "Game.h"
#include "InputManager.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Collision.h"
#include "Projectile.h"
#include "GameStateDirector.h"
#include "MenuObject.h"
#include "GameStates.h"
#include "Log.h"
#include "TextElement.h"
#include "TextureCache.h"
#include "Settings.h"

double Game::DeltaTime = 0.0;
//todo : this is not good
SDL_Renderer* Game::Renderer = nullptr;

Game::Game()
{
	mPlayerScore = 0;
	testText = nullptr;
	mMouseCollider = nullptr;
	testDeathEntity = nullptr;
	DeltaTime = 0.0;
	mWindowWidth = 0;
	mWindowHeight = 0;
	mIsInitialised = false;
	mIsRunning = false;
	mWindow = nullptr;
	mInputManager = nullptr;
	mStateDirector = nullptr;
	mShip = nullptr;
	mWindowCollider = nullptr;
}

Game::~Game()
{
	if (mIsInitialised)
		Shutdown();
}

void Game::Initialise(int argc, char* argv[])
{
	mIsInitialised = (InitialiseSystems() && InitialiseWorldObjects());

}

bool Game::InitialiseWindow(std::string title, int xpos, int ypos, int width, int height, Uint32 flags, bool isFullscreen)
{
	if (mWindow)
		SDL_DestroyWindow(mWindow);
		
	if (title == "")
		title = "No Window Title Given";

	if (xpos == 0)
		xpos = SDL_WINDOWPOS_CENTERED;

	if (ypos == 0)
		ypos = SDL_WINDOWPOS_CENTERED;

	if (width == 0)
		width = 800;

	if (height == 0)
		height = 600;

	if (isFullscreen)
		flags |= SDL_WINDOW_FULLSCREEN;

	mWindow = SDL_CreateWindow(&title[0], xpos, ypos, width, height, flags);

	if (mWindow)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Window created.");
		return true;
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Window failed to create.");
		return false;
	}
}

bool Game::InitialiseGraphics()
{
	if (!mWindow || mWindow == nullptr)
		return false;

	if (Renderer)
		SDL_DestroyRenderer(Renderer);

	Renderer = SDL_CreateRenderer(mWindow, -1, 0);

	if (Renderer)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Renderer created.");
		return true;
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Renderer failed to create.");
		return false;
	}
}

bool Game::InitialiseWorldObjects()
{
	mWindowCentre = Vector2f(mWindowWidth / 2.0f, mWindowHeight / 2.0f);
	mWindowCollider = new BoundingBox(mWindowCentre, mWindowWidth, mWindowHeight);
	mMouseCollider = new BoundingBox(mWindowCentre, 8, 8);

	mShip = new Ship(*Renderer, std::string("Assets/ship.bmp"), mWindowCentre, 0.0f, 32.0f, 1.2f, 5.0f);
	mShip->SetPhysicsEnabled(true);
	mShip->SetDragEnabled(true);

	mInputManager->Bind(IM_KEY_CODE::IM_KEY_W, std::bind(&Ship::MoveUpPressed, mShip));
	mInputManager->Bind(IM_KEY_CODE::IM_KEY_UP_ARROW, std::bind(&Ship::MoveUpPressed, mShip));
	//Add the release functions
	//mInputManager->Bind(IM_KEY_CODE::IM_KEY_S, std::bind(&Ship::MoveDown, mShip));
	//mInputManager->Bind(IM_KEY_CODE::IM_KEY_DOWN_ARROW, std::bind(&Ship::MoveDown, mShip));
	mInputManager->Bind(IM_KEY_CODE::IM_KEY_A, std::bind(&Ship::MoveLeft, mShip));
	mInputManager->Bind(IM_KEY_CODE::IM_KEY_LEFT_ARROW, std::bind(&Ship::MoveLeft, mShip));
	mInputManager->Bind(IM_KEY_CODE::IM_KEY_D, std::bind(&Ship::MoveRight, mShip));
	mInputManager->Bind(IM_KEY_CODE::IM_KEY_RIGHT_ARROW, std::bind(&Ship::MoveRight, mShip));
	mInputManager->Bind(IM_KEY_CODE::IM_KEY_SPACE, std::bind(&Ship::Shoot, mShip, &testProjectiles));
	mInputManager->Bind(IM_KEY_CODE::IM_KEY_Z, std::bind(&Settings::SetDrawColliders, Settings::Get(), !Settings::Get()->GetDrawColliders()));

	testText = new TextElement(Vector2f(100.0f, 100.0f), "DT: ");

	SetupGameStateFunctions();

	return true;
}

bool Game::InitialiseSystems()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Subsystem created.");

		if (InitialiseWindow("Test Window", 128, 128, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, false) == false)
			return false;

		if (TTF_Init() < 0)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "Error initializing SDL_ttf");
			Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
		}


		SDL_GetWindowSize(mWindow, &mWindowWidth, &mWindowHeight);

		if (InitialiseGraphics() == false)
			return false;

		//Initialise image loader.
		int flags = 0;
		flags |= IMG_INIT_PNG;
		flags |= IMG_INIT_JPG;
		flags |= IMG_INIT_TIF;
		flags |= IMG_INIT_WEBP;

		if (IMG_Init(flags) == 0)
		{
			mIsInitialised = false;
			return false;
		}

		mInputManager = new InputManager();
		
		if (!mInputManager)
			return false;


		mStateDirector = new GameStateDirector();
		if (!mStateDirector)
			return false;

		return true;
	}
	else
		return false;
}

void Game::SetupGameStateFunctions()
{
	mStateDirector->SetupState(
		new DirectorState(
			GameStateIdentifier::GAME_STATE_MAIN_MENU,
			std::bind(&Game::MenuState_Start, this),
			std::bind(&Game::MenuState_Update, this),
			std::bind(&Game::MenuState_Render, this),
			std::bind(&Game::MenuState_End, this)
	));

	mStateDirector->SetupState(
		new DirectorState(
			GameStateIdentifier::GAME_STATE_RUNNING,
			std::bind(&Game::PlayState_Start, this),
			std::bind(&Game::PlayState_Update, this),
			std::bind(&Game::PlayState_Render, this),
			std::bind(&Game::PlayState_End, this)
	));


	mStateDirector->SetupState(
		new DirectorState(
			GameStateIdentifier::GAME_STATE_PLAYER_DEATH,
			std::bind(&Game::DeathState_Start, this),
			std::bind(&Game::DeathState_Update, this),
			std::bind(&Game::DeathState_Render, this),
				std::bind(&Game::DeathState_End, this)
		));

	//todo : remove
	mStateDirector->SetupState(
		new DirectorState(
			GameStateIdentifier::GAME_STATE_TESTBED,
			std::bind(&Game::TestState_Start, this),
			std::bind(&Game::TestState_Update, this),
			std::bind(&Game::TestState_Render, this),
			std::bind(&Game::TestState_End, this)
		));

	mStateDirector->SetState(GameStateIdentifier::GAME_STATE_TESTBED);
	mStateDirector->SetState(GameStateIdentifier::GAME_STATE_MAIN_MENU);
}

void Game::TestState_Start()
{
	mShip->SetAlive(true);
	mShip->SetPosition(Vector2f(650.0f, 410.0f));

	testAsteroids.push_back(
		new Asteroid(
			*Renderer,
			3,
			Vector2f(
				mWindowCentre.X + (float)GetRandomIntExcludingCentre(640, 640 - 100),
				mWindowCentre.X + (float)GetRandomIntExcludingCentre(640, 640 - 100)
			)));
	testAsteroids[0]->SetPhysicsEnabled(false);
	testAsteroids[0]->SetPosition(mWindowCentre + Vector2f(100.0f, 100.0f));
}

void Game::TestState_Update()
{
	for (auto& itr : testProjectiles)
		itr->Update(DeltaTime);

	for(auto& itr : testAsteroids)
	{
		WrapScreenEntity(*itr);
		itr->Update(DeltaTime);
		itr->SetRotation(testAsteroids[0]->GetRotation() + 1);
	}

	WrapScreenEntity(*mShip);
	mShip->Update(DeltaTime);
}

void Game::TestState_Render()
{
	for (auto& itr : testProjectiles)
		itr->Render();

	for (auto& itr : testAsteroids)
		itr->Render();

	mShip->Render();

	if (Collision_Detection::CheckCollision(mShip->GetCollider(), testAsteroids[0]->GetCollider()))
	{
		testText->SetString("Colliding");
		testText->SetShowing(true);
	}
	else
		testText->SetShowing(false);
}

void Game::TestState_End()
{
	
}

void Game::CleanupDeadEntites()
{
	Asteroid* asteroid = nullptr;
	auto astr_proj = testAsteroids.begin();
	while (astr_proj != testAsteroids.end())
	{
		asteroid = *astr_proj._Ptr;
		if (asteroid->GetIsAlive() == false)
		{
			astr_proj = testAsteroids.erase(astr_proj);
		}
		else 
			++astr_proj;
	}

	Projectile* projectile = nullptr;
	auto proj_itr = testProjectiles.begin();
	while (proj_itr != testProjectiles.end())
	{
		projectile = *proj_itr._Ptr;
		if (projectile->GetIsAlive() == false)
		{
			proj_itr = testProjectiles.erase(proj_itr);
		}
		else
			++proj_itr;
	}

	projectile = nullptr;
	asteroid = nullptr;
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	IMG_Quit();

	delete mShip;
	mShip = nullptr;

	mIsInitialised = false;
}

void Game::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:

			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				mInputManager->SetMouseDown(true);
			}
			else if(event.type == SDL_MOUSEBUTTONUP)
			{
				mInputManager->SetMouseDown(false);
			}

			break;

		case SDL_MOUSEMOTION:
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);
			mInputManager->MouseUpdate(x, y);

			break;
			
		case SDL_QUIT:
			mIsRunning = false;
			break;

		case SDL_WINDOWEVENT_RESIZED:
			SDL_GetWindowSize(mWindow, &mWindowWidth, &mWindowHeight);
			break;

		case SDL_KEYDOWN:
		if(event.key.keysym.sym == SDLK_ESCAPE)
			mIsRunning = false;
		else
			mInputManager->KeyUpdate(event.key.keysym.sym, true);
			break;

		case SDL_KEYUP:
			mInputManager->KeyUpdate(event.key.keysym.sym, false);
			break;
		}
	}
}

void Game::WrapScreenEntity(Entity& entity)
{
	if (entity.GetPosition().X > (float)mWindowWidth)
	{
		Vector2f pos = entity.GetPosition();
		pos.X -= (float)mWindowWidth;
		entity.SetPosition(pos);
	}
	else if (entity.GetPosition().X < 0.0f)
	{
		Vector2f pos = entity.GetPosition();
		pos.X += mWindowWidth;
		entity.SetPosition(pos);
	}

	if (entity.GetPosition().Y > mWindowHeight)
	{
		Vector2f pos = entity.GetPosition();
		pos.Y -= mWindowHeight;
		entity.SetPosition(pos);
	}
	else if (entity.GetPosition().Y < 0.0f)
	{
		Vector2f pos = entity.GetPosition();
		pos.Y += mWindowHeight;
		entity.SetPosition(pos);
	}
}

void Game::MenuState_Start()
{
	float x = (float)(mWindowWidth / 2);
	float y = (float)(mWindowHeight / 3);

	menuEntities.emplace_back(new MenuObject(*Renderer, "Assets/title.png", Vector2f(x, y), 0.0f));
	menuEntities.emplace_back(new MenuObject(*Renderer, "Assets/play.png", Vector2f(x, y * 2), 0.0f));
}

void Game::MenuState_End()
{
	if(menuEntities.size() != 0)
	{
		for (auto& menuEntity : menuEntities)
		{
			delete menuEntity;
			menuEntity = nullptr;
		}

		menuEntities.clear();
	}
}

void Game::MenuState_Update()
{
	for (auto* itr : menuEntities)
	{
		itr->Update(DeltaTime);				
	}

	//todo : add supprt for explosion particles

	if (mInputManager->GetMouseDown() == true)
	{
		if (Collision_Detection::CheckCollision(mMouseCollider, menuEntities.back()->GetCollider()))
		{
			mStateDirector->SetState(GameStateIdentifier::GAME_STATE_RUNNING);
		}
	}
	
}

void Game::MenuState_Render()
{
	for(auto& itr : menuEntities)
	{
		itr->Render();
	}
}

void Game::DeathState_Start()
{
	testDeathEntity = new MenuObject(*Renderer, "Assets/death.png", mWindowCentre, 0.0f);
}

void Game::DeathState_End()
{
	testText->SetShowing(false);

	for (size_t i = 0; i < testAsteroids.size(); i++)
	{
		delete testAsteroids[i];
		testAsteroids[i] = nullptr;
	}

	testAsteroids.clear();

	mShip->SetAlive(false);

	if(testDeathEntity)
	{
		delete testDeathEntity;
		testDeathEntity = nullptr;
	}
}

void Game::DeathState_Update()
{
	testDeathEntity->Update(DeltaTime);

	if(mInputManager->GetMouseDown())
	{
		if (Collision_Detection::CheckCollision(testDeathEntity->GetCollider(), mMouseCollider))
		{
			mShip->SetAlive(true);
			mStateDirector->SetState(GameStateIdentifier::GAME_STATE_MAIN_MENU);

			//todo : add exit
			//todo : add functions to reset the ship and recreate all the asteroids
			//todo : add naturally spawning asteroids (use inverse collision with screen windows for setalive() and start them one pixel into the screen flying towards teh center
		}
	}
}

void Game::DeathState_Render()
{
	PlayState_Render();
	testDeathEntity->Render();
}

void Game::PlayState_Start()
{
	mPlayerScore = 0;
	mUIElements[0]->SetShowing(true);

	mShip->Reset();
	mShip->SetAlive(true);
	mShip->SetPosition(mWindowCentre);

	Vector2f val;
	for (int i = 0; i < Settings::Get()->GetAsteroidCount(); i++)
	{
		testAsteroids.push_back(
			new Asteroid(
				*Renderer,
				3, 
				Vector2f(
					mWindowCentre.X + (float)GetRandomIntExcludingCentre(640, 640 - 200),
					mWindowCentre.X + (float)GetRandomIntExcludingCentre(640, 640 - 200)
				)));

		val.X = (float)GetRandomIntExcludingCentre(512, 256);
		val.Y = (float)GetRandomIntExcludingCentre(512, 256);

		testAsteroids.back()->AddForce(val);
	}

	Vector2f position;
	position.X = (float)mWindowWidth / 2.0f;
	position.Y = (float)mWindowHeight / 4.0f;
	testText->SetPosition(position);
}

void Game::PlayState_End()
{
	

}

void Game::PlayState_Update()
{
	testText->SetString("Score: " + std::to_string(mPlayerScore));
	testText->Update(DeltaTime);

	if (mShip->GetIsAlive() == false)
		mStateDirector->SetState(GameStateIdentifier::GAME_STATE_PLAYER_DEATH);

	mShip->Update(DeltaTime);

	WrapScreenEntity(*mShip);

	int asteroidCount = testAsteroids.size();

	for (auto& projectile : testProjectiles)
	{
		projectile->Update(DeltaTime);

		if (Collision_Detection::CheckCollision(mWindowCollider, projectile->GetCollider()) == false)
		{
			projectile->SetAlive(false);
		}

		//todo : fix it killing the player by accident
		//if (Collision_Detection::CheckCollision(mShip->GetCollider(), projectile->GetCollider()))
		//{
		//	mShip->SetAlive(false);
		//}

		for (int i = 0; i < asteroidCount; i++)
		{
			
			if (Collision_Detection::CheckCollision(projectile->GetCollider(), testAsteroids[i]->GetCollider()))
			{
				projectile->SetAlive(false);
				testAsteroids[i]->SetAlive(false);
				Vector2f position = testAsteroids[i]->GetPosition();
				Vector2f force;

				//todo : make a proper sound system
				std::string s = "Assets/Sounds/explode.wav";

				//PlaySound(s.c_str(), NULL, SND_FILENAME | SND_ASYNC);

				mPlayerScore += 1000;

				switch (testAsteroids[i]->GetAsteroidSize())
				{
				//Create 2 of size 2
				case 3:
					force.X = (float)GetRandomIntExcludingCentre(512, 256);
					force.Y = (float)GetRandomIntExcludingCentre(512, 256);
					testAsteroids.push_back(new Asteroid(*Renderer, 2, position));
					testAsteroids.back()->AddForce(force * (float)testAsteroids[i]->GetAsteroidSize());

					force.X = (float)GetRandomIntExcludingCentre(512, 256);
					force.Y = (float)GetRandomIntExcludingCentre(512, 256);
					testAsteroids.push_back(new Asteroid(*Renderer, 2, position));
					testAsteroids.back()->AddForce(force * (float)testAsteroids[i]->GetAsteroidSize());

					break;

				//Create 2 of size 1
				case 2:
					force.X = (float)GetRandomIntExcludingCentre(512, 256);
					force.Y = (float)GetRandomIntExcludingCentre(512, 256);
					testAsteroids.push_back(new Asteroid(*Renderer, 1, position));
					testAsteroids.back()->AddForce(force * (float)testAsteroids[i]->GetAsteroidSize());

					force.X = (float)GetRandomIntExcludingCentre(512, 256);
					force.Y = (float)GetRandomIntExcludingCentre(512, 256);
					testAsteroids.push_back(new Asteroid(*Renderer, 1, position));
					testAsteroids.back()->AddForce(force * (float)testAsteroids[i]->GetAsteroidSize());
					break;

				//Do not create any new ones
				case 1:
				default:
					break;
				}
			}
		}
	}

	for (size_t i = 0; i < testAsteroids.size(); i++)
	{
		if (Collision_Detection::CheckCollision(mShip->GetCollider(), testAsteroids[i]->GetCollider()))
		{
			mShip->SetAlive(false);
		}

		if (testAsteroids[i]->GetIsAlive() == false)
			continue;

		if (testAsteroids[i]->GetPosition().X >= (float)mWindowWidth)
		{
			Vector2f pos = testAsteroids[i]->GetPosition();
			pos.X -= (float)mWindowWidth;
			testAsteroids[i]->SetPosition(pos);
		}
		else if (testAsteroids[i]->GetPosition().X <= 0.0f)
		{
			Vector2f pos = testAsteroids[i]->GetPosition();
			pos.X += mWindowWidth;
			testAsteroids[i]->SetPosition(pos);
		}

		if (testAsteroids[i]->GetPosition().Y >= (float)mWindowHeight)
		{
			Vector2f pos = testAsteroids[i]->GetPosition();
			pos.Y -= mWindowHeight;
			testAsteroids[i]->SetPosition(pos);
		}
		else if (testAsteroids[i]->GetPosition().Y <= 0.0f)
		{
			Vector2f pos = testAsteroids[i]->GetPosition();
			pos.Y += mWindowHeight;
			testAsteroids[i]->SetPosition(pos);
		}


		testAsteroids[i]->Update(DeltaTime);
	}

	//Cleanup any dead entites
	CleanupDeadEntites();
}

void Game::PlayState_Render()
{
	for (auto asteroid : testAsteroids)
		asteroid->Render();

	for (auto& projectile : testProjectiles)
		projectile->Render();

	mWindowCollider->Render(*Renderer);

	mShip->Render();
}

void Game::Update()
{
	mInputManager->Update();
	mMouseCollider->mOrigin = mInputManager->GetMousePosition();
	mMouseCollider->Update(DeltaTime);
	mStateDirector->Update(DeltaTime);
}

void Game::Render()
{

	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);

	mMouseCollider->Render(*Renderer);
	mStateDirector->Render(*Renderer);
	testText->Render();

	SDL_RenderPresent(Renderer);
}
