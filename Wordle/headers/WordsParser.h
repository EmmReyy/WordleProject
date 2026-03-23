#pragma once
#include <string>
#include <vector>

/*The WordsParser class is responsible for reading the text file of words and stroign each word in a
list
*/

class WordsParser
{
private:
	std::vector<std::string> words;											//list of parsed common words
	std::vector<std::string> completeWords;									//list of parsed complete words

public:


	WordsParser(std::string dir, std::string completeDir);					//constructor for class, sets the directory of the file

	void parseFile(std::string dir, std::vector<std::string>& listWords);	//parses the text line by line and savess to words list

	std::vector<std::string>& getWords();									//returns the word list

	std::vector<std::string>& getCompleteWords();									//returns the complete word list

	void capsLock(std::string& str);										//capitalises words
};

