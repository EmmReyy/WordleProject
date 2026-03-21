#pragma once
#include <string>
#include <vector>

class GameLogic {

private:
	std::vector<std::string> words;
	std::vector<char> greyed;
	std::string chosenWord;
	int letterLimit;


public:
	GameLogic(int letterLimit, std::vector<std::string>& words);
	
	std::string pickWord();

	int* checkGuess(std::string guess);
};
