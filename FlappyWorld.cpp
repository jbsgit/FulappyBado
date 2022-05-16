#include "FlappyWorld.h"
#include "Savegame.h"
FlappyWorld::FlappyWorld()
{
	sAppName = "Fulappi Bado";
}

bool FlappyWorld::OnUserCreate()
{
	sprBirdGlide = std::make_unique<olc::Sprite>("./Gfx/BirdGlide.png");
	sprBirdFlap = std::make_unique<olc::Sprite>("./Gfx/BirdFlap.png");
	sprTreeTrunk = std::make_unique<olc::Sprite>("./Gfx/TreeTrunk.png");
	sprSkyBackground = std::make_unique<olc::Sprite>("./Gfx/SkyBackground.png");
	sprSkyBackgroundLower = std::make_unique<olc::Sprite>("./Gfx/SkyBackgroundLower.png");

	ListOfObstacles = { 0, 0, 0, 0 };
	fObstacleGapWidth = (float)ScreenWidth() / (float)(ListOfObstacles.size() - 1); // gap between obstacles

	sprBirdPoop = std::make_unique<olc::Sprite>("./Gfx/BirdPoop.png");
	decBirdPoop = std::make_unique<olc::Decal>(sprBirdPoop.get());

	sprCrappyBirdTitle = std::make_unique<olc::Sprite>("./Gfx/CrappyBirdTitle.png");
	sprGroundBackground = std::make_unique<olc::Sprite>("./Gfx/GroundBackground.png");

	ScoreText = "0";

	if (_GameState == nullptr)
	{
		std::cout << "No _GameState";
		return false;
	}
	// load sound into buffer
	if (!BadoPoopBuffer.loadFromFile("./Sound/BadoPoop.ogg"))
	{
		return -1;
	}
	if (!BadoSplatBuffer.loadFromFile("./Sound/BadoSplat.ogg"))
	{
		return -1;
	}
	if (!FulappiBadoBuffer.loadFromFile("./Sound/fulappivoice.ogg"))
	{
		return -1;
	}
	if (!FlapSoundBuffer.loadFromFile("./Sound/flap.ogg"))
	{
		return -1;
	}
	if (!GameLostSoundBuffer.loadFromFile("./Sound/GameLost.ogg"))
	{
		return -1;
	}
	if (!HighScoreSoundBuffer.loadFromFile("./Sound/HighScore.ogg"))
	{
		return -1;
	}
	// Init sound
	FlapSound.setBuffer(FlapSoundBuffer);
	FlapSound.setVolume(20.f);
	FlapSound.setPitch(3.f);

	GameLostSound.setBuffer(GameLostSoundBuffer);
	GameLostSound.setVolume(100.f);
	GameLostSound.setPitch(1.f);

	HighScoreSound.setBuffer(HighScoreSoundBuffer);
	HighScoreSound.setVolume(100.f);
	HighScoreSound.setPitch(1.f);

	FulappiBado.setBuffer(FulappiBadoBuffer);
	FulappiBado.setVolume(100.f);
	FulappiBado.setPitch(1.f);

	BadoPoop.setBuffer(BadoPoopBuffer);
	BadoPoop.setVolume(50.f);
	BadoPoop.setPitch(1.f);

	BadoSplat.setBuffer(BadoSplatBuffer);
	BadoSplat.setVolume(50.f);
	BadoSplat.setPitch(1.2f);

	if (!Music.openFromFile("./Sound/FulappiMusic.ogg"))
		return -1;
	Music.setLoop(1);
	Music.play();

	HighScoreSound.play();
	return true;
}

bool FlappyWorld::OnUserUpdate(float fElapsedTime)
{
	switch (_GameState->GetGameState()) // `gamestate
	{
	case State::Menu:
		_Menu.MainMenu(this);
		break;
	case State::Playing:
		PlayGame(fElapsedTime);
		break;
	case State::Paused:
		Paused(fElapsedTime);
		break;
	case State::GameOver:
		GameOver(fElapsedTime);
		break;
	default:
		break;
	}

	return true;
}

