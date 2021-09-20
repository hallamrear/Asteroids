#include "PCH.h"
#include "Game.h"

Game* app = nullptr;
int main(int argc, char* argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	app = new Game();
	app->Initialise(argc, argv);
	
	if (app->GetIsInitialised() == false)
	{
		app->Shutdown();
		delete app;
		app = nullptr;
		return 0;
	}
	else
		app->SetIsRunning(true);

	Uint32 currentTime = 0, deltaTime = 0, oldTime = 0;
	Uint32 frameTime;

	oldTime = SDL_GetTicks();

	while (app->GetIsRunning())
	{
		currentTime = SDL_GetTicks();
		deltaTime = currentTime - oldTime;

		Game::DeltaTime = (double)((double)deltaTime / 1000.0);

		if(deltaTime != 0)
		{
			app->HandleEvents();
			app->Update();
			app->Draw();
		}

		frameTime = SDL_GetTicks() - currentTime;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);

		oldTime = currentTime;
	}

	app->Shutdown();

	delete app;
	app = nullptr;

	return 0;
}