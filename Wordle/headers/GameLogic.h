#pragma once
#include <string>
#include <vector>

/*The GameLogic class handles the rulse of the game, such as picking a correct word at random, 
checking if the guessed word is correct, and generating notation for guesses*/

class GameLogic {

private:
	std::vector<std::string> words;
	std::vector<char> greyed;
	int letterLimit;

	bool guessed = false;

public:
	GameLogic(int letterLimit, std::vector<std::string>& words);
	
	std::string pickWord();

	int* checkGuess(std::string guess, std::string chosenWord);

};
