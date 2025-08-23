#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct LeaderboardRecord {
    std::string name;
    int score;
    
    LeaderboardRecord() : name(""), score(0) {}
    LeaderboardRecord(const std::string& n, int s) : name(n), score(s) {}
};

// Comparison function for sorting
bool compareLeaderboardRecords(const LeaderboardRecord& a, const LeaderboardRecord& b);

class Leaderboard {
private:
    std::vector<LeaderboardRecord> records;
    static const int MAX_RECORDS = 5;
    
public:
    Leaderboard();
    
    void addScore(const std::string& name, int score);
    void generateFakeRecords();
    const std::vector<LeaderboardRecord>& getRecords() const;
    int getPlayerRank(const std::string& playerName) const;
    void draw(sf::RenderWindow& window, const sf::Font& font, const std::string& playerName, int playerScore) const;
};
