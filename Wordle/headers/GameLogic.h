#pragma once
#include <string>
#include <vector>

/*The GameLogic class handles the rulse of the game, such as picking a correct word at random, 
checking if the guessed word is correct, and generating notation for guesses*/

class GameLogic {

private:
	std::vector<std::string> words;									//list of common words
	std::vector<std::string> completeWords;							//list of complet words
	std::vector<char> greyed;										//list of letters not included in the word
	std::vector<char> yellow;										//list of letters included in the word
	std::vector<char> green;										//list of letters included in the word, in the correct pos
	int letterLimit;												//hard limit to the number of letters				

public:
	GameLogic(int letterLimit, std::vector<std::string>& words, std::vector<std::string>& completeWords);	//constuctor, sets the letter limit and the word list
	
	std::string pickWord();											//randomly chooses the worrd to guess

	int* checkGuess(std::string guess, std::string chosenWord);		//checks the user guess and returns the notation for the guess

	float getAccuracy(int rounds, int wins);						//gets the accuracy (score) 

	bool isGuessValid(std::string guess);							//searches the words list, checking it the guess is a valid word

	std::vector<char> getGreys();									//returns the letters not present in the correct word

	std::vector<char> getYellows();									//returns the letters present in the correct word

	std::vector<char> getGreens();									//returns the letters present in the correct word at the correc pos

	void clear();													//empties list of colored letters
};
