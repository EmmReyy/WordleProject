#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <conio.h>
#include <windows.h>
#include <filesystem>
#include "WordsParser.h"	
#include <vector>
#include "GameLogic.h"


//TODO: create separate clas that handles printer functions


void notationPrinter(int* notation, std::string guess);						//prints out each character of the guess, depending on the notation
bool guessed(int* notation);												//for checking if the notation indicates a correct answer
void clearPrevLines(int lines);												//helper function that clears previous lines from terminal
void clearScreen();															//helper function to clear the terminal
void capsLock(std::string& str);											//converts string to capital
const int limit = 5;														//hard limit set to 5 letters, can be changed in future updates
void displayAccuracy(float accuracy);										//for dsiplaying the players succes rate after the game
void tempPrinter(const std::string& msg, int milliseconds);					//for printin output that disappears after a set time
bool enableANSI();															//enabe ANSI escape codes, allows for colored fonts, handled in separate fucntion
bool startNewLoop();														//decides if new round will be started and updates neede variables if so; also handles win or loss conditions; hanled in separate function for convenience and readability
void printValidLetters(GameLogic gl);										//prints out all the letters, if letter is excluded it is greyed
void charNotationPrinter(char msg, int color);								//function that prints characcters (temporarily) with font color
void printRules();															//prints the intructions of how to play the game
void bufferredPrint(std::string str, int time, int notationColor);			//buffered printing effect where statements are printed char by char
void delay(int ms);															//delay actions for given amount of time; skips delay when enter is pressed

