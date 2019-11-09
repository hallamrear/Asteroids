#include "PCH.h"
#include "Application.h"

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

		if (InitialiseWindow("Test Window", 128, 128, 1280, 720, SDL_WINDOW_SHOWN, false) == false)
		{
			mIsInitialised = false;
			return;
		}

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
	
		// Load image as SDL_Surface
		SDL_Surface* surface = IMG_Load("Assets/ship.bmp");
		// SDL_Surface is just the raw pixels
		// Convert it to a hardware-optimzed texture so we can render it
		mTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
		// Don't need the orignal texture, release the memory
		SDL_FreeSurface(surface);

		if (mTexture == nullptr)
			std::cout << "Failed to load texture " << "'Assets/ship.bmp'" << " error : " << SDL_GetError() << std::endl;



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

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				mIsRunning = false;
				break;
			case SDLK_UP:
			case SDLK_w:
				mShipVelocity.y -= 2.0f;
				break;
			case SDLK_DOWN:
			case SDLK_s:
				mShipVelocity.y += 2.0f;
				break;
			case SDLK_LEFT:
			case SDLK_a:
				mShipVelocity.x -= 2.0f;
				break;
			case SDLK_d:
			case SDLK_RIGHT:
				mShipVelocity.x += 2.0f;
				break;
			}
			break;
		}
	}
}

void Application::Update(double deltaTime)
{
	std::cout << "DT: " << std::to_string(deltaTime) << std::endl;

	pos.x += mShipVelocity.x;
	pos.y += mShipVelocity.y;

	//Capping at 15u/s - X
	if (mShipVelocity.x > 30.0f)
		mShipVelocity.x = 30.0f;
	else if (mShipVelocity.x < -30.0f)
		mShipVelocity.x = -30.0f;

	//Friction of 0.1u/s
	if (mShipVelocity.x > 0.1f)
		mShipVelocity.x -= 0.1f;
	else if(mShipVelocity.x < -0.1f)
		mShipVelocity.x += 0.1f;
	else
		mShipVelocity.x = 0.0f;


	//Capping at 15u/s - Y
	if (mShipVelocity.y > 30.0f)
		mShipVelocity.y = 30.0f;
	else if (mShipVelocity.y < -30.0f)
		mShipVelocity.y = -30.0f;

	//Friction of 0.1u/s
	if (mShipVelocity.y > 0.1f)
		mShipVelocity.y -= 0.1f;
	else if (mShipVelocity.y < -0.1f)
		mShipVelocity.y += 0.1f;
	else
		mShipVelocity.y = 0.0f;

	///
	//Wraparound
	if (pos.x > 1280.0f)
		pos.x -= 1280.0f;
	else if (pos.x < 0.0f)
		pos.x += 1280.0f;

	if (pos.y > 720)
		pos.y -= 720;
	else if (pos.y < 0.0f)
		pos.y += 720;
	///

	std::cout << "VelX: " << mShipVelocity.x << ", VelY: " << mShipVelocity.y << std::endl;
}

void Application::Draw()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawPoint(mRenderer, 500, 500);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(mRenderer, 320, 200, 300, 240);
	SDL_RenderDrawLine(mRenderer, 300, 240, 340, 240);
	SDL_RenderDrawLine(mRenderer, 340, 240, 320, 200);

	SDL_Rect destRect;
	destRect.w = 64;
	destRect.h = 64;
	destRect.x = pos.x - (destRect.w / 2);
	destRect.y = pos.y - (destRect.h / 2);
	SDL_RenderCopyEx(mRenderer, mTexture, NULL, &destRect, 45.0, NULL, SDL_FLIP_NONE);

	SDL_RenderPresent(mRenderer);
}
