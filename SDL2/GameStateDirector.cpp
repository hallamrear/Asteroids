#include "PCH.h"
#include "GameStateDirector.h"

GameStateDirector::GameStateDirector()
{
	mCurrentState = nullptr;
}

GameStateDirector::~GameStateDirector()
{
	mCurrentState = nullptr;
}

void GameStateDirector::SetupState(DirectorState* state)
{
	//Todo : error check for uniques
	mDirectorStates.emplace(state);
}

void GameStateDirector::SetState(const GameStateIdentifier state)
{
	for(auto& itr : mDirectorStates)
	{
		if(itr->Identifier == state)
		{
			if (mCurrentState)
			{
				if (mCurrentState->EndFunction)
					mCurrentState->EndFunction();
			}

			if (itr->StartFunction)
				itr->StartFunction();
			
			mCurrentState = itr;
			return;
		}
	}
}

const DirectorState* GameStateDirector::GetState()
{
	return mCurrentState;
}

void GameStateDirector::Update(double deltaTime)
{
	if(mCurrentState->UpdateFunction)
	{
		mCurrentState->UpdateFunction(deltaTime);
	}
}

void GameStateDirector::Render(SDL_Renderer& renderer)
{
	if (mCurrentState->RenderFunction)
	{
		mCurrentState->RenderFunction(renderer);
	}
}
