#pragma once
#include <string>
#include <vector>

/*The WordsParser class is responsible for reading the text file of words and stroign each word in a
list
*/

class WordsParser
{
private:
	std::vector<std::string> words;

public:

	WordsParser(std::string dir);

	void parseFile(std::string dir);

	std::vector<std::string>& getWords();
};

