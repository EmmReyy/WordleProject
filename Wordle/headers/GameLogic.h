#pragma once
#include <string>
#include <vector>

/*The GameLogic class handles the rulse of the game, such as picking a correct word at random, 
checking if the guessed word is correct, and generating notation for guesses*/

class GameLogic {

private:
	std::vector<std::string> words;									//list of words
	std::vector<char> greyed;										//list of letters not included in the word
	int letterLimit;												//hard limit to the number of letters				

public:
	GameLogic(int letterLimit, std::vector<std::string>& words);	//constuctor, sets the letter limit and the word list
	
	std::string pickWord();											//randomly chooses the worrd to guess

	int* checkGuess(std::string guess, std::string chosenWord);		//checks the user guess and returns the notation for the guess

	float getAccuracy(int rounds, int wins);						//gets the accuracy (score) 
};
