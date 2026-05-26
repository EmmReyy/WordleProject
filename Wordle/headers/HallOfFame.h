#pragma once
#include <string>
#include <vector>

//player struct 
struct Player
{
    int id;                 //unique id for player
    std::string name;       //name of player
    int wins;               //number of wins by player
    int games;              //number of games played
    int currentStreak;      //number of games won back to back
    int bestStreak;         //most games won back to back 
    int fastestWin;         //least amount of guesses to win
    float totalAcc;         //total accrued accuracy 
    float accuracy;         //accuracy in percent
    std::string lastPlayed; //date of last played
};

class HallOfFame
{
public:
    //constructor takes file path
    HallOfFame(std::string dir);

    //main functions
    void updatePlayer(const std::string& name, int guesses, bool win, float acc);
    void displayHallOfFame() const;
    int numOfLines();
    void resetHallOfFame();

private:
    std::vector<Player> players;
    std::string fileDir;

    //file handling
    void load();
    void save() const;

    //helpers
    int generateID() const;
    std::string getDate();
};

