#pragma once
#include "SFML/Graphics.hpp"
class HealthBar {
public:
	HealthBar();
	~HealthBar();
	void Draw(sf::RenderWindow& window);
	void Update(const int& Health);
private:
	float m_W = 64.f;
	sf::Texture* m_TBackground = nullptr;
	sf::Texture* m_TBar = nullptr;
	sf::Sprite* m_Background = nullptr;
	sf::Sprite* m_Bar = nullptr;
	sf::IntRect m_BarRect = { 0,0,0,0 };
};
