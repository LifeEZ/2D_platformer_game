#pragma once
#include "SFML/Graphics.hpp"
#include "..//level.h"
class Game {
private:
	sf::RenderWindow* m_Window = NULL;
	std::string m_GameName;
	sf::Vector2i m_ScreenSize = {1280, 720};
public:
	int LevelNumber = 1;
	Game();
	Game(const int& X, const int& Y, const std::string& GameName);
	Game(const std::string& GameName);
	~Game();
	std::pair<bool, size_t> LevelRunning();
	void ChoseLevel(Level& level);
	void Run();
};
