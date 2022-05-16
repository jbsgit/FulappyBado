#pragma once
#include <string>
#include <vector>
class Savegame
{
public:
	struct sScores
	{
		std::string Name;
		int Score;

		bool operator < (const sScores& rhs)
		{
			return this->Score > rhs.Score;
		}
	};
	std::vector<sScores>* LoadScores();
	std::pair<int, size_t> GetMiniumScore();
	void SaveScore(std::string Name, int Score);

private:
	std::vector<sScores> vScores;
};

