#include "Menu.h"
#include "FlappyWorld.h"
#include "Savegame.h"
#include <algorithm>
#include "SFML\Audio.hpp"
#include <mutex>


Menu::Menu()
{
	PoopHitColor = olc::WHITE;
	PoopHitColor.a = 255;
	_MenuSelection = MenuSelection::Play;
}

void Menu::MainMenu(FlappyWorld* fw)
{
	Background(fw);
	HighScores(fw);
	Options(fw);
}

void Menu::Background(FlappyWorld* fw)
{
	float fElapsedTime = fw->GetElapsedTime();
	fw->SetPixelMode(olc::Pixel::NORMAL);
	fw->Clear(olc::VERY_DARK_BLUE);
	// draw background
	//upper
	fSkyBackgroundPosition -= fSkyBackgroundSpeed * fElapsedTime;
	if (fSkyBackgroundPosition < -852)
	{
		FlipBackgroundUpper = !FlipBackgroundUpper;
		fSkyBackgroundPosition += 852;
	}
	fw->DrawSprite(olc::vi2d(fSkyBackgroundPosition, 0), fw->sprSkyBackground.get(), 1, FlipBackgroundUpper);
	//lower
	fSkyBackgroundLowerPosition -= fSkyBackgroundLowerSpeed * fElapsedTime;
	if (fSkyBackgroundLowerPosition < -852)
	{
		FlipBackgroundLower = !FlipBackgroundLower;
		fSkyBackgroundLowerPosition += 852;

	}
	fw->DrawSprite(olc::vi2d(fSkyBackgroundLowerPosition, 120), fw->sprSkyBackgroundLower.get(), 1, FlipBackgroundLower);

	//ground
	fw->DrawSprite(olc::vi2d(0, fw->ScreenHeight() - 15), fw->sprGroundBackground.get(), 1);

	// bird flying movement
	if (fBirdX <= 0.f) fBirdX = (float)(fw->ScreenWidth() / 8);

	fBirdAcceleration += fElapsedTime * fBirdMaxSpeed;
	if (fBirdAcceleration > fBirdMaxAcceleration)
	{
		fBirdAcceleration = fBirdMaxAcceleration;
	}

	if (!bSlowDown)
	{
		fBirdVelocity += fBirdAcceleration * fElapsedTime;
		if (fBirdVelocity > fBirdMaxSpeed)
		{
			fBirdVelocity = fBirdMaxSpeed;
		}
	}
	else if (bSlowDown)
	{
		fBirdVelocity -= fBirdAcceleration * fElapsedTime;
		if (fBirdVelocity < 10.f)
		{
			fBirdVelocity = 10.f;
		}
	}
	// back and forth
	if ((fBirdX < fw->ScreenWidth() - (fw->ScreenWidth() / 10) - 48 - (fw->ScreenWidth() / 7)) && bFlyRight)
	{
		fBirdX += fElapsedTime * fBirdVelocity;
	}
	else if ((fBirdX < fw->ScreenWidth() - (fw->ScreenWidth() / 10) - 48) && bFlyRight)
	{
		if (!bSlowDown) bSlowDown = true;
		fBirdX += fElapsedTime * fBirdVelocity;
	}
	else if (bFlyRight)
	{
		fBirdAcceleration = 0.f;
		fBirdVelocity = 0.f;
		bFlyRight = !bFlyRight;
		bFlipBird = !bFlipBird;
		bSlowDown = !bSlowDown;
	}
	if ((fBirdX > (fw->ScreenWidth() / 10) + (fw->ScreenWidth() / 7)) && !bFlyRight)
	{
		fBirdX -= fElapsedTime * fBirdVelocity;
	}
	else if ((fBirdX > fw->ScreenWidth() / 10) && !bFlyRight)
	{
		if (!bSlowDown) bSlowDown = true;
		fBirdX -= fElapsedTime * fBirdVelocity;
	}
	else if (!bFlyRight)
	{
		fBirdAcceleration = 0.f;
		fBirdVelocity = 0.f;
		bFlyRight = !bFlyRight;
		bFlipBird = !bFlipBird;
		bSlowDown = !bSlowDown;
	}
	// fly up and down
	if (fBirdY <= 0.f && !bBirdYSet)
	{
		fBirdY = (float)(fw->ScreenHeight() / 4);
		bBirdYSet = true;
	}

	float Random = rand() % 10;

	if (fBirdY < (float)(fw->ScreenHeight() / 4) + 10 && bFlyUp)
	{
		fBirdY += fBirdVelocity * Random / 50 * fElapsedTime;
	}
	else if (bFlyUp)
	{
		bFlyUp = !bFlyUp;
	}
	if (fBirdY > (float)(fw->ScreenHeight() / 4) - 5 && !bFlyUp)
	{
		fBirdY -= fBirdVelocity * Random / 50 * fElapsedTime;
	}
	else if (!bFlyUp)
	{
		bFlyUp = !bFlyUp;
	}
	fw->SetPixelMode(olc::Pixel::MASK);
	// Game title graphics
	fw->DrawSprite(olc::vi2d(fw->ScreenWidth() / 23, fw->ScreenHeight() / 45), fw->sprCrappyBirdTitle.get(), 1);

	// Flap with timer
	Time_Now = std::chrono::high_resolution_clock::now();
	Time_Duration = Time_Now - Time_Then;
	fw->SetPixelMode(olc::Pixel::ALPHA);
	if (Time_Duration.count() < .6f)
	{
		fw->DrawSprite(olc::vi2d(fBirdX, fBirdY), fw->sprBirdGlide.get(), 1, bFlipBird);
	}
	else if (Time_Duration.count() < .61f)
	{
		auto FlapSoundStatus = fw->FlapSound.getStatus();
		if (FlapSoundStatus != sf::SoundSource::Status::Playing)
		{
			fw->FlapSound.play();
		}
		fw->DrawSprite(olc::vi2d(fBirdX, fBirdY), fw->sprBirdFlap.get(), 1, bFlipBird);

	}
	else if (Time_Duration.count() < .8f)
	{
		fw->DrawSprite(olc::vi2d(fBirdX, fBirdY), fw->sprBirdFlap.get(), 1, bFlipBird);
	}
	else if (Time_Duration.count() > .8f)
	{
		Time_Then = Time_Now;
		Time_Duration.zero();
	}

	fw->SetPixelMode(olc::Pixel::MASK);

	// play title sound, with timer
	Time_Duration_TitleVoice = Time_Now - Time_Then_TitleVoice;

	if (Time_Duration_TitleVoice.count() < 2.f)
	{
	}
	else if (Time_Duration_TitleVoice.count() > 2)
	{
		Time_Then_TitleVoice = Time_Now;
		if (bPlayVoice)
		{
			std::call_once(onceFlag, [&] { fw->FulappiBado.play(); });
			bInitPoop = true;
		}
		bPlayVoice = true;
		Time_Duration_TitleVoice.zero();
	}
	// Poop, started simultaneously with title voice being played. bInitPoop.
	Time_Duration_Poop = Time_Now - Time_Then_Poop;
	if (Time_Duration_Poop.count() < 6.f && bInitPoop)
	{
		if (bStartPoop)
		{
			fPoopY = fBirdY + 16.f;
			bStartPoop = false;
			fw->BadoPoop.play();
		}
		fPoopY += fPoopVelocity * fw->GetElapsedTime();
		fw->DrawDecal(olc::vi2d(fBirdX + 20, fPoopY), fw->decBirdPoop.get(), olc::vf2d(.3f, .8f));
		if (fPoopY > fw->ScreenHeight() - 8)
		{
			if (bPoopHit)
			{
				fPoopHitX = fBirdX + 20;
				for (int i = 0; i < 55; ++i)
				{
					sPoopFragments f;
					f.Pos = { fPoopHitX, (float)fw->ScreenHeight() - 10.f };
					float fAngle = float(rand()) / float(RAND_MAX) * 2.f * 3.14159f;
					float fVelocity = float(rand()) / float(RAND_MAX) * 44.f;
					f.Vel = { fVelocity * cos(fAngle), fVelocity * sin(fAngle) };
					f.fAngle = fAngle;
					f.fTime = fFragmentLifeTime;
					f.Color = olc::WHITE;
					ListPoopFragments.push_back(f);
					fw->BadoSplat.play();
				}
				bPoopHit = false;
			}
			fw->DrawDecal(olc::vi2d(fPoopHitX - 8, fw->ScreenHeight() - 12), fw->decBirdPoop.get(), olc::vf2d(2.f, 1.f), PoopHitColor);

			// Update poop fragments
			for (auto& f : ListPoopFragments)
			{
				f.Vel += olc::vf2d(0.f, 100.f) * fElapsedTime;
				f.Pos -= f.Vel * fElapsedTime;
				f.fAngle += 5.f * fElapsedTime;
				f.fTime -= fElapsedTime;
				f.Color.a = f.fTime / fFragmentLifeTime * 255;
			}
			ListPoopFragments.erase(
				std::remove_if(ListPoopFragments.begin(), ListPoopFragments.end(),
					[](const sPoopFragments& f)
			{ return f.fTime < 0.0f; }),
				ListPoopFragments.end()
			);
		}
	}
	else if (Time_Duration_Poop.count() > 3.f && bInitPoop)
	{
		Time_Then_Poop = Time_Now;
		bStartPoop = true;
		bPoopHit = true;
		Time_Duration_Poop.zero();
	}
	for (auto& F : ListPoopFragments)
	{
		fw->DrawRotatedDecal(F.Pos, fw->decBirdPoop.get(), F.fAngle, { 4,4 }, { .3f,.7f }, F.Color);
	}
}

