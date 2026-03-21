#include "WordsParser.h"
#include <fstream>
#include <iostream>

//constructor for the class, sets the directory of the words file
WordsParser::WordsParser(std::string dir) {
	parseFile(dir);
}

//reads out the file, line by line, and stores each word in a list (a type of array)
void WordsParser::parseFile(std::string dir) {
	
	std::ifstream wordsFile(dir);

	if (!wordsFile.is_open()) {
		std::cerr << "No words file was opened" << std::endl;
		return;
	}

	std::string word;

	while (std::getline(wordsFile, word)) {
		WordsParser::words.push_back(word);
	}

	wordsFile.close();
}

//returns the list of words
std::vector<std::string>& WordsParser::getWords(){
	return WordsParser::words;
}
