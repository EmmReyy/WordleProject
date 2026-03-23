#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "GameLogic.h"

//constructor for the class, sets the variables for the list of words, and the letter limits
GameLogic::GameLogic(int letterLimit, std::vector<std::string>& words) {
	GameLogic::words = words;
	GameLogic::letterLimit = letterLimit;
}

//pick a random word to make players guess
std::string GameLogic::pickWord() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));		//seeding
	int chosenWordNdx = (std::rand() % GameLogic::words.size()-1);	//generate the last a number between zero and the last index of the list of words
	return GameLogic::words[chosenWordNdx];							//returns the chosen word
}

//checks the guess for correct letters and position

/*
We use a notation system where there is an array with a fixed size:

{0, 0, 0, 0, 0}

each item in the array is a stand in for a letter, where 0 = GRAY, 1 = YELLOW, and 2 = GREEN

so if the correct word is:

RAISE

and the user guessed 

PHASE

the notation for that is 

{0, 0, 1, 2, 2}

*/
int* GameLogic::checkGuess(std::string guess, std::string chosenWord) {

	//extra precaution, checks if the guess meets required length
	if (guess.size() != letterLimit) {
		std::cerr << "Guesses can only be "<< letterLimit <<" letter!" << std::endl;
		return 0;
	}

	int* guessNotation = new int[5] {0, 0, 0, 0, 0}; //the initial array that represent notation (which is to say, the correctness of the guess)

	//if the guess matches the chosenWord, immediately returns a notation of {2,2,2,2,2}
	if (guess == chosenWord){
		for (int i = 0; i < 5; i++) {
			guessNotation[i] = 2;
		}
		return guessNotation;
	}

	for (int i = 0; i < letterLimit; i++) {
		//if the character of the guess matches the position of the same character of the correct word, mark it green
		if (guess[i] == chosenWord[i]) {
			guessNotation[i] = 2;
		}
		//if the letter of the guess is present in the correct word but the position is wrong, mark it yellow
		else if (chosenWord.find(guess[i]) != std::string::npos && guess[i] != chosenWord[i])  {
			guessNotation[i] = 1;
		}
		//if the leter is not present in the correct word, save it to grey letters list
		else if (chosenWord.find(guess[i]) != std::string::npos) {
			greyed.push_back(guess[i]);
		}
	}

	return guessNotation;
}

//return the score of the player
float GameLogic::getAccuracy(int rounds, int wins) {
	return std::round((static_cast<float>(wins) / rounds) * 1000.0f) / 10.0f;	//returns accuracy in percent; multiplied by 1000.0 and divided by 10.0 to set decimal point to one place only
}