void Menu::Options(FlappyWorld* fw)
{
	if (bDisplayCredits)
	{
		int StringWidth = fw->ScreenWidth() / 25;
		int StringHeight1 = (int)(fw->ScreenHeight() / 2.1f);
		int StringHeight2 = (int)(fw->ScreenHeight() / 1.5f);
		int StringHeight3 = (int)(fw->ScreenHeight() / 1.3f);

		fw->DrawString({ StringWidth , StringHeight1 }, "Made by Jonas Bech-Sorensen", olc::BLACK, 1);
		fw->DrawString({ StringWidth , StringHeight2 }, "Graphics Engine: olcPixelGameEngine by OneLoneCoder", olc::BLACK, 1);
		fw->DrawString({ StringWidth , StringHeight3 }, "Audio engine: SFML", olc::BLACK, 1);

		if (fw->GetKey(olc::ENTER).bPressed || fw->GetKey(olc::SPACE).bPressed || fw->GetMouse(0).bPressed)
		{
			bDisplayCredits = false;
			bHideScores = false;
		}
	}
	else
	{
		int StringWidth = fw->ScreenWidth() / 7;
		int StringHeightPlay = (int)(fw->ScreenHeight() - 120);
		int StringHeightCredits = (int)(fw->ScreenHeight() - 95);
		int StringHeightQuit = (int)(fw->ScreenHeight() - 70);

		// mouse over Play
		if (fw->GetMouseX() > StringWidth && fw->GetMouseX() < StringWidth + 70
			&& fw->GetMouseY() > StringHeightPlay - 4 && fw->GetMouseY() < StringHeightPlay + 18)
		{
			_MenuSelection = MenuSelection::Play;
			bMouseOverMenuItem = true;
		}
		// mouse over Credits
		else if (fw->GetMouseX() > StringWidth && fw->GetMouseX() < StringWidth + 70
			&& fw->GetMouseY() > StringHeightCredits - 4 && fw->GetMouseY() < StringHeightCredits + 18)
		{
			_MenuSelection = MenuSelection::Credits;
			bMouseOverMenuItem = true;
		}
		// mouse over Quit
		else if (fw->GetMouseX() > StringWidth && fw->GetMouseX() < StringWidth + 70
			&& fw->GetMouseY() > StringHeightQuit - 4 && fw->GetMouseY() < StringHeightQuit + 18)
		{
			_MenuSelection = MenuSelection::Quit;
			bMouseOverMenuItem = true;
		}
		else
		{
			bMouseOverMenuItem = false;

		}
		if (fw->GetKey(olc::DOWN).bPressed || fw->GetKey(olc::S).bPressed)
		{
			switch (_MenuSelection)
			{
			case MenuSelection::Play:
				_MenuSelection = MenuSelection::Credits;
				break;
			case MenuSelection::Credits:
				_MenuSelection = MenuSelection::Quit;
				break;
			case MenuSelection::Quit:
				break;
			case MenuSelection::None:
				_MenuSelection = MenuSelection::Play;
				break;
			default:
				break;
			}
		}
		if (fw->GetKey(olc::UP).bPressed || fw->GetKey(olc::W).bPressed)
		{
			switch (_MenuSelection)
			{
			case MenuSelection::Play:
				break;
			case MenuSelection::Credits:
				_MenuSelection = MenuSelection::Play;
				break;
			case MenuSelection::Quit:
				_MenuSelection = MenuSelection::Credits;
				break;
			case MenuSelection::None:
				_MenuSelection = MenuSelection::Play;

			default:
				break;
			}
		}
		switch (_MenuSelection)
		{
		case Menu::MenuSelection::Play:
			fTextSizePlay = 2.f;
			fTextSizeCredits = 1.f;
			fTextSizeQuit = 1.f;
			break;
		case Menu::MenuSelection::Credits:
			fTextSizePlay = 1.f;
			fTextSizeCredits = 2.f;
			fTextSizeQuit = 1.f;
			break;
		case Menu::MenuSelection::Quit:
			fTextSizePlay = 1.f;
			fTextSizeCredits = 1.f;
			fTextSizeQuit = 2.f;
			break;
		case Menu::MenuSelection::None:
			fTextSizePlay = 1.f;
			fTextSizeCredits = 1.f;
			fTextSizeQuit = 1.f;

		default:
			break;
		}
		if (fw->GetKey(olc::ENTER).bPressed || fw->GetKey(olc::SPACE).bPressed || (fw->GetMouse(0).bPressed && bMouseOverMenuItem))
		{
			switch (_MenuSelection)
			{
			case MenuSelection::Play:
				fw->_GameState->SetGameState(State::Playing);
				break;
			case MenuSelection::Credits:
				bDisplayCredits = true;
				bHideScores = true;
				break;
			case MenuSelection::Quit:
				fw->olc_Terminate();
				break;
			default:
				break;
			}
		}
		// draw menu entries
		fw->DrawString({ StringWidth , StringHeightPlay }, "PLAY", olc::BLACK, fTextSizePlay);
		fw->DrawString({ StringWidth, StringHeightCredits }, "CREDITS", olc::BLACK, fTextSizeCredits);
		fw->DrawString({ StringWidth, StringHeightQuit }, "QUIT", olc::BLACK, fTextSizeQuit);
	}
}

