#include "WordsParser.h"
#include <fstream>
#include <iostream>

WordsParser::WordsParser(std::string dir) {
	parseFile(dir);
}

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

std::vector<std::string>& WordsParser::getWords(){
	return WordsParser::words;
}
