#include "WordsParser.h"
#include <fstream>
#include <iostream>

//constructor for the class, sets the directory of the words file
WordsParser::WordsParser(std::string dir, std::string completeDir) {
	parseFile(dir, WordsParser::words);
	parseFile(completeDir, WordsParser::completeWords);
}

//reads out the file, line by line, and stores each word in a list (a type of array)
void WordsParser::parseFile(std::string dir, std::vector<std::string>& listWords) {
	
	std::ifstream wordsFile(dir);		//opens the file; input file stream of the WORDS file; para lang to na cin except yung input galing sa file

	//checks if a file is successfully opened, prints error and exits if failed
	if (!wordsFile.is_open()) {
		std::cerr << "No words file was opened" << std::endl;
		return;
	}

	std::string word;	//placeholder variable for parsed word

	//iterates through the file, line by line, and saves each line as string and adds to the word list
	while (std::getline(wordsFile, word)) {
		capsLock(word);
		listWords.push_back(word);
	}

	//close the file
	wordsFile.close();
}

//returns the list of words
std::vector<std::string>& WordsParser::getWords(){
	return WordsParser::words;
}

//returns the complete list of words
std::vector<std::string>& WordsParser::getCompleteWords() {
	return WordsParser::completeWords;
}

//helper function to capitalise the words
void WordsParser::capsLock(std::string& str) {
	for (char& c : str) {
		c = std::toupper(c);
	}
}
