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
	m_Background->setScale(2.5f, 2.5f);
	m_Bar->setScale(2.5f, 2.5f);
}

HealthBar::~HealthBar() = default;

void HealthBar::Draw(sf::RenderWindow& Window) const {
	const sf::Vector2f center = Window.getView().getCenter();
	const sf::Vector2f size = Window.getView().getSize();
	m_Background->setPosition(center.x - size.x / 2 + 17.f, center.y - size.y / 2 + 10.f);
	m_Bar->setPosition(center.x - size.x / 2 + 37.75f, center.y - size.y / 2 + 14.5f);
	Window.draw(*m_Background);
	Window.draw(*m_Bar);
}

void HealthBar::Update(const int& Health) const {
	const float tmp = 64.f - (((100.f - static_cast<float>(Health)) / 10.f) * 6.4f);
	const sf::IntRect bar_rect = { 0,0,static_cast<int>(tmp),5 };
	m_Bar->setTextureRect(bar_rect);
}
