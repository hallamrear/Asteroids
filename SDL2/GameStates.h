#pragma once

struct SDLRenderer;

enum class GameStateIdentifier
{
	GAME_STATE_UNKNOWN = 0,
	GAME_STATE_MAIN_MENU,
	GAME_STATE_PAUSED,
	GAME_STATE_RUNNING,
	GAME_STATE_PLAYER_DEATH,
	GAME_STATE_TESTBED
};

class GameState
{
public:
	virtual ~GameState() {};
	virtual void Start() = 0;
	virtual void Update(double) = 0;
	virtual void Render(SDL_Renderer&) = 0;
	virtual void End() = 0;
};