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

class DirectorState
{
public:
	GameStateIdentifier					Identifier;
	std::function<void()>				StartFunction;
	std::function<void(double)>			UpdateFunction;
	std::function<void(SDL_Renderer&)>	RenderFunction;
	std::function<void()>				EndFunction;

	DirectorState(
		GameStateIdentifier identifier,
		std::function<void()> start,
		std::function<void(double)> update,
		std::function<void(SDL_Renderer&)> render,
		std::function<void()> end)
	{
		this->Identifier = identifier;
		this->StartFunction = start;
		this->UpdateFunction = update;
		this->RenderFunction = render;
		this->EndFunction = end;
	}
};


class GameState
{
	virtual void Start() = 0;
	virtual void Update(double) = 0;
	virtual void Render(SDL_Renderer&) = 0;
	virtual void End() = 0;
};