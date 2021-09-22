#pragma once

struct SDLRenderer;

enum class GameStateIdentifier
{
	GAME_STATE_UNKNOWN = 0,
	GAME_STATE_MAIN_MENU,
	GAME_STATE_PAUSED,
	GAME_STATE_RUNNING,
	GAME_STATE_PLAYER_DEATH
};

class DirectorState
{
public:
	GameStateIdentifier					Identifier;
	std::function<void()>				StartFunction;
	std::function<void(float)>			UpdateFunction;
	std::function<void(SDL_Renderer&)>	RenderFunction;
	std::function<void()>				EndFunction;

	DirectorState(
		GameStateIdentifier identifier,
		std::function<void()> start,
		std::function<void(float)> update,
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