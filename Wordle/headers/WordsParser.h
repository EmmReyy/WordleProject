#pragma once
#include <string>
#include <vector>

/*The WordsParser class is responsible for reading the text file of words and stroign each word in a
list
*/

class WordsParser
{
private:
	std::vector<std::string> words;			//list of parsed words

public:


	WordsParser(std::string dir);			//constructor for class, sets the directory of the file

	void parseFile(std::string dir);		//parses the text line by line and savess to words list

	std::vector<std::string>& getWords();	//returns the word list

	void capsLock(std::string& str);		//capitalises words
};

