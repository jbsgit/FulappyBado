#pragma once
#include <iostream>
#include "olcPixelGameEngine.h"

class Practice : public olc::PixelGameEngine
{
public:
	Practice(); // Constructor
	~Practice(); // Deconstructor
	virtual bool OnUserCreate() override;
	virtual bool OnUserUpdate(float fElapsedTime) override;
	void PracticeTest();
};