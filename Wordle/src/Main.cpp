#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <conio.h>
#include <windows.h>
#include <filesystem>
#include "WordsParser.h"	//header for my WordsParse class, which read the text file
#include <vector>
#include "GameLogic.h"		//header for my GameLogic class, which handles the rules of the game


void notationPrinter(int* notation, std::string guess);	//prints out each character of the guess, depending on the notation
bool guessed(int* notation);	//for checking if the notation indicates a correct answer
void clearInputLine();	//helper function that clears inputs from terminal
void clearScreen();		//helper function to clear the terminal
void capsLock(std::string& str);		//converts string to capital
const int limit = 5;	//hard limit set to 5 letters, can be changed in future updates
void displayAccuracy(float accuracy);		//for dsiplaying the players succes rate after the game
void tempPrinter(const std::string& msg, int milliseconds);		//for printin output that disappears after a set time
bool enableANSI();		//enabe ANSI escape codes, allows for colored fonts, handled in separate fucntion
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

void displayAccuracy(float accuracy) {
	std::cout << "You acuracy is " << accuracy << "%, ";
	
	if (accuracy == 100) std::cout << "Flawless!" << std::endl;
	else if (accuracy >= 95.0 && accuracy < 100) std::cout << "Impeccable!" << std::endl;
	else if (accuracy >= 90.0 && accuracy < 95.0) std::cout << "Impressive!" << std::endl;
	else if (accuracy >= 80.0 && accuracy < 90.0) std::cout << "Nicely Played!" << std::endl;
	else if (accuracy >= 70.0 && accuracy < 80.0) std::cout << "Well Done!" << std::endl;
	else if (accuracy >= 60.0 && accuracy < 70.0) std::cout << "Passable!" << std::endl;
	else if (accuracy < 60.0) std::cout << "Do Better Next Time!" << std::endl;
}

bool guessed(int* notation) {
	for (int i = 0; i < limit; i++) {
		if (notation[i] != 2) {
			return false;
		}
	}

	return true;
}

void tempPrinter(const std::string& msg, int milliseconds) {
	std::cout << msg << std::endl;
	std::cout.flush();

	int elapsed = 0;
	const int interval = 50; // check every 50ms

	while (elapsed < milliseconds) {
		if (_kbhit()) { // user pressed a key
			int ch = _getch(); // consume it
			if (ch == 13) {    // Enter key
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(interval));
		elapsed += interval;
	}

	// Clear the line
	std::cout << "\x1b[1A" << "\x1b[2K" << "\r";
	std::cout.flush();
}

bool enableANSI() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) return false;

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) return false;

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) return false;

	return true;
}


int main(int argc, char* argv[]) {

	//FOR TESTING; checks if ANSI is enabled properly
	if (!enableANSI()) {
		std::cerr << "WARNING: ANSI escape sequences may not work in this terminal.\n";
	}

	//gets the directory of the words file, relative to the output directory
	std::filesystem::path exeDir = std::filesystem::path(argv[0]).parent_path();
	std::filesystem::path wordsPath = exeDir / "WORDS";
	
	//creates an object of WordParser class
	WordsParser wp(wordsPath.string());

	//stores the list of words given by Parser object
	std::vector<std::string> words = wp.getWords();

	//create an object of GameLogic class
	GameLogic gl = GameLogic(limit, words);

	//is the counter for how many turns player has taken
	int counter = 0;
	//for sotring how many games the player played
	int rounds = 0;
	//for storing how many games the player won
	int wins = 0;
	//is the word that the player is guessing
	std::string correctWord = gl.pickWord();
	//converts the word to all uppercase
	capsLock(correctWord);
	
	std::cout << "===========================COMMAND LINE WORDLE===========================" << std::endl;
	tempPrinter("[1] Guess the word in 6 turns or less!", 2000);
	tempPrinter("[2] Clues will come in the form of font color of your guess' letters!", 2000);
	tempPrinter("[3] Grey means the correct word does not have this letter.", 2000);
	tempPrinter("[4] Yellow means the correct word does have this letter, but it's in the wrong position!", 2000);
	tempPrinter("[5] Green means the correct word has this letter and its in the right position!", 2000);
	tempPrinter("[6] Enter your guess to begin the game!", 2000);

	//the game loop
	while (true) {
		//the guess input by the user
		std::string guess;
		//is the array notation for the guess (pointer)
		int* notation;
		//reads user input
		std::getline(std::cin, guess);
		//capitalizes input, to match the correct word
		capsLock(guess);
		//removes the echo of user input, so that input guesses wont stick around
		clearInputLine();

		std::cout << std::endl;

		if (guess == "HINT PLS") {
			tempPrinter(correctWord + " is the answer!", 750);
			continue;
		}

		//checks if input meets letter limit requirement
		if (guess.size() != limit) {
			std::string msg = "Guesses should be " + std::to_string(limit) + " letters long!";
			tempPrinter(msg, 1000);
			continue;
		}

		//saves the notation based on the guess
		notation = gl.checkGuess(guess, correctWord);
		
		//prints the gues with the proper font colors based on notation
		notationPrinter(notation, guess);

		//if the guess matches correct word, exit out of loop
		if (guessed(notation)) {
			std::cout << "You got it! The word is: " << correctWord << std::endl;

			rounds++;
			wins++;

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
				displayAccuracy(gl.getAccuracy(rounds, wins));
				break;
			}
		}

		//keep track of how many turns have passed
		counter++;

		//if there have been 6 turns, automatically fail and print the correct answer
		if (counter == 6 ) {

			rounds++;

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
				displayAccuracy(gl.getAccuracy(rounds, wins));
				break;
			}
		}
	}
	return 0;
}


