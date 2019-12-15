#pragma once
#include "SFML/Graphics.hpp"
#include "..//level.h"
class Game {
	sf::RenderWindow* m_Window = nullptr;
	std::string m_GameName;
	sf::Vector2i m_ScreenSize = { 1280, 720 };
	int m_LevelNumber = 1;
	enum Difficulty { easy, normal, hard } m_Difficulty = normal;
	int m_Option = 4;
	bool m_FirstEntry = true;
	std::string m_PlayerName = "name";
	int m_PlayerScore = 0;
	int m_TempPlayerScore = 0;
public:
	Game();
	Game(const int& X, const int& Y, std::string GameName);
	explicit Game(std::string GameName);
	~Game();
	void Menu();
	void EnterName();
	void Options();
	void ScoreBoard();
	void LevelLoad();
	void UpdateScore(const int& Score) const;
	void SortScores(std::vector<std::string>& Lines, std::vector<int>& Scores, const int& Last) const;
	std::vector<std::string> LoadScore() const;
	std::pair<bool, size_t> LevelRunning();
	void ChoseLevel(Level& Lvl) const;
	void Run();
};
