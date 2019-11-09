#pragma once

struct Vector2
{
	float x;
	float y;

	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2(float X, float Y)
	{
		x = X;
		y = Y;
	}
};

class Application
{
private:
	struct SDL_Window*			mWindow;
	struct SDL_Renderer*		mRenderer;
	struct SDL_Texture*			mTexture;

	Vector2 mShipVelocity;
	Vector2 pos = Vector2(0, 540);

	bool mIsInitialised;
	bool mIsRunning;
	//class Graphics sGraphics;

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

