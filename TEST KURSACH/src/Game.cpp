#include <iostream>
#include "Game.h"
#include "Player.h"
#include "Entity.h"
#include "Enemy.h"

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
	delete m_Window;
}

void Game::Run() {
	//Level load
	Level level;
	level.LoadFromFile("map.tmx");

	//Checking positions of objects to spawn
	Object PlayerObj = level.GetObject("player");
	Object EnemyObj = level.GetObject("enemy");

	//Creating Window
	m_Window = new sf::RenderWindow(sf::VideoMode(m_ScreenSize.x, m_ScreenSize.y), m_GameName, sf::Style::Titlebar | sf::Style::Close);
	m_Window->setMouseCursorVisible(false);

	//Creating player and resetting view
	Player player(level, PlayerObj.rect.left, PlayerObj.rect.top);
	player.view.reset(sf::FloatRect(0, 0, 1280, 720));

	std::vector<Entity*> enteties;
	enteties.emplace_back(new Enemy(level, EnemyObj.rect.left, EnemyObj.rect.top));

	//Loadig background
	sf::View BackgroundView;
	BackgroundView.reset(sf::FloatRect(0, 0, 1280, 720));
	sf::Texture BackgroundTexture;
	sf::Sprite Background;
	BackgroundTexture.loadFromFile("../res/Textures/main_background.png");
	sf::Vector2u TextureSize;
	sf::Vector2u WindowSize;
	TextureSize = BackgroundTexture.getSize();
	WindowSize = m_Window->getSize();
	float ScaleX = (float)WindowSize.x / TextureSize.x;
	float ScaleY = (float)WindowSize.y / TextureSize.y;
	Background.setTexture(BackgroundTexture);
	Background.setScale(ScaleX * 1.5f, ScaleY * 1.5f);
	Background.setPosition(0, 512);

	sf::Clock clock;

	while (m_Window->isOpen()) {

		float time = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		time /= 800;
		sf::Event Event;
		while (m_Window->pollEvent(Event)) {
			switch (Event.type) {

			case sf::Event::Closed:
				m_Window->close();

				break;

			case sf::Event::KeyPressed:
				switch (Event.key.code) {

				case sf::Keyboard::Escape:
					m_Window->close();
				}
				break;
			}
		}
		try {
			player.Update(time, enteties, player);
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << " 1\n";
		}
		try {
			for (size_t i = 0; i < enteties.size(); i++) {
				enteties.at(0)->Update(time, enteties, player);
			}
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << " 2\n";
		}
		m_Window->clear(sf::Color::Black);
		m_Window->draw(Background);
		level.Draw(*m_Window);
		for (size_t i = 0; i < enteties.size(); i++) {
			enteties.at(0)->Draw(*m_Window);
		}
		m_Window->setView(player.view);
		player.Draw(*m_Window);
		m_Window->display();
	}
}
