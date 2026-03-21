#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "GameLogic.h"


GameLogic::GameLogic(int letterLimit, std::vector<std::string>& words) {
	GameLogic::words = words;
	GameLogic::chosenWord = pickWord();
	GameLogic::letterLimit = letterLimit;

	std::cout << "The word is: " << chosenWord << std::endl;
}

std::string GameLogic::pickWord() {

	//seeding
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	//generate the last a number between zero and the last index of the list of words
	int chosenWordNdx = (std::rand() % GameLogic::words.size()-1);

	return GameLogic::words[chosenWordNdx];
}

int* GameLogic::checkGuess(std::string guess) {

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
	}

	return guessNotation;
}