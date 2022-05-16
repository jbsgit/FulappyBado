#include "GameState.h"
#include "Menu.h"
#include "FlappyWorld.h"

//// todo: temporary. delete.
//#include "Savegame.h" 
//#include <vector>


GameState::GameState()
{
}

void GameState::SetGameState(State GS)
{
	_State = GS;
}

State GameState::GetGameState()
{
	return _State;
}

void GameState::Initialize()
{
	FlappyWorld _FlappyWorld;

	SetGameState(State::Menu);
	_FlappyWorld._GameState = this;
	_FlappyWorld.Construct(426, 240, 4, 4, true, true);
	_FlappyWorld.Start();
}


int main()
{
	GameState _GameState;
	_GameState.Initialize();
}

