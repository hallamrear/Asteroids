#include "PCH.h"
#include "Application.h"
#include "Ship.h"

Application::Application()
{ 
	mIsInitialised = false;
}

Application::~Application()
{
	if (mIsInitialised)
		Cleanup();
}

void Application::Initialise(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystem created." << std::endl;

		if (InitialiseWindow("Test Window", 128, 128, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, false) == false)
		{
			mIsInitialised = false;
			return;
		}

		SDL_GetWindowSize(mWindow, &mWindowWidth, &mWindowHeight);

		if (InitialiseGraphics() == false)
		{
			mIsInitialised = false;
			return;
		}

		//Initialise image loader.
		int flags;
		flags |= IMG_INIT_PNG;
		flags |= IMG_INIT_JPG;
		flags |= IMG_INIT_TIF;
		flags |= IMG_INIT_WEBP;

		if (IMG_Init(flags) == 0)
		{
			mIsInitialised = false;
			return;
		}

		mShip = new Ship(*mRenderer, std::string("Assets/ship.bmp"), Vector2f(300.0f, 300.0f), 0.0f, 25.0f, 0.47f, 10.0f);
		mShip->SetPhysicsEnabled(true);


		mIsInitialised = true;
	}
	else
		mIsInitialised = false;
}

bool Application::InitialiseWindow(std::string title, int xpos, int ypos, int width, int height, Uint32 flags, bool isFullscreen)
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
		std::cout << "Window created." << std::endl;
		return true;
	}
	else
	{
		std::cout << "Window failed to create." << std::endl;
		return false;
	}
}

bool Application::InitialiseGraphics()
{
	if (!mWindow || mWindow == nullptr)
		return false;

	if (mRenderer)
		SDL_DestroyRenderer(mRenderer);

	mRenderer = SDL_CreateRenderer(mWindow, -1, 0);

	if (mRenderer)
	{
		std::cout << "Renderer created." << std::endl;
		return true;
	}
	else
	{
		std::cout << "Renderer failed to create." << std::endl;
		return false;
	}
}

void Application::Cleanup()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	IMG_Quit();

	delete mShip;
	mShip = nullptr;

	mIsInitialised = false;
}

void Application::HandleEvents(double deltaTime)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;

		case SDL_WINDOWEVENT_RESIZED:
			SDL_GetWindowSize(mWindow, &mWindowWidth, &mWindowHeight);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
			case SDLK_w:
				mKeyStates.Key_Up = true;
				break;

			case SDLK_DOWN:
			case SDLK_s:
				mKeyStates.Key_Down = true;
				break;

			case SDLK_LEFT:
			case SDLK_a:
				mKeyStates.Key_Left = true;
				break;

			case SDLK_RIGHT:
			case SDLK_d:
				mKeyStates.Key_Right = true;
				break;

			case SDLK_ESCAPE:
				mIsRunning = false;
				break;
			}
			break;



		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
			case SDLK_w:
				mKeyStates.Key_Up = false;
				break;

			case SDLK_DOWN:
			case SDLK_s:
				mKeyStates.Key_Down = false;
				break;

			case SDLK_LEFT:
			case SDLK_a:
				mKeyStates.Key_Left = false;
				break;

			case SDLK_RIGHT:
			case SDLK_d:
				mKeyStates.Key_Right = false;
				break;
			}
			break;


		}
	}
}

void Application::Update(double deltaTime)
{
	std::cout << "DT: " << std::to_string(deltaTime) << std::endl;

	if(mKeyStates.Key_Up)
		mShip->AddForce(Vector2f(0.0f, -750.0f));

	if (mKeyStates.Key_Down)
		mShip->AddForce(Vector2f(0.0f, 750.0f));

	if (mKeyStates.Key_Left)
		mShip->AddForce(Vector2f(-750.0f, 0.0f));

	if (mKeyStates.Key_Right)
		mShip->AddForce(Vector2f(750.0f, 0.0f));

	mShip->Update(deltaTime);

	///
	//Wraparound
	if (mShip->GetPosition().X > mWindowWidth)
	{
		Vector2f pos = mShip->GetPosition();
		pos.X -= mWindowWidth;
		mShip->SetPosition(pos);
	}
	else if (mShip->GetPosition().X < 0.0f)
	{
		Vector2f pos = mShip->GetPosition();
		pos.X += mWindowWidth;
		mShip->SetPosition(pos);
	}

	if (mShip->GetPosition().Y > mWindowHeight)
	{
		Vector2f pos = mShip->GetPosition();
		pos.Y -= mWindowHeight;
		mShip->SetPosition(pos);
	}
	else if (mShip->GetPosition().Y < 0.0f)
	{
		Vector2f pos = mShip->GetPosition();
		pos.Y += mWindowHeight;
		mShip->SetPosition(pos);
	}
	///
}

void Application::Draw()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	SDL_Rect outline;
	outline.x = 1;
	outline.y = 1;
	outline.w = mWindowWidth - 1;
	outline.h = mWindowHeight - 1;
	SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(mRenderer, &outline);

	mShip->Draw();

	SDL_RenderPresent(mRenderer);
}
