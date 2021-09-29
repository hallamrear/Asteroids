#pragma once
#include <map>
#include "GameStates.h"

//todo : make singleton
class StateDirector
{
private:
	static StateDirector* mInstance;

	GameState* mCurrentState;
	std::map<GameStateIdentifier, GameState*> mDirectorStates;

	StateDirector();
	~StateDirector();

	void					SetState_Impl(const GameStateIdentifier state);
	void					SetupState_Impl(GameStateIdentifier identifier, GameState* state);
	const GameState*		GetState_Impl();
	static StateDirector*	Get();
	void					Update_Impl(double DeltaTime);
	void					Render_Impl(SDL_Renderer& renderer);

public:
	static void				SetState(const GameStateIdentifier state);
	static void				SetupState(GameStateIdentifier identifier, GameState* state);
	static const GameState*	GetState();
	static void				Update(double DeltaTime);
	static void				Render(SDL_Renderer& renderer);
};
