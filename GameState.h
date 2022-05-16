#pragma once

enum class State
{
	Menu,
	Playing,
	Paused,
	GameOver
};

class GameState
{
public: // functions
	GameState();
	void SetGameState(State GS);
	State GetGameState();
	void Initialize();
private: // vars 
	State _State;
};

int main();

