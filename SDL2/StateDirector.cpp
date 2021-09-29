#include "PCH.h"
#include "StateDirector.h"

StateDirector* StateDirector::mInstance = nullptr;

StateDirector::StateDirector()
{
	mCurrentState = nullptr;
}

StateDirector::~StateDirector()
{
	mCurrentState = nullptr;
}

void StateDirector::SetupState(GameStateIdentifier identifier, GameState* state)
{
	Get()->SetupState_Impl(identifier, state);
}

void StateDirector::SetupState_Impl(GameStateIdentifier identifier, GameState* state)
{
	//Todo : error check for uniques
	//todo : check if identifier exists and if so, throw;
	mDirectorStates.emplace(identifier, state);
}

void StateDirector::SetState_Impl(const GameStateIdentifier state)
{
	auto itr = mDirectorStates.find(state);
	assert(itr != mDirectorStates.end());

	if (mCurrentState)
		mCurrentState->End();
	mCurrentState = itr->second;
	mCurrentState->Start();
}

void StateDirector::SetState(const GameStateIdentifier state)
{
	Get()->SetState_Impl(state);
}

StateDirector* StateDirector::Get()
{
	if (!mInstance)
		mInstance = new StateDirector();

	return mInstance;
}

const GameState* StateDirector::GetState_Impl()
{
	return mCurrentState;
}

const GameState* StateDirector::GetState()
{
	return Get()->GetState_Impl();
}

void StateDirector::Update(double DeltaTime)
{
	Get()->Update_Impl(DeltaTime);
}

void StateDirector::Render(SDL_Renderer& renderer)
{
	Get()->Render_Impl(renderer);
}

void StateDirector::Update_Impl(double DeltaTime)
{
	if(mCurrentState)
		mCurrentState->Update(DeltaTime);
}
void StateDirector::Render_Impl(SDL_Renderer& renderer)
{
	if (mCurrentState)
		mCurrentState->Render(renderer);
}
