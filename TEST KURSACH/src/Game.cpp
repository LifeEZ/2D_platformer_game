#include <iostream>
#include <string>
#include "Game.h"
#include "Vector2.h"m_ScreenSize

Game::Game()
	: m_GameName("Game") {
}

Game::Game(const int& X, const int& Y, const std::string& GameName)
	: m_ScreenSize(X, Y), m_GameName(GameName) {
}

Game::~Game() {
}

void Game::Run() {
	m_Window = new sf::RenderWindow(sf::VideoMode(m_ScreenSize.X, m_ScreenSize.Y), m_root_object->getName());
	sf::Event event;
	sf::Clock clock;
	sf::Time acumulator = sf::Time::Zero;
	const sf::Time ups = sf::seconds(1.f / 60.f);
	//m_window->setFramerateLimit(60);
	//m_window->setVerticalSyncEnabled(true);

	while (true)   // game loop
	{

		while (m_window->pollEvent(event)) {
			if (event.type == sf::Event::EventType::Closed) {
				m_window->close();
				exit(0);
			}
			eventManager().pushEvent(event);
		}

		sf::Time elapsedTime = clock.restart();
		acumulator += elapsedTime;
		//updateStats(elapsedTime);

		while (acumulator > ups) {
			acumulator -= ups;
			inputManager().update(ups.asMilliseconds());
			update(ups.asMilliseconds());
			sf::sleep(sf::milliseconds(10));
		}

		m_window->clear(m_clear_color);
		draw(m_window);
		m_window->display();
	}
}