void FlappyWorld::PlayGame(float fElapsedTime)
{
	if (bResetGame) // `resetgame
	{
		ListOfObstacles = { 0, 0, 0, 0 };
		ScoreText = "0";
		nScore = 0;
		fBirdVelocity = 0.f;
		fBirdAcceleration = 0.f;
		fBirdPositionY = ScreenHeight() / 2;
		fObstaclePosition = 0.f;
		fObstaclePositionSpeed = 140.f;
		fObstacleGapHeight = 75.f;
		fSkyBackgroundSpeed = 45.f;
		fSkyBackgroundLowerSpeed = 70.f;
		bTriggerObstacleClear = true;
		bClearObstacles = false;
		bResetGame = false;
		bJustStartedGame = true;
		Music.setVolume(100.f);
		FulappiBado.play();
	}
	if (GetKey(olc::ESCAPE).bPressed) // `pause
	{
		_GameState->SetGameState(State::Paused);
	}
	//level movement
	if (!bJustStartedGame)
	{
		fObstaclePositionSpeed += fObstaclePositionSpeedIncrease * fElapsedTime;
		if (fObstaclePositionSpeed > fObstaclePositionMaxSpeed)
		{
			fObstaclePositionSpeed = fObstaclePositionMaxSpeed;
		}


		fObstaclePosition += fObstaclePositionSpeed * fElapsedTime;
		if (fObstaclePosition > fObstacleGapWidth)
		{
			fBirdPositionXBackTEST = 90.f;
			fObstaclePosition -= fObstacleGapWidth;
			if (!ListOfObstacles.empty())
			{
				ListOfObstacles.pop_front();
			}
			//int i = rand() % (ScreenHeight() - 140);
			int i = rand() % (ScreenHeight() - 100);
			if (i <= 7)
			{
				i = 0;
			}
			if (!bClearObstacles)
			{
				ListOfObstacles.push_back(i);
			}
		}
	}
	// bird physics
	if (GetKey(olc::SPACE).bPressed || GetMouse(0).bPressed)
	{
		fBirdAcceleration = fBirdAcceleration / 2;
		fBirdVelocity = -fGravity / 10;
		bJustStartedGame = false;
		FlapSound.play();
	}

	if (!bJustStartedGame)
	{
		fBirdAcceleration += fGravity * fElapsedTime;

		if (fBirdAcceleration > fGravity)
		{
			fBirdAcceleration = fGravity;
		}
		fBirdVelocity += fBirdAcceleration * fElapsedTime;
		fBirdPositionY += fBirdVelocity * fElapsedTime;
	}
	Clear(olc::VERY_DARK_BLUE);
	DrawBackGround(fElapsedTime);
	// draw level
	SetPixelMode(olc::Pixel::ALPHA);

	int nObstacleSection = 0;
	for (auto nObstacleRandomHeight : ListOfObstacles)
	{
		float fObstaclePosx = nObstacleSection * fObstacleGapWidth + 10 - fObstaclePosition;
		float fLowerObstacleHeight = ScreenHeight() - nObstacleRandomHeight;
		float fUpperObstacleHeight = ScreenHeight() - nObstacleRandomHeight - fObstacleGapHeight;
		if (nObstacleRandomHeight != 0)
		{
			 //test for `collision
			 //is bird's front and back position between the obstacles left and right edge
			if ((fObstaclePosx < fBirdPositionXFront &&
				fObstaclePosx + nObstacleWidth > fBirdPositionXFront)
				||
				(fObstaclePosx < fBirdPositionXBack + 5 &&
					fObstaclePosx + nObstacleWidth > fBirdPositionXBack + 5))
			{
				// is it touching obstacle
				if (fBirdPositionY + 10 > fLowerObstacleHeight ||
					fBirdPositionY < fUpperObstacleHeight - 5)
				{
					_GameState->SetGameState(State::GameOver);
				}
			}

			// add `score
			if ((nCheckObstacleSection = nObstacleSection) && (fObstaclePosx + nObstacleWidth < fBirdPositionXBackTEST))
			{
				fBirdPositionXBackTEST = 0.f;
				++nCheckObstacleSection;
				++nScore;
				ScoreText = std::to_string(nScore);
			}
			// draw obstacles
			DrawSprite(olc::vi2d(fObstaclePosx, fLowerObstacleHeight), sprTreeTrunk.get(), 1, 2);
			DrawSprite(olc::vi2d(fObstaclePosx, -240 + fUpperObstacleHeight), sprTreeTrunk.get(), 1);
		}
		nObstacleSection++;
	}
	// obstacle clear and re-add after speed increase
	if (fObstaclePositionSpeed > 192 && bTriggerObstacleClear)
	{
		bClearObstacles = true;
		if (ListOfObstacles.empty())
		{
			ListOfObstacles.push_back(0);
			ListOfObstacles.push_back(0);
			ListOfObstacles.push_back(0);
			fObstacleGapWidth = (float)ScreenWidth() / (float)(ListOfObstacles.size() - 1); 
			fObstaclePosition = 0.f;
			fObstaclePositionSpeedIncrease = 30.f;
			fObstacleGapHeight = 93.f;
			bClearObstacles = false;
			bTriggerObstacleClear = false;
		}
	}
	else if (fObstaclePositionSpeed > 280)
	{
		fObstaclePositionSpeedIncrease = 2.f;
	}
	if (fObstaclePositionSpeed > 350)
	{
		bClearObstacles = true;
		if (ListOfObstacles.empty())
		{
			ListOfObstacles.push_back(0);
			ListOfObstacles.push_back(0);
			ListOfObstacles.push_back(0);
			ListOfObstacles.push_back(0);
			fObstacleGapWidth = (float)ScreenWidth() / (float)(ListOfObstacles.size() - 1);
			fObstaclePosition = 0.f;
			fObstaclePositionSpeed = 150.f;
			fSkyBackgroundSpeed = 45.f;
			fSkyBackgroundLowerSpeed = 70.f;
			fObstaclePositionSpeedIncrease = 1.5f;
			fObstacleGapHeight = 75.f;
			bClearObstacles = false;
			bTriggerObstacleClear = true;
		}
	}

	// draw score, draw speed
	size_t NewSize = 3;
	std::string SpeedText = std::to_string(fObstaclePositionSpeed);
	SpeedText.resize(NewSize);
	DrawString({ 10, 7 }, "Score: " + ScoreText, olc::BLACK);
	DrawString({ 10, 7+16 }, "Speed: " + SpeedText, olc::BLACK);

	// draw bird  48x16
	if (bJustStartedGame)
	{
		DrawString(olc::vi2d(fBirdPositionXBack, fBirdPositionY - 25), "Press Space or Left mouse to flap.", olc::BLACK);
		DrawSprite(olc::vi2d(fBirdPositionXBack, fBirdPositionY), sprBirdGlide.get(), 1);

	}
	else if (fBirdVelocity > 0) // bird gliding
		DrawSprite(olc::vi2d(fBirdPositionXBack, fBirdPositionY), sprBirdGlide.get(), 1);
	else // bird flapping
		DrawSprite(olc::vi2d(fBirdPositionXBack, fBirdPositionY), sprBirdFlap.get(), 1);
	SetPixelMode(olc::Pixel::NORMAL);
}

