#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "GameLogic.h"

//constructor for the class, sets the variables for the list of words, and the letter limits
GameLogic::GameLogic(int letterLimit, std::vector<std::string>& words, std::vector<std::string>& completeWords ) {
	GameLogic::words = words;
	GameLogic::completeWords = completeWords;
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

	//TODO: Figure out what cause bug where some grey letters are printed green in keyboard
	//for now, redundant checks are in place to prevent the bug
	for (int i = 0; i < letterLimit; i++) {
		//if the character of the guess matches the position of the same character of the correct word, mark it green
		if (guess[i] == chosenWord[i]) {
			guessNotation[i] = 2;

			//remove from yellow
			yellow.erase(std::remove(yellow.begin(), yellow.end(), guess[i]), yellow.end());

			//remove from grey
			greyed.erase(std::remove(greyed.begin(), greyed.end(), guess[i]), greyed.end());

			//add to green if not already there
			if (std::find(green.begin(), green.end(), guess[i]) == green.end()) {
				green.push_back(guess[i]);
			}
		}
		//if the letter of the guess is present in the correct word but the position is wrong, mark it yellow
		else if (chosenWord.find(guess[i]) != std::string::npos && guess[i] != chosenWord[i])  {
			guessNotation[i] = 1;

			//only if not already green
			if (std::find(green.begin(), green.end(), guess[i]) == green.end()) {

				//remove from grey
				greyed.erase(std::remove(greyed.begin(), greyed.end(), guess[i]), greyed.end());

				//add if not already yellow
				if (std::find(yellow.begin(), yellow.end(), guess[i]) == yellow.end()) {
					yellow.push_back(guess[i]);
				}
			}
		}
		//if the leter is not present in the correct word, save it to grey letters list
		else if (chosenWord.find(guess[i]) == std::string::npos) {
			//only add to grey vector if not present in yellow or greens
			if (std::find(green.begin(), green.end(), guess[i]) == green.end() &&
				std::find(yellow.begin(), yellow.end(), guess[i]) == yellow.end()) {

				if (std::find(greyed.begin(), greyed.end(), guess[i]) == greyed.end()) {
					greyed.push_back(guess[i]);
				}
			}
		}
	}

	return guessNotation;
}

//checks if the guess is a valid word
bool GameLogic::isGuessValid(std::string guess) {
	return (std::binary_search(completeWords.begin(), completeWords.end(), guess));

}

//return the score of the player
float GameLogic::getAccuracy(int rounds, int wins) {
	return std::round((static_cast<float>(wins) / rounds) * 1000.0f) / 10.0f;	//returns accuracy in percent; multiplied by 1000.0 and divided by 10.0 to set decimal point to one place only
}

//returns guessed grey letters
std::vector<char> GameLogic::getGreys() {
	return GameLogic::greyed;
}

//returns guessed green letters
std::vector<char> GameLogic::getGreens() {
	return GameLogic::green;
}

//returns guessed yellow letters
std::vector<char> GameLogic::getYellows() {
	return GameLogic::yellow;
}