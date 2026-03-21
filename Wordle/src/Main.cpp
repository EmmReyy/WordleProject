#include <iostream>
#include <string>
#include "WordsParser.h"
#include <vector>
#include "GameLogic.h"

const int limit = 5;

void notationPrinter(int* notation, std::string guess) {
	
	for (int i = 0; i < limit; i++) {
		
		if (notation[i] == 0) {
			std::cout << "|" << guess[i];
		}
		else if (notation[i] == 1) {
			std::cout << "|\033[33m" << guess[i] << "\033[0m";
		}
		else if (notation[i] == 2) {
			std::cout << "|\033[32m" << guess[i] << "\033[0m";
		}
	}

	std::cout << "|" << std::endl;
}

int main() {
	WordsParser wp = WordsParser("C:\\Users\\gobbl\\source\\repos\\Wordle\\Wordle\\WORDS");
	std::vector<std::string> words = wp.getWords();

	GameLogic gl = GameLogic(5, words);


	bool guessed = false;
	while (!guessed) {

		std::cout << "Enter your guess: ";
		std::string guess;
		std::getline(std::cin, guess);
		std::cout << std::endl;

		if (gl.checkGuess(guess) == new int[5] {2, 2, 2, 2, 2}) {
			guessed = true;
		}
		
		notationPrinter(gl.checkGuess(guess), guess);

	}

	return 0;
}
