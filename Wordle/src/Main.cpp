#include <iostream>
#include <string>
#include "WordsParser.h"	//header for my WordsParse class, which read the text file
#include <vector>
#include "GameLogic.h"		//header for my GameLogic class, which handles the rules of the game


void notationPrinter(int* notation, std::string guess);	//prints out each character of the guess, depending on the notation
bool guessed(int* notation);	//for checking if the notation indicates a correct answer
void clearInputLine();	//helper function that clears inputs from terminal
void clearScreen();		//helper function to clear the terminal
void capsLock(std::string& str);		//converts string to capital
const int limit = 5;	//hard limit set to 5 letters, can be changed in future updates

void capsLock(std::string& str) {

	for (char &c : str) {
		c = std::toupper(c);
	}
}

void clearInputLine() {
	// move the cursor up one line, clear the entire line, and return the cursor to the start
	std::cout << "\x1b[1A" << "\x1b[2K" << "\r";
	// clears the out stream so next input is sent to terminal
	std::cout.flush(); 
}

void clearScreen() {
	// ANSI escape codes to clear screen and move cursor to home position
	std::cout << "\033[2J\033[H";
	// clears the out stream so next input is sent to terminal
	std::cout.flush();
}

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
	//creates an object of WordParser class
	WordsParser wp = WordsParser("C:\\Users\\gobbl\\source\\repos\\Wordle\\Wordle\\WORDS");
	//stores the list of words given by Parser object
	std::vector<std::string> words = wp.getWords();

	//create an object of GameLogic class
	GameLogic gl = GameLogic(limit, words);

	int counter = 0;
	std::string correctWord = gl.pickWord();
	capsLock(correctWord);

	while (true) {

		//FOR TESTING ONLY
		if (counter == 0)std::cout << "The word is: " << correctWord << std::endl;


		std::string guess;
		int* notation;
		std::getline(std::cin, guess);
		capsLock(guess);
		clearInputLine();

		std::cout << std::endl;



		//checks if input meets letter limit requirement
		if (guess.size() != limit) {
			std::cout << "Guesses should be " << limit << " letters long!" << std::endl;
			continue;
		}

		//saves the notation based on the guess
		notation = gl.checkGuess(guess, correctWord);
		
		//prints the gues with the proper font colors based on notation
		notationPrinter(notation, guess);

		//if the guess matches correct word, exit out of loop
		if (guessed(notation)) {
			break;
		}

		//keep track of how many turns have passed
		counter++;

		//if there have been 6 turns, automatically fail and print the correct answer
		if (counter == 6) {

			for (char c : correctWord) {
				c = std::toupper(c);
			}

			std::cout << "The correct word is: " << correctWord << std::endl;
			std::cout << "Play again? " << std::endl;

			std::string resp;
			std::getline(std::cin, resp);
			capsLock(resp);

			if (resp == "YES") {
				clearScreen();
				correctWord = gl.pickWord();
				counter = 0;
				continue;
			}
			else {
				break;
			}
			
		}
	}

	return 0;
}
