#pragma once
//#include <chrono>
//#include <thread>
#include "olcPixelGameEngine.h"
class FlappyWorld;

class Menu
{
	enum class MenuSelection
	{
		Play,
		Credits,
		Quit,
		None
	};

public:
	Menu();
	void MainMenu(FlappyWorld* fw);
	void Background(FlappyWorld* fw);
	void Options(FlappyWorld* fw);
	void HighScores(FlappyWorld* fw);
	void SetCalcScores(bool state);

private:
	float fSkyBackgroundSpeed = 30.f;
	float fSkyBackgroundPosition = 0.f;
	float fSkyBackgroundLowerSpeed = 45.f;
	float fSkyBackgroundLowerPosition;
	int FlipBackgroundUpper = 0;
	int FlipBackgroundLower = 0;

	float fBirdX = 0.f;
	float fBirdVelocity = 0.f;
	float fBirdAcceleration = 0.f;
	float fBirdMaxAcceleration = 100.f;
	float fBirdMaxSpeed = 105.f;

	float fBirdY = 0.f;

	bool bFlyRight = true;
	bool bFlipBird = false;
	bool bSlowDown = false;
	bool bBirdYSet = false;

	bool bFlyUp = true;

	std::chrono::time_point<std::chrono::steady_clock> Time_Now;
	std::chrono::time_point<std::chrono::steady_clock> Time_Then;
	std::chrono::duration<float> Time_Duration;
	bool bFreezeTimeThen = false;
	bool bToggleFlap = true;

	bool bInitPoop = false;// Init poop only after timer already ran once.
	float fPoopVelocity = 150.f;
	float fPoopY;
	float fPoopX;
	bool bStartPoop = true;
	float fPoopHitX;
	bool bPoopHit = true;

	olc::Pixel PoopHitColor;

	struct sPoopFragments
	{
		olc::vf2d Pos;
		olc::vf2d Vel;
		float fAngle;
		float fTime;
		olc::Pixel Color;
	};

	std::list<sPoopFragments> ListPoopFragments;
	float fFragmentLifeTime = .5f;

	std::chrono::time_point<std::chrono::steady_clock> Time_Then_Poop;
	std::chrono::duration<float> Time_Duration_Poop;

	std::chrono::time_point<std::chrono::steady_clock> Time_Then_TitleVoice;
	std::chrono::duration<float> Time_Duration_TitleVoice;
	bool bPlayVoice = false;
	std::once_flag onceFlag;

	MenuSelection _MenuSelection;
	float fTextSizePlay = 2.f;
	float fTextSizeCredits = 1.f;
	float fTextSizeQuit = 1.f;

	bool bDisplayCredits = false;
	bool bMouseOverMenuItem = false;
	bool bHideScores = false;
	int WidthOffset = 25;
	std::string Name0, Name1, Name2, Name3, Name4, Score0, Score1, Score2, Score3, Score4;

	bool bCalcScores = true;
};