void FlappyWorld::GameOver(float fElapsedTime)
{
	int iScore = std::stoi(ScoreText);
	if (bGameOverCheck)
	{
		Music.setVolume(30.f);
		Savegame savegame;
		MinimumScore = savegame.GetMiniumScore().first;
		ScoresLength = savegame.GetMiniumScore().second;
		// Place in top 5 if better than minimum score, or same as minimum score if there is room on the scoreboard.
		if (iScore > MinimumScore || (iScore == MinimumScore && ScoresLength < 5))
		{
			bSubmittingScore = true;
			HighScoreSound.play();
		}
		else
			GameLostSound.play();
		bGameOverCheck = false;
	}
	// draw game over screen
	FillRect({ ScreenWidth() / 2 - 90, ScreenHeight() / 2 - GameOverBoxHeightStartModifier }, { 190, 60 + GameOverBoxHeightEndModifier }, olc::VERY_DARK_GREY);
	DrawString({ ScreenWidth() / 2 - 35, ScreenHeight() / 2 - 52 }, "Game over", olc::WHITE);
	GameOverBoxHeightEndModifier = 42;
	// if highscore, draw name entry stuff
	if (iScore > MinimumScore || (iScore == MinimumScore && ScoresLength < 5)) // highscore
	{
		GameOverBoxHeightStartModifier = 44;
		DrawString({ ScreenWidth() / 2 - 85, ScreenHeight() / 2 - 50 + 16 }, "New highscore!", olc::WHITE);
		if (bSubmittingScore)
		{
			DrawString({ ScreenWidth() / 2 - 85, ScreenHeight() / 2 - 50 + 32 }, "Enter your name: ", olc::WHITE);
			// get name input.
			if (GetKey(olc::BACK).bPressed && !Name.empty())
				Name.pop_back();
			for (auto& m : valueInputKeys)
			{
				if (GetKey(m.key).bPressed && Name.length() < 9)
				{
					if (GetKey(olc::SHIFT).bHeld)
						Name += m.upper;
					else
						Name += m.lower;
				}
			}
			// draw name
			DrawString({ ScreenWidth() / 2 - 85, ScreenHeight() / 2 - 50 + 48 }, Name, olc::WHITE);
		}
		else // after inputing name
			DrawString({ ScreenWidth() / 2 - 85, ScreenHeight() / 2 - 50 + 32 }, "Score submitted!", olc::WHITE);
	}
	// submit name and score to savefile
	if (GetKey(olc::ENTER).bPressed && !Name.empty() && bSubmittingScore)
	{
		Savegame savegame;
		savegame.SaveScore(Name, iScore);
		Name.clear();
		bScoreSubmitted = true;
		bSubmittingScore = false;
	}
	// reset game if enter
	else if (GetKey(olc::ENTER).bPressed && !bSubmittingScore)
	{
		bResetGame = true;
		bGameOverCheck = true;
		_GameState->SetGameState(State::Playing);

	}
	if (bScoreSubmitted || !bSubmittingScore)
	{
		DrawString({ ScreenWidth() / 2 - 85, ScreenHeight() / 2 - 50 + 64 }, "Enter: Retry", olc::WHITE);
		DrawString({ ScreenWidth() / 2 - 85, ScreenHeight() / 2 - 50 + 80 }, "Esc: Menu.", olc::WHITE);

		if (GetKey(olc::ESCAPE).bPressed)
		{
			GoToMenu();
		}
	}
	if (GameLostSound.getStatus() == sf::SoundSource::Status::Stopped)
	{
		Music.setVolume(100.f);
	}
}

