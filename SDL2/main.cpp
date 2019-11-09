#include "PCH.h"
#include "Application.h"

Application* app = nullptr;
int main(int argc, char* argv[])
{
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	app = new Application();
	app->Initialise(argc, argv);
	
	if (app->GetIsInitialised() == false)
	{
		app->Cleanup();
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

		app->HandleEvents((double)deltaTime / 1000.0);
		app->Update((double)deltaTime / 1000.0);
		app->Draw();

		frameTime = SDL_GetTicks() - currentTime;
		if (frameDelay > frameTime)
			SDL_Delay(frameDelay - frameTime);

		oldTime = currentTime;
	}

	app->Cleanup();

	delete app;
	app = nullptr;

	return 0;
}