#pragma once
#include <set>

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
private:
//todo : possibly include create and destruct functions to expand the states

public:
	GameStateIdentifier		Identifier;
	std::function<void()>	UpdateFunction;
	std::function<void(SDL_Renderer&)>	RenderFunction;

	DirectorState(GameStateIdentifier identifier, std::function<void()> update, std::function<void(SDL_Renderer&)> render)
	{
		this->Identifier = identifier;
		this->UpdateFunction = update;
		this->RenderFunction = render;
	}
};

class GameStateDirector
{
private:
	DirectorState* mCurrentState;
	std::set<DirectorState*> mDirectorStates;

public:
	GameStateDirector();
	~GameStateDirector();

	void				 SetupState(DirectorState* state);
	void				 SetState(const GameStateIdentifier state);
	const DirectorState* GetState();
	void				 Update(double deltaTime);
	void				 Render(SDL_Renderer& renderer);
};