void delay(int ms) {
	int elapsed = 0;
	const int interval = 50;			//check every 50ms

	while (elapsed < ms) {
		if (_kbhit()) {					//checks if user enters 
			int ch = _getch();			//get the kry
			if (ch == 13) {				//checks if enter key
				break;					//breaks and immediately clear without waiting for time if user enters
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(interval));		//waits for the time to pass
		elapsed += interval;
	}
}

void bufferredPrint(std::string str, int time, int notationColor) {
	for (char c : str) {
		charNotationPrinter(c, notationColor);
		std::this_thread::sleep_for(std::chrono::milliseconds(time));
	}
	std::cout << std::endl;
}

void printRules() {
	bufferredPrint("[1] Guess the word in 6 turns or less!", 5, -1);
	bufferredPrint("[2] Clues will come in the form of font colors of your guess' letters!", 5, -1);
	bufferredPrint("[3] Grey means the correct word does not have this letter.", 5,-1);
	bufferredPrint("[4] Yellow means the correct word does have this letter, but it's in the wrong position!", 5, -1);
	bufferredPrint("[5] Green means the correct word has this letter and it's in the right position!", 5, -1);
	bufferredPrint("[6] Enter your guess to begin the game!", 5, -1);

	delay(5000);

	clearPrevLines(6);

	bufferredPrint("[*]Also, you can type in \"letters please\" to display valid letters or \"rules please\" to display the tutorial again", 10, -1);

	delay(2000);

	clearPrevLines(1);
}

void charNotationPrinter(char msg, int color) {
	
	switch (color)
	{

	case 0:
		std::cout << "\033[90m" << msg << "\033[0m";		//prints the message in grey
		break;
	case 1:
		std::cout << "\033[33m" << msg << "\033[0m";		//prints the message in yellow
		break;
	case 2:
		std::cout << "\033[32m" << msg << "\033[0m";		//prints the message in green
		break;

	default:
		std::cout << msg;									//prints the message plain
		break;
	}


}

void printValidLetters(GameLogic gl) {
	std::vector<char> greyed = gl.getGreys();
	std::vector<char> greens = gl.getGreens();
	std::vector<char> yellows = gl.getYellows();

	int totalDelay = 15000;             //keyboard will stay for 15 secs
	const int interval = 50;            //check every 50 ms
	int elapsed = 0;					//keep track of how much time has passe

	const char keyboard[3][10] = {		//2D array that holds the keyboard chars
		{'Q','W','E','R','T','Y','U','I','O','P'},
		{' ','A','S','D','F','G','H','J','K','L'},
		{' ',' ','Z','X','C','V','B','N','M',' '}
	};
	const int rowSizes[3] = { 10, 10, 10 };

	//outer loop to iterate through the rows
	for (int r = 0; r < 3; ++r) {
		//inner loop to iterate through every char in a row
		for (int c = 0; c < rowSizes[r]; ++c) {
			char ch = keyboard[r][c];

			//if the char is a whitespace, print white space to properly indent
			if (ch == ' ') {
				std::cout << " ";
			}
			else {
				//check if the char being iterated thorough is a greyed out letter, if yes prints it freyed
				int color = -1;

				if (std::find(greyed.begin(), greyed.end(), ch) != greyed.end()) color = 0;
				else if (std::find(yellows.begin(), yellows.end(), ch) != yellows.end()) color = 1;
				else if (std::find(greens.begin(), greens.end(), ch) != greens.end()) color = 2;

				//letterboxing and printing of chars
				std::cout << "|";
				charNotationPrinter(ch, color);
			}

			//clear cout
			std::cout.flush();
			//pause in between character prints for design
			std::this_thread::sleep_for(std::chrono::milliseconds(50)); 

			// Check Enter during printing
			if (_kbhit() && _getch() == 13) { // Enter key
				clearPrevLines(3);
				return;
			}

			//for printing out the closing boxes of the keyboard;
			//REPLACE WITH MORE ELEGANT SOLUTION
			if ((r == 2 && c == 8) || (r == 1 && c == 9) || (r == 0 && c == 9)) std::cout << "|";
		}

		std::cout << std::endl;
	}

	// Wait remaining time in small intervals to detect Enter
	while (elapsed < totalDelay) {
		if (_kbhit() && _getch() == 13) { // Enter
			clearPrevLines(3);
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(interval));
		elapsed += interval;
	}

	// Clear keyboard after delay
	clearPrevLines(3);
}

bool startNewLoop() {
	std::cout << std::endl;
	std::cout << "Play again? \n" << std::endl;

	std::string resp;
	std::getline(std::cin, resp);
	capsLock(resp);

	//checks input if user wants to play again, yes starts a new loop and any other intpu exits; also prints out the top bar
	if (resp == "YES") {
		clearScreen();
		std::cout << "===========================COMMAND LINE WORDLE===========================" << std::endl;
		return true;

	}
	else {
		//remove the prompt asking users for response and retunrs true
		clearPrevLines(3);
		return false;
	}
}

void capsLock(std::string& str) {

	//iterate through the chars in a string, converts char to uppercase
	for (char &c : str) {
		c = std::toupper(c);
	}
}

void clearPrevLines(int lines) {
	//repeats clearing of line until reaches desired amount of lines to clear
	for (int i = 0; i < lines; ++i) {
		std::cout << "\x1b[1A"		//ANSI escape code that moves the cursor up
			<< "\x1b[2K";			//escape code that clears the line
	}
	std::cout << "\r";				//resets cursor position
	std::cout.flush();				//reset the out stream
}

void clearScreen() {
	
	std::cout << "\033[2J\033[H";		//ANSI escape codes to clear screen and move cursor to starting pos
	std::cout.flush();					//clears the out stream so next input is sent to terminal
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
	//prints the final | that separates chars
	std::cout << "|" << std::endl;
}

void displayAccuracy(float accuracy) {
	std::cout << "You acuracy is " << accuracy << "%, ";
	
	//prints out a remark that depends on the users score
	if (accuracy == 100) std::cout << "Flawless!" << std::endl;
	else if (accuracy >= 95.0 && accuracy < 100) std::cout << "Impeccable!" << std::endl;
	else if (accuracy >= 90.0 && accuracy < 95.0) std::cout << "Impressive!" << std::endl;
	else if (accuracy >= 80.0 && accuracy < 90.0) std::cout << "Nicely Played!" << std::endl;
	else if (accuracy >= 70.0 && accuracy < 80.0) std::cout << "Well Done!" << std::endl;
	else if (accuracy >= 60.0 && accuracy < 70.0) std::cout << "Passable!" << std::endl;
	else if (accuracy < 60.0 && accuracy > 0) std::cout << "Do Better Next Time!" << std::endl;
	else if (accuracy == 0) std::cout << "Did you even try?" << std::endl;
}

bool guessed(int* notation) {
	//iterates through the guess' notation; will return true unless there is an incorrect char, which will cuase it to retrun false
	for (int i = 0; i < limit; i++) {
		if (notation[i] != 2) {
			return false;
		}
	}

	return true;
}

void tempPrinter(const std::string& msg, int ms) {
	std::cout << msg << std::endl;		//prints the message
	std::cout.flush();					//precaution; clears cout

	int elapsed = 0;
	const int interval = 50;			//check every 50ms

	delay(ms);

	// Clear the line
	clearPrevLines(1);
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
	std::filesystem::path completeWordsPath = exeDir / "fiveLetterWords.txt";

	WordsParser wp(wordsPath.string(), completeWordsPath.string());		//creates an object of WordParser class
	std::vector<std::string> words = wp.getWords();						//stores the list of words given by Parser object
	std::vector<std::string> completeWords = wp.getCompleteWords();
	GameLogic gl = GameLogic(limit, words, completeWords);				//create an object of GameLogic class
			
	int counter = 0;													//is the counter for how many turns player has taken	
	int rounds = 0;														//for sotring how many games the player played	
	int wins = 0;														//for storing how many games the player won	
	std::string correctWord = gl.pickWord();							//is the word that the player is guessing
	capsLock(correctWord);												//converts the word to all uppercase
	
	//initial printout of the instructions for new players
	std::cout << "===========================COMMAND LINE WORDLE===========================" << std::endl;
	printRules();

	//the game loop
	while (true) {
		
		std::string guess;					//the guess input by the user		
		int* notation;						//is the array notation for the guess (pointer)		
		std::getline(std::cin, guess);		//reads user input		
		capsLock(guess);					//capitalizes input, to match the correct word
		clearPrevLines(1);					//removes the echo of user input, so that input guesses wont stick around

		std::cout << std::endl;

		//WAS USED FOR TESTING, NOW FEATURE; displays the answer briefly
		//PURPOSEFULLY KEPT SECRET FROM USERS TO KEEP IT FAIR
		if (guess == "HELP PLS") {
			tempPrinter(correctWord + " is the answer!", 750);
			continue;
		}

		//prints out a keyboard with the greyed letters
		if (guess == "LETTERS PLEASE") {
			printValidLetters(gl);
			continue;
		}

		//prints out the rules of wordle
		if (guess == "RULES PLEASE") {
			printRules();
			continue;
		}

		//checks if input meets letter limit requirement
		if (guess.size() != limit) {
			std::string msg = "Guesses should be " + std::to_string(limit) + " letters long!";
			tempPrinter(msg, 1000);
			continue;
		}

		//check if word is valid
		if (!gl.isGuessValid(guess)) {
			tempPrinter("Your guessed isn't a valid word! Try a different word.", 1500);
			continue;
		}

		notation = gl.checkGuess(guess, correctWord);		//saves the notation based on the guess

		notationPrinter(notation, guess);					//prints the gues with the proper font colors based on notation

		//if the guess matches correct word, ask user if new game
		if (guessed(notation)) {
			std::cout << "You got it! The word is: " << correctWord << std::endl;

			rounds++;		//since the game is over, add to the number of rounds played
			wins++;			//since game ended by winning, add to the number of wins

			//asks the user for a response, then starts a new round or displays the results and ends the loop
			if (startNewLoop()) {
				correctWord = gl.pickWord();
				counter = 0;
				continue;
			}
			else {
				displayAccuracy(gl.getAccuracy(rounds, wins));
				tempPrinter("\n=========================================================================", 5000);
				break;
			}
		}

		counter++;			//keep track of how many turns have passed

		//if there have been 6 turns, automatically fail and print the correct answer
		if (counter == 6 ) {
			rounds++;		//add to the number of rounds played

			//asks the user for a response, then starts a new round or displays the results and ends the loop
			std::cout << "\nTurns up! that's too bad! " << "The correct word is: " << correctWord << "!" << std::endl;
			if (startNewLoop()) {
				correctWord = gl.pickWord();
				counter = 0;
				continue;
			}
			else {
				displayAccuracy(gl.getAccuracy(rounds, wins));
				tempPrinter("\n=========================================================================", 5000);
				break;
			}
		}
	}
	return 0;
}


