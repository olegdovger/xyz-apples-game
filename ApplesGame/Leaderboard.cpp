#include "Leaderboard.h"
#include "Math.h"

bool compareLeaderboardRecords(const LeaderboardRecord& a, const LeaderboardRecord& b) {
    return a.score < b.score;
}

Leaderboard::Leaderboard() {
    generateFakeRecords();
}

void Leaderboard::addScore(const std::string& name, int score) {
    records[name] = score;
}

void Leaderboard::generateFakeRecords() {
    records.clear();
    records["Alice"] = 54;
    records["Bob"] = 43;
    records["Carol"] = 32;
    records["Dave"] = 21;
    records["Player"] = 0;
}

const std::unordered_map<std::string, int>& Leaderboard::getRecords() const {
    return records;
}

void Leaderboard::draw(sf::RenderWindow& window, const sf::Font& font, const std::string& playerName, int playerScore) const {
    std::unordered_map<std::string, int> displayMap = records;
    displayMap[playerName] = playerScore;
    
    std::vector<LeaderboardRecord> displayRecords;
    
    Math::insertionSortDescending(displayMap, displayRecords, compareLeaderboardRecords);
    
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("LEADERBOARD");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setStyle(sf::Text::Bold);
    
    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    titleText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 200);
    
    window.draw(titleText);
    
    float startY = window.getSize().y / 2.0f - 120;
    float lineHeight = 40;
    
    for (int i = 0; i < displayRecords.size(); i++) {
        LeaderboardRecord record = displayRecords[i];
        
        sf::Color textColor;
        if (record.name == playerName) {
            textColor = sf::Color::Green;
        } else {
            textColor = sf::Color::White;
        }
        
        sf::Text rankText;
        rankText.setFont(font);
        rankText.setString(std::to_string(i + 1) + ".");
        rankText.setCharacterSize(24);
        rankText.setFillColor(textColor);
        rankText.setPosition(window.getSize().x / 2.0f - 150, startY + i * lineHeight);
        window.draw(rankText);
        
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(record.name);
        nameText.setCharacterSize(24);
        nameText.setFillColor(textColor);
        nameText.setPosition(window.getSize().x / 2.0f - 80, startY + i * lineHeight);
        window.draw(nameText);
        
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString(std::to_string(record.score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(textColor);
        scoreText.setPosition(window.getSize().x / 2.0f + 80, startY + i * lineHeight);
        window.draw(scoreText);
    }
    
    sf::Text instructionText;
    instructionText.setFont(font);
    instructionText.setString("Press ESC to return to mode selection");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color(200, 200, 200));
    
    sf::FloatRect instructionRect = instructionText.getLocalBounds();
    instructionText.setOrigin(instructionRect.left + instructionRect.width / 2.0f, instructionRect.top + instructionRect.height / 2.0f);
    float instructionY = startY + displayRecords.size() * lineHeight + 40;
    instructionText.setPosition(window.getSize().x / 2.0f, instructionY);
    
    window.draw(instructionText);
}
