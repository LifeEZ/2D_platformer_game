#pragma once
#include "SFML/Graphics.hpp"
class Game {
private:
	sf::RenderWindow* m_Window = NULL;
	std::string m_GameName;
	sf::Vector2i m_ScreenSize = {1280, 720};
public:
	Game();
	Game(const int& X, const int& Y, const std::string& GameName);
	Game(const std::string& GameName);
	~Game();
	void Run();
};