void Menu::HighScores(FlappyWorld* fw)
{
	if (!bHideScores)
	{
		if (bCalcScores)
		{
			Savegame savegame;
			auto vScores = savegame.LoadScores();
			if (vScores == nullptr) 
			{
				std::cout << "Vscores nullptr";
				return;
			}
			switch (vScores->size())
			{
			case 0:
				break;
			case 1:
				Score0 = std::to_string((*vScores)[0].Score);
				Name0 = (*vScores)[0].Name;
				break;
			case 2:
				Score0 = std::to_string((*vScores)[0].Score);
				Score1 = std::to_string((*vScores)[1].Score);
				Name0 = (*vScores)[0].Name;
				Name1 = (*vScores)[1].Name;
				break;
			case 3:
				Score0 = std::to_string((*vScores)[0].Score);
				Score1 = std::to_string((*vScores)[1].Score);
				Score2 = std::to_string((*vScores)[2].Score);
				Name0 = (*vScores)[0].Name;
				Name1 = (*vScores)[1].Name;
				Name2 = (*vScores)[2].Name;
				break;
			case 4:
				Score0 = std::to_string((*vScores)[0].Score);
				Score1 = std::to_string((*vScores)[1].Score);
				Score2 = std::to_string((*vScores)[2].Score);
				Score3 = std::to_string((*vScores)[3].Score);
				Name0 = (*vScores)[0].Name;
				Name1 = (*vScores)[1].Name;
				Name2 = (*vScores)[2].Name;
				Name3 = (*vScores)[3].Name;
				break;
			case 5:
				Score0 = std::to_string((*vScores)[0].Score);
				Score1 = std::to_string((*vScores)[1].Score);
				Score2 = std::to_string((*vScores)[2].Score);
				Score3 = std::to_string((*vScores)[3].Score);
				Score4 = std::to_string((*vScores)[4].Score);
				Name0 = (*vScores)[0].Name;
				Name1 = (*vScores)[1].Name;
				Name2 = (*vScores)[2].Name;
				Name3 = (*vScores)[3].Name;
				Name4 = (*vScores)[4].Name;
				break;
			default:
				Score0 = std::to_string((*vScores)[0].Score);
				Score1 = std::to_string((*vScores)[1].Score);
				Score2 = std::to_string((*vScores)[2].Score);
				Score3 = std::to_string((*vScores)[3].Score);
				Score4 = std::to_string((*vScores)[4].Score);
				Name0 = (*vScores)[0].Name;
				Name1 = (*vScores)[1].Name;
				Name2 = (*vScores)[2].Name;
				Name3 = (*vScores)[3].Name;
				Name4 = (*vScores)[4].Name;
				break;
			}
			bCalcScores = false;
		}

		// frame
		fw->DrawRect({ fw->ScreenWidth() / 2 + 20 + WidthOffset, fw->ScreenHeight() / 2 - 30 }, { 120,130 }, olc::BLACK);
		// title line
		fw->DrawLine({ fw->ScreenWidth() / 2 + 20 + WidthOffset, fw->ScreenHeight() / 2 - 10 },
			{ fw->ScreenWidth() / 2 + 140 + WidthOffset, fw->ScreenHeight() / 2 - 10 }, olc::BLACK);
		// mid line
		fw->DrawLine({ fw->ScreenWidth() / 2 + 130, fw->ScreenHeight() / 2 - 10 },
			{ fw->ScreenWidth() / 2 + 130, fw->ScreenHeight() / 2 + 100 }, olc::BLACK);

		// title
		fw->DrawString({ fw->ScreenWidth() / 2 + 85, fw->ScreenHeight() / 2 - 23 }, "Top 5", olc::BLACK);

		// names
		// 1
		fw->DrawString({ fw->ScreenWidth() / 2 + 53, fw->ScreenHeight() / 2 - 0 }, Name0, olc::BLACK);
		// 2
		fw->DrawString({ fw->ScreenWidth() / 2 + 53, fw->ScreenHeight() / 2 + 21 }, Name1, olc::BLACK);
		// 3
		fw->DrawString({ fw->ScreenWidth() / 2 + 53, fw->ScreenHeight() / 2 + 42 }, Name2, olc::BLACK);
		// 4
		fw->DrawString({ fw->ScreenWidth() / 2 + 53, fw->ScreenHeight() / 2 + 63 }, Name3, olc::BLACK);
		// 5
		fw->DrawString({ fw->ScreenWidth() / 2 + 53, fw->ScreenHeight() / 2 + 84 }, Name4, olc::BLACK);

		// Scores
		// 1
		fw->DrawString({ fw->ScreenWidth() / 2 + 109 + WidthOffset, fw->ScreenHeight() / 2 - 0 }, Score0, olc::BLACK);
		// 2
		fw->DrawString({ fw->ScreenWidth() / 2 + 109 + WidthOffset, fw->ScreenHeight() / 2 + 21 }, Score1, olc::BLACK);
		// 3
		fw->DrawString({ fw->ScreenWidth() / 2 + 109 + WidthOffset, fw->ScreenHeight() / 2 + 42 }, Score2, olc::BLACK);
		// 4
		fw->DrawString({ fw->ScreenWidth() / 2 + 109 + WidthOffset, fw->ScreenHeight() / 2 + 63 }, Score3, olc::BLACK);
		// 5
		fw->DrawString({ fw->ScreenWidth() / 2 + 109 + WidthOffset, fw->ScreenHeight() / 2 + 84 }, Score4, olc::BLACK);
	}
}

void Menu::SetCalcScores(bool state)
{
	bCalcScores = state;
}
