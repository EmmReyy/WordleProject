#include <iostream>
#include <string>
#include "WordsParser.h"	//header for my WordsParse class, which read the text file
#include <vector>
#include "GameLogic.h"		//header for my GameLogic class, which handles the rules of the game


void notationPrinter(int* notation, std::string guess);	//prints out each character of the guess, depending on the notation
bool guessed(int* notation);	//for checking if the notation indicates a correct answer
const int limit = 5;	//hard limit set to 5 letters, can be changed in future updates


void notationPrinter(int* notation, std::string guess) {
	
	for (int i = 0; i < limit; i++) {
		
		/*if the notation is 0 that means the word does not contain the letter,
		prints out the letter greyed out*/
		if (notation[i] == 0) {
			std::cout << "|\033[90m" << guess[i] << "\033[0m";
		}
		/*if the notation at the letter's position is 1, that means the correct word contains the letter but
		the position is wrong*/ 
		else if (notation[i] == 1) {
			std::cout << "|\033[33m" << guess[i] << "\033[0m";
		}
		/*if the notation is at 2, that means the letter is present in the correct word and the position is correct*/
		else if (notation[i] == 2) {
			std::cout << "|\033[32m" << guess[i] << "\033[0m";
		}
	}

	std::cout << "|" << std::endl;
}

bool guessed(int* notation) {
	for (int i = 0; i < limit; i++) {
		if (notation[i] != 2) {
			return false;
		}
	}

	return true;
}

int main() {
	WordsParser wp = WordsParser("C:\\Users\\gobbl\\source\\repos\\Wordle\\Wordle\\WORDS");
	std::vector<std::string> words = wp.getWords();

	GameLogic gl = GameLogic(5, words);


	while (true) {

		std::cout << "Enter your guess: ";
		std::string guess;
		int* notation;
		std::getline(std::cin, guess);
		std::cout << std::endl;

		notation = gl.checkGuess(guess);
		
		notationPrinter(notation, guess);

		if (guessed(notation)) {
			break;
		}
	}

	return 0;
}
