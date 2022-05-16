#include "Savegame.h"
#include <iostream>
#include <fstream>
#include <optional>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>

std::vector<Savegame::sScores>* Savegame::LoadScores()
{
	/* read file */
	std::ifstream ifs("hs.sc");
	std::string InLine;
	int InInt;

	std::string Name;
	int Score;

	// clear vScores before refreshing it with new entries.
	vScores.clear();

	if (ifs)
	{
		while (std::getline(ifs, InLine, '\n'))
		{
			std::stringstream linestream(InLine);
			std::getline(linestream, Name, ',');
			linestream >> Score;
			vScores.push_back({ Name, Score });
		}
	}
	// sort scores
	std::sort(vScores.begin(), vScores.end());
	return &vScores;
}

// find lowest score of the top 5 scores, and find size of vScores
std::pair<int, size_t> Savegame::GetMiniumScore()
{
	if (LoadScores() == nullptr) 
	{
		std::cout << "LoadScores() == nullptr";
		return std::pair<int, size_t>{-1, -1};
	}
	int LowestScore = 0;
	auto Scores = LoadScores();
	if (Scores->size() > 0)
	{
		LowestScore = (*Scores)[0].Score;
		for (int i = 0; i < 5; ++i) // loop thru the top 5.
		{
			if ((*Scores)[i].Score < LowestScore)
			{
				LowestScore = (*Scores)[i].Score;
			}
		}
	}
	std::pair<int, size_t> LowestAndSize{ LowestScore, (*Scores).size() };
	return LowestAndSize;
}


void Savegame::SaveScore(std::string Name, int Score)
{
	Savegame::vScores.push_back({ Name,Score });
	std::ofstream ofs("hs.sc", std::ofstream::app);
	if (ofs)
	{
		for (auto i : vScores)
		{
			ofs << i.Name << ",";
			ofs << i.Score << "\n";
		}
	}
}

