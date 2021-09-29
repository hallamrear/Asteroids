#pragma once

class Game
{
private:
	bool						mIsInitialised;
	bool						mIsRunning;

	struct SDL_Window*			mWindow;

	bool InitialiseGraphics();
	bool InitialiseWindow(std::string title = "", int xpos = 0, int ypos = 0, int width = 0, int height = 0, Uint32 flags = SDL_WINDOW_SHOWN, bool isFullscreen = false);
	void SetupGameStateFunctions();
	bool InitialiseWorldObjects();
	bool InitialiseSystems();

public:
	static double				DeltaTime;
	static struct SDL_Renderer*	Renderer;

	Game();
	~Game();

	void		Initialise(int argc, char* argv[]);
	void		HandleEvents();
	void		Update();
	void		Render();
	void		Shutdown();

	void	    SetIsRunning(bool state) { mIsRunning = state; };
	const bool  GetIsRunning() const { return mIsRunning; };
	const bool  GetIsInitialised() const { return mIsInitialised; };

};

