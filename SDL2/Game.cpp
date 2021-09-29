#include "PCH.h"
#include "Game.h"
#include "InputManager.h"
#include "StateDirector.h"
#include "GameStates.h"
#include "Log.h"
#include "PlayState.h"
#include "MainMenuState.h"
#include "DeathState.h"
#include "TextureCache.h"
#include "Settings.h"

double Game::DeltaTime = 0.0;
//todo : this is not good
//todo : abstract renderer
SDL_Renderer* Game::Renderer = nullptr;

Game::Game()
{
	DeltaTime = 0.0;
	mIsInitialised = false;
	mIsRunning = false;
	mWindow = nullptr;
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
	InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_Z, std::bind(&Settings::SetDrawColliders, Settings::Get(), !Settings::Get()->GetDrawColliders()));

	SetupGameStateFunctions();

	return true;
}

bool Game::InitialiseSystems()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Subsystem created.");

		Vector2f dimensions = Settings::Get()->GetWindowDimensions();

		if (InitialiseWindow("Test Window", 128, 128, (int)dimensions.X, (int)dimensions.Y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, false) == false)
			return false;

		if (TTF_Init() < 0)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "Error initializing SDL_ttf");
			Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
		}


		int w, h;
		SDL_GetWindowSize(mWindow, &w, &h);
		Settings::Get()->SetWindowDimensions(Vector2f((float)w, (float)h));

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
			
		return true;
	}
	else
		return false;
}

void Game::SetupGameStateFunctions()
{
	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_MAIN_MENU, new MainMenuState);
	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_RUNNING, new PlayState);
	StateDirector::SetupState(GameStateIdentifier::GAME_STATE_PLAYER_DEATH, new DeathState);
	StateDirector::SetState(GameStateIdentifier::GAME_STATE_MAIN_MENU);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	IMG_Quit();

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
				InputManager::Get()->SetMouseDown(true);
			}
			else if(event.type == SDL_MOUSEBUTTONUP)
			{
				InputManager::Get()->SetMouseDown(false);
			}

			break;

		case SDL_MOUSEMOTION:
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);
			InputManager::Get()->MouseUpdate(x, y);

			break;
			
		case SDL_QUIT:
			mIsRunning = false;
			break;

		case SDL_WINDOWEVENT_RESIZED:
			int w, h;
			SDL_GetWindowSize(mWindow, &w, &h);
			Settings::Get()->SetWindowDimensions(Vector2f((float)w, (float)h));
			break;

		case SDL_KEYDOWN:
		if(event.key.keysym.sym == SDLK_ESCAPE)
			mIsRunning = false;
		else
			InputManager::Get()->KeyUpdate(event.key.keysym.sym, true);
			break;

		case SDL_KEYUP:
			InputManager::Get()->KeyUpdate(event.key.keysym.sym, false);
			break;
		}
	}
}

void Game::Update()
{
	InputManager::Get()->Update();
	StateDirector::Update(DeltaTime);
}

void Game::Render()
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);
	StateDirector::Render(*Renderer);
	SDL_RenderPresent(Renderer);
}
