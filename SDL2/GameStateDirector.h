#pragma once
#include <set>
#include "GameStates.h"

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
