#include "HealthBar.h"
#include <iostream>

HealthBar::HealthBar() {
	m_TBackground = new sf::Texture;
	m_TBar = new sf::Texture;
	m_Background = new sf::Sprite;
	m_Bar = new sf::Sprite;
	m_TBackground->loadFromFile("../res/Textures/HealthBarBg.png");
	m_TBar->loadFromFile("../res/Textures/HealthBar.png");
	m_Background->setTexture(*m_TBackground);
	m_Bar->setTexture(*m_TBar);
	m_Background->setScale(2.f, 2.f);
	m_Bar->setScale(2.f, 2.f);
}

HealthBar::~HealthBar() {
}

void HealthBar::Draw(sf::RenderWindow& window) {
	sf::Vector2f center = window.getView().getCenter();
	sf::Vector2f size = window.getView().getSize();
	m_Background->setPosition(center.x - size.x / 2 + 17.f, center.y - size.y / 2 + 10.f);
	m_Bar->setPosition(center.x - size.x / 2 + 33.f, center.y - size.y / 2 + 14.f);
	window.draw(*m_Background);
	window.draw(*m_Bar);
}

void HealthBar::Update(const int& Health) {
	float tmp = 64 - (((100 - Health) / 10) * 6.4);
	sf::IntRect m_BarRect = { 0,0,static_cast<int>(tmp),5 };
	m_Bar->setTextureRect(m_BarRect);
}
