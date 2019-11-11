#pragma once

struct KeyPresses
{
	bool Key_Up = false;
	bool Key_Down = false;
	bool Key_Left = false;
	bool Key_Right = false;
};

class Application
{
private:
	int mWindowWidth;
	int mWindowHeight;

	struct SDL_Window*			mWindow;
	struct SDL_Renderer*		mRenderer;
	struct SDL_Texture*			mTexture;
	class  Ship*				mShip;
	class  Asteroid*				tAsteroid;

	KeyPresses mKeyStates;

	bool mIsInitialised;
	bool mIsRunning;

	bool InitialiseGraphics();
	bool InitialiseWindow(std::string title = "", int xpos = 0, int ypos = 0, int width = 0, int height = 0, Uint32 flags = SDL_WINDOW_SHOWN, bool isFullscreen = false);

public:
	Application();
	~Application();

	void Initialise(int argc, char* argv[]);

	void HandleEvents(double deltaTime);
	void Update(double deltaTime);
	void Draw();

	//Setters
	void SetIsRunning(bool state) { mIsRunning = state; };
	//Getters
	const bool GetIsRunning() const { return mIsRunning; };
	const bool GetIsInitialised() const { return mIsInitialised; };

	void Cleanup();
};

