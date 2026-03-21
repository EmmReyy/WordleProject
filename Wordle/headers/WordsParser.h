#pragma once
#include <string>
#include <vector>
class WordsParser
{
private:
	std::vector<std::string> words;

public:

	WordsParser(std::string dir);

	void parseFile(std::string dir);

	std::vector<std::string>& getWords();
};