void FlappyWorld::DrawBackGround(float fElapsedTime)
{
	/* Background is split in two horizontal images, which scrolls at different speed for a parallax effect */
	//upper
	fSkyBackgroundSpeed += fObstaclePositionSpeedIncrease * fElapsedTime;
	fSkyBackgroundPosition -= fSkyBackgroundSpeed * fElapsedTime;
	if (fSkyBackgroundPosition < -852)
	{
		FlipBackgroundUpper = !FlipBackgroundUpper;
		fSkyBackgroundPosition += 852;
	}
	DrawSprite(olc::vi2d(fSkyBackgroundPosition, 0), sprSkyBackground.get(), 1, FlipBackgroundUpper);
	//lower
	fSkyBackgroundLowerSpeed += fObstaclePositionSpeedIncrease * fElapsedTime;
	fSkyBackgroundLowerPosition -= fSkyBackgroundLowerSpeed * fElapsedTime;
	if (fSkyBackgroundLowerPosition < -852)
	{
		FlipBackgroundLower = !FlipBackgroundLower;
		fSkyBackgroundLowerPosition += 852;

	}
	DrawSprite(olc::vi2d(fSkyBackgroundLowerPosition, 120), sprSkyBackgroundLower.get(), 1, FlipBackgroundLower);
}

void FlappyWorld::Paused(float fElapsedTime)
{
	DrawString({ ScreenWidth() / 2 - 60, ScreenHeight() / 2 - 50 }, "   Paused", olc::BLACK);
	DrawString({ ScreenWidth() / 2 - 60, ScreenHeight() / 2 - 25 }, "Esc   - Menu", olc::BLACK);
	DrawString({ ScreenWidth() / 2 - 60, ScreenHeight() / 2 - 0 }, "Enter - Continue", olc::BLACK);
	if (GetKey(olc::ESCAPE).bPressed)
		GoToMenu();
	if (GetKey(olc::ENTER).bPressed)
		_GameState->SetGameState(State::Playing);
}

void FlappyWorld::GoToMenu()
{
	bResetGame = true;
	bGameOverCheck = true;
	_Menu.SetCalcScores(true);
	_GameState->SetGameState(State::Menu);
}
