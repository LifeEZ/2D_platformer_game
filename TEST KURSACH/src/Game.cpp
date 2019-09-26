#include <iostream>
#include <string>
#include "Game.h"
#include "Vector2.h"

Game::Game()
	: m_GameName("Game") {
}

Game::Game(const int& X, const int& Y, const std::string& GameName)
	: m_ScreenSize(X, Y), m_GameName(GameName) {
}

Game::Game(const std::string& GameName)
	: m_GameName(GameName) {
}

Game::~Game() {
}

void Game::Run() {
	m_Window = new sf::RenderWindow(sf::VideoMode(m_ScreenSize.X, m_ScreenSize.Y), m_GameName, sf::Style::Titlebar | sf::Style::Close);
	m_Window->setKeyRepeatEnabled(true);
	sf::Texture texture;
	texture.loadFromFile("../res/Textures/main_background.png");
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);
	while (m_Window->isOpen()) {
		sf::Event Event;
		while (m_Window->pollEvent(Event)) {
			switch (Event.type) {
			case sf::Event::Closed:
				m_Window->close();
			}
		}
		m_Window->clear(sf::Color::Black);
		m_Window->draw(sprite);
		m_Window->display();
	}
}
