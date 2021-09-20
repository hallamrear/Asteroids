#pragma once

#include <vector>

class MenuObject;
class Entity;
class Asteroid;
class Projectile;

class Game
{
private:
	int								mWindowWidth;
	int								mWindowHeight;
	bool							mIsInitialised;
	bool							mIsRunning;

	struct SDL_Window*				mWindow;
	struct SDL_Renderer*			mRenderer;
	class  InputManager*			mInputManager;
	class  GameStateDirector*		mStateDirector;
	struct SDL_Texture*				mTexture;
	class  Ship*					mShip;
	class  BoundingBox*				mWindowCollider;
	class  BoundingBox*				mMouseCollider;

	std::vector<MenuObject*>		menuEntities;
	std::vector<Asteroid*>			testAsteroids;
	std::vector<Projectile*>		testProjectiles;
	MenuObject*						testDeathEntity;


	//Game state functions
	void							MenuState_Update();
	void							PlayState_Update();
	void							DeathState_Update();
	void							MenuState_Render();
	void							PlayState_Render();
	void							DeathState_Render();

	bool InitialiseGraphics();
	bool InitialiseWindow(std::string title = "", int xpos = 0, int ypos = 0, int width = 0, int height = 0, Uint32 flags = SDL_WINDOW_SHOWN, bool isFullscreen = false);
	void SetupGameStateFunctions();
	bool InitialiseWorldObjects();
	bool InitialiseSystems();
	void CleanupDeadEntites();

public:
	static double					DeltaTime;

	Game();
	~Game();

	void Initialise(int argc, char* argv[]);

	void HandleEvents();
	void Update();
	void Draw();

	//Setters
	void SetIsRunning(bool state) { mIsRunning = state; };
	//Getters
	const bool GetIsRunning() const { return mIsRunning; };
	const bool GetIsInitialised() const { return mIsInitialised; };

	void Shutdown();
};

