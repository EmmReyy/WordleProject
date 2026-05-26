#include "HallOfFame.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <ctime>

//constructor
HallOfFame::HallOfFame(std::string dir) {
    fileDir = dir;
    load();
}

//returns the current date
std::string HallOfFame::getDate() {
    std::time_t now = std::time(nullptr);

    std::tm localTimeStruct;
    localtime_s(&localTimeStruct, &now);

    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTimeStruct);

    return std::string(buffer);
}

//load file into list
void HallOfFame::load() {
    //list is cleared to be safe
    players.clear();

    //opens the file given the directory, input file
    std::ifstream file(fileDir);

    //checks if file is opened
    if (!file.is_open())
    {
        std::cerr << "Hall of Fame file not found: " << fileDir << std::endl;
        return;
    }

    //creates an object of struct Player
    Player p;

    //reads the file entry by entry, keeping track of the structs properties/members
    while (file >> p.id
        >> p.name
        >> p.wins
        >> p.games
        >> p.currentStreak
        >> p.bestStreak
        >> p.fastestWin
        >> p.totalAcc
        >> p.accuracy
        >> p.lastPlayed)
    {
        //the struct with the assigned memebers is pushed into list
        players.push_back(p);
    }

    //file strem is closed
    file.close();
}

// save vector to file
void HallOfFame::save() const {
    std::ofstream file(fileDir);

    //writes every Player object in the list, into the file
    for (const Player& p : players)
    {
        file << p.id << " "
            << p.name << " "
            << p.wins << " "
            << p.games << " "
            << p.currentStreak << " "
            << p.bestStreak << " "
            << p.fastestWin << " "
            << p.totalAcc << " "
            << p.accuracy << " "
            << p.lastPlayed << "\n";
    }

    file.close();
}

//generate next id for new users
int HallOfFame::generateID() const {
    int maxID = 0;

    for (const Player& p : players)
    {
        if (p.id > maxID)
            maxID = p.id;
    }

    return maxID + 1;
}

//update or add player
void HallOfFame::updatePlayer(const std::string& name, int guesses, bool win, float acc) {

    //this is what keeps track if entry is new or not. -1 means the entry in the hall of fame is new
    int index = -1;

    for (int i = 0; i < (int)players.size(); i++)
    {
        if (players[i].name == name)
        {
            index = i;
            break;
        }
    }

    //if the index is not -1 that means the current user is already saved in the hall of fame
    if (index != -1)
    {
        Player& p = players[index];

        //counts game first
        p.games++; 

        //if the player wins the round, add to the number of wins, the current streak, and update the best streak if needed
        if (win) {
            p.currentStreak++;
            p.wins++;
            if (p.bestStreak < p.currentStreak) p.bestStreak = p.currentStreak;
        }
        //if player loses, ends the current streak count
        else {
            p.currentStreak = 0;
        }

        //if the number of guesses for the round is less than the recorded best number of guesses, update the record
        if (guesses < p.fastestWin || p.fastestWin == 0)
            p.fastestWin = guesses;

        //update the total accuracy score and the rate of accuracy and update the date played
        p.totalAcc += acc;
        p.accuracy = (((p.totalAcc) / p.games) * 10.0f) / 1000.0f;
        p.lastPlayed = getDate();
    }

    //if the index is not -1, the player does not have a record in the hall of fame, program creates a new one
    else
    {
        //player struct object
        Player p;

        //set the name of the entry, the id of the player, and set the games played to 1
        p.id = generateID();
        p.name = name;
        p.games = 1;

        //if the player wins, the number of wins the streak and best streak are all initially set to 1
        if (win) {
            p.wins = 1;
            p.currentStreak = 1;
            p.bestStreak = 1;
        }
        else {
            p.wins = 0;
            p.currentStreak = 0;
            p.bestStreak = 0;
        }

        //sets values for the rest of the variables
        p.fastestWin = guesses;
        p.totalAcc = acc;
        p.accuracy = (((p.totalAcc) / p.games) * 10.0f) / 1000.0f;
        p.lastPlayed = getDate();

        players.push_back(p);
    }

    //sorts the player based on number of wins. most wins to least wins.
    std::sort(players.begin(), players.end(),
        [](const Player& a, const Player& b)
        {
            return a.wins > b.wins;
        });

    save();
}

//display leaderboard
void HallOfFame::displayHallOfFame() const {
    std::cout << "\n============ HALL OF FAME ============\n";

    for (const Player& p : players)
    {
        std::cout << p.name
            << " || Wins: " << p.wins
            << " || Games Played: " << p.games
            << " || Current Streak: " << p.currentStreak
            << " || Best Streak: " << p.bestStreak
            << " || Fastest Win (guesses): " << p.fastestWin
            << " || Accuracy: " << p.accuracy
            << " || Last: " << p.lastPlayed
            << "\n\n";
    }

    std::cout << "\n====================================\n";
}

//returns the number of lines in a file
int HallOfFame::numOfLines() {
    
    //opens the file for readinf
    std::ifstream file(fileDir, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        std::cerr << "Cannot open file: " << fileDir << std::endl;
        return -1;
    }

    //using tellg, the size of the stream of data in the file is determined
    //after that moves the read pointer at the start
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    //the buffersize is a give
    const int bufferSize = 4096;
    char buffer[bufferSize];

    //inital value of 0 for the number of lines
    int lines = 0;

    //reads the file continously at a buffer
    //gcount is used to tell how many chars are read
    //gcount also used since read doesnt work it at end of file (eof)
    while (file.read(buffer, bufferSize) || file.gcount()) {
        std::streamsize bytesRead = file.gcount();

        //for every mass of chars/lines that are read, increment the line counter
        for (std::streamsize i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n')
                ++lines;
        }
    }

    return lines + 1;
}


//clears the hall of fame
void HallOfFame::resetHallOfFame() {
    players.clear();

    //opens the file for writing in truncate mode, overwriting all the data with nothing, esentially erasinf it
    std::ofstream file(fileDir, std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "Failed to reset Hall of Fame file: " << fileDir << std::endl;
        return;
    }

    //close file
    file.close();
}