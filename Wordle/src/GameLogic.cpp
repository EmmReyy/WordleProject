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

	//seeding
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	//generate the last a number between zero and the last index of the list of words
	int chosenWordNdx = (std::rand() % GameLogic::words.size()-1);
	return GameLogic::words[chosenWordNdx];
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

	if (guess.size() != letterLimit) {
		std::cerr << "Guesses can only be "<< letterLimit <<" letter!" << std::endl;
		return 0;
	}

	int guessNotation[5] = {0,0,0,0,0};

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
		else if (chosenWord.find(guess[i]) != std::string::npos) {
			greyed.push_back(guess[i]);
		}
	}

	return guessNotation;
}


float GameLogic::getAccuracy(int rounds, int wins) {
	return std::round((static_cast<float>(wins) / rounds) * 1000.0f) / 10.0f;
}