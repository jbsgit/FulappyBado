#pragma once
#include "olcPixelGameEngine.h"
#include "GameState.h"
#include "Menu.h"
#include "SFML\Audio.hpp"

class FlappyWorld : public olc::PixelGameEngine
{
	friend class Menu; // gives Menu class full access to this class and its variables.
public: 
	FlappyWorld();
	GameState* _GameState;
	Menu _Menu;

protected: 

	virtual bool OnUserCreate() override;
	virtual bool OnUserUpdate(float fElapsedTime) override;
	void PlayGame(float fElapsedTime);
	void GameOver(float fElapsedTime);
	void DrawBackGround(float fElapsedTime);
	void Paused(float fElapsedTime);
	void GoToMenu();

private:
	float fBirdVelocity = 0.f;
	float fBirdAcceleration = 0.f;
	float fBirdPositionY = ScreenHeight() / 2;
	float fBirdPositionXBack = ScreenWidth() / 3;
	float fBirdPositionXFront = fBirdPositionXBack + (8 * 6);
	float fGravity = 2400.f;

	std::list<int> ListOfObstacles;
	float fObstacleGapWidth;
	int nObstacleWidth = 30; //default 30
	float fObstaclePosition = 0.f;
	float fObstaclePositionScoreCalc = fObstaclePosition;
	float fObstacleMinScreenEdge = 12.f; //12 // minimum distance from top and bottom of the screen.
	float fObstacleGapHeight = 75.f; // default 85
	float fObstaclePositionSpeed = 140.f; // default: 140.
	float fObstaclePositionSpeedIncrease = 1.5f;
	float fObstaclePositionMaxSpeed = 352.f; // default 350.
	bool bTriggerObstacleClear = true;
	bool bClearObstacles = false;

	std::unique_ptr<olc::Sprite> sprBirdGlide;
	std::unique_ptr<olc::Sprite> sprBirdFlap;
	std::unique_ptr<olc::Sprite> sprTreeTrunk;
	std::unique_ptr<olc::Sprite> sprSkyBackground;
	std::unique_ptr<olc::Sprite> sprSkyBackgroundLower;
	float fSkyBackgroundSpeed = 45.f;
	float fSkyBackgroundPosition;
	float fSkyBackgroundLowerSpeed = 70.f;
	float fSkyBackgroundLowerPosition;
	int FlipBackgroundUpper = 0;
	int FlipBackgroundLower = 0;
	std::unique_ptr<olc::Sprite> sprBirdPoop;
	std::unique_ptr<olc::Decal> decBirdPoop;
	std::unique_ptr<olc::Sprite> sprCrappyBirdTitle;
	std::unique_ptr<olc::Sprite> sprGroundBackground;

	int nScore = 0;
	std::string ScoreText;
	bool bAddScore = true;

	int nCheckObstacleSection;
	float fBirdPositionXBackTEST = 90.f;

	bool bResetGame = false;
	bool bJustStartedGame = true;

	sf::SoundBuffer FlapSoundBuffer;
	sf::Sound FlapSound;

	sf::SoundBuffer GameLostSoundBuffer;
	sf::Sound GameLostSound;

	sf::SoundBuffer HighScoreSoundBuffer;
	sf::Sound HighScoreSound;

	sf::SoundBuffer FulappiBadoBuffer;
	sf::Sound FulappiBado;

	sf::SoundBuffer BadoPoopBuffer;
	sf::Sound BadoPoop;

	sf::SoundBuffer BadoSplatBuffer;
	sf::Sound BadoSplat;

	sf::Music Music;

	std::string Name;

	int GameOverBoxHeightStartModifier = 60;
	int GameOverBoxHeightEndModifier = 0;
	bool bSubmittingScore = false;
	bool bScoreSubmitted = false;
	bool bGameOverCheck = true;
	int MinimumScore;
	size_t ScoresLength;

	struct KeyCharMap
	{
		olc::Key key;
		char lower;
		char upper;
	};

	std::vector<KeyCharMap> valueInputKeys =
	{
					{olc::A, 'a', 'A'},
					{olc::B, 'b', 'B'},
					{olc::C, 'c', 'C'},
					{olc::D, 'd', 'D'},
					{olc::E, 'e', 'E'},
					{olc::F, 'f', 'F'},
					{olc::G, 'g', 'G'},
					{olc::H, 'h', 'H'},
					{olc::I, 'i', 'I'},
					{olc::J, 'j', 'J'},
					{olc::K, 'k', 'K'},
					{olc::L, 'l', 'L'},
					{olc::M, 'm', 'M'},
					{olc::N, 'n', 'N'},
					{olc::O, 'o', 'O'},
					{olc::P, 'p', 'P'},
					{olc::Q, 'q', 'Q'},
					{olc::R, 'r', 'R'},
					{olc::S, 's', 'S'},
					{olc::T, 't', 'T'},
					{olc::U, 'u', 'U'},
					{olc::V, 'v', 'V'},
					{olc::W, 'w', 'W'},
					{olc::X, 'x', 'X'},
					{olc::Y, 'y', 'Y'},
					{olc::Z, 'z', 'Z'},
					{olc::K0, '0', ')'},
					{olc::K1, '1', '!'},
					{olc::K2, '2', '@'},
					{olc::K3, '3', '#'},
					{olc::K4, '4', '$'},
					{olc::K5, '5', '%'},
					{olc::K6, '6', '^'},
					{olc::K7, '7', '&'},
					{olc::K8, '8', '*'},
					{olc::K9, '9', '('},
					{olc::NP0, '0', '0'},
					{olc::NP1, '1', '1'},
					{olc::NP2, '2', '2'},
					{olc::NP3, '3', '3'},
					{olc::NP4, '4', '4'},
					{olc::NP5, '5', '5'},
					{olc::NP6, '6', '6'},
					{olc::NP7, '7', '7'},
					{olc::NP8, '8', '8'},
					{olc::NP9, '9', '9'},
					{olc::NP_MUL, '*', '*'},
					{olc::NP_DIV, '/', '/'},
					{olc::NP_ADD, '+', '+'},
					{olc::NP_SUB, '-', '-'},
					{olc::NP_DECIMAL, '.', '.'},
					{olc::PERIOD, '.', '>'},
					{olc::SPACE, ' ', ' '},
					{olc::OEM_1, ';', ':'},
					{olc::OEM_2, '/', '?'},
					{olc::OEM_3, '`', '~'},
					{olc::OEM_4, '[', '{'},
					{olc::OEM_5, '\\', '|'},
					{olc::OEM_6, ']', '}'},
					{olc::OEM_7, '\'', '"'},
					{olc::OEM_8, '-', '-'},
					{olc::EQUALS, '=', '+'},
					{olc::COMMA, ',', '<'},
					{olc::MINUS, '-', '_'}
	};
};