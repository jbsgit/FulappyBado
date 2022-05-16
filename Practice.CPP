#include "Practice.h"
using namespace std;

Practice::Practice()
{
	sAppName = "Practice";
}

Practice::~Practice()
{
}

bool Practice::OnUserCreate()
{
	return true;
}

bool Practice::OnUserUpdate(float fElapsedTime)
{
	for (int X = 0; X < ScreenWidth(); X++)
	{
		for (int Y = 0; Y < ScreenHeight(); Y++)
		{
			Draw(X, Y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256, 255));
		}
	}
	return true;
}

void Practice::PracticeTest()
{
	//std::cout << "Testing 1 2 3";

}
