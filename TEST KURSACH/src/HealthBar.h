#pragma once
#include "SFML/Graphics.hpp"
class HealthBar {
public:
	HealthBar();
	~HealthBar();
	void Draw(sf::RenderWindow& Window) const;
	void Update(const int& Health) const;
private:
	sf::Texture* m_TBackground = nullptr;
	sf::Texture* m_TBar = nullptr;
	sf::Sprite* m_Background = nullptr;
	sf::Sprite* m_Bar = nullptr;
};
