#include "Game.h"
#include "Player.h"
#include "Entity.h"
#include "Enemy.h"
#include "Scoreboard.h"
#include <fstream>
#include <sstream>
#include <utility>
#include <iostream>

inline bool IsEmpty(const std::string& FileName) {
	std::fstream file(FileName);
	file.seekp(0, std::ios::end);
	std::streamoff tmp = file.tellg();
	return tmp ? false : true;
}

inline bool FileExists(const std::string& FileName) {
	struct stat buffer{};
	return (stat(FileName.c_str(), &buffer) == 0);
}

Game::Game()
	: m_GameName("Game") {
	m_Window = new sf::RenderWindow(sf::VideoMode(m_ScreenSize.x, m_ScreenSize.y), m_GameName,
	                                sf::Style::Titlebar | sf::Style::Close);
	m_Window->setMouseCursorVisible(false);
}

Game::Game(const int& X, const int& Y, std::string GameName)
	: m_GameName(std::move(GameName)), m_ScreenSize(X, Y) {
	m_Window = new sf::RenderWindow(sf::VideoMode(m_ScreenSize.x, m_ScreenSize.y), m_GameName,
	                                sf::Style::Titlebar | sf::Style::Close);
	m_Window->setMouseCursorVisible(false);
}

Game::Game(std::string GameName)
	: m_GameName(std::move(GameName)) {
	m_Window = new sf::RenderWindow(sf::VideoMode(m_ScreenSize.x, m_ScreenSize.y), m_GameName,
	                                sf::Style::Titlebar | sf::Style::Close);
	m_Window->setMouseCursorVisible(false);
}

Game::~Game() {
	delete m_Window;
}

void Game::ChoseLevel(Level& Lvl) const {
	switch (m_LevelNumber) {
	case 1:
		Lvl.LoadFromFile("map1.tmx");
		break;
	case 2:
		Lvl.LoadFromFile("map2.tmx");
		break;
	default:
		throw std::out_of_range("level" + std::to_string(m_LevelNumber) + "not found!");
	}
}

void Game::Run() {
	do {
		Menu();
	} while (m_Option != 5);
}

void Game::Menu() {
	if (m_FirstEntry) {
		m_Option = 1;
		m_FirstEntry = false;
	}
#pragma region button
	sf::Texture t_button;
	t_button.loadFromFile("../res/Textures/button_bg.png");
	sf::Sprite button;
	button.setColor(sf::Color(255, 255, 255, 210));
	button.setTexture(t_button);
#pragma endregion setting menu button texture
#pragma region background
	sf::Texture t_background;
	t_background.loadFromFile("../res/Textures/bg_menu.png");
	sf::Sprite background;
	background.setTexture(t_background);
#pragma endregion setting bg texture
#pragma region font
	sf::Font font;
	font.loadFromFile("../res/Fonts/dpcomic.ttf");
	sf::Text play, options, score, exit;
	play.setFont(font);
	options.setFont(font);
	score.setFont(font);
	exit.setFont(font);
#pragma endregion setting text font
#pragma region text
	play.setString("Play");
	options.setString("Options");
	score.setString("Scoreboard");
	exit.setString("Exit");
#pragma endregion setting text
#pragma region color
	play.setFillColor(sf::Color(255, 255, 255));
	options.setFillColor(sf::Color(255, 255, 255));
	score.setFillColor(sf::Color(255, 255, 255));
	exit.setFillColor(sf::Color(255, 255, 255));
#pragma endregion setting text color
#pragma region position
	sf::Vector2f center = m_Window->getView().getCenter();
	play.setPosition(center.x - 300, center.y - 125);
	options.setPosition(center.x - 350, center.y - 25);
	score.setPosition(center.x - 420, center.y + 25);
	exit.setPosition(center.x - 295, center.y + 75);
#pragma endregion setting text position
#pragma region size
	play.setCharacterSize(42);
	options.setCharacterSize(42);
	score.setCharacterSize(42);
	exit.setCharacterSize(42);
#pragma endregion setting text size
	while (m_Window->isOpen()) {
		sf::Event event{};
		while (m_Window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_Option = 5;
				return;
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					m_Option = 5;
					return;
				}
				if (event.key.code == sf::Keyboard::Up) {
					m_Option--;
					if (m_Option < 1) {
						m_Option = 1;
					}
				}
				if (event.key.code == sf::Keyboard::Down) {
					m_Option++;
					if (m_Option > 4) {
						m_Option = 4;
					}
				}
				if (event.key.code == sf::Keyboard::Enter) {
					switch (m_Option) {
					case 1:
						if (!m_FirstEntry) {
							EnterName();
							m_FirstEntry = true;
						}
						LevelLoad();
						break;
					case 2:
						Options();
						break;
					case 3:
						ScoreBoard();
						break;
					case 4:
						m_Option = 5;
						return;
					default:
						break;
					}
				}
			}
		}
		switch (m_Option) {
		case 1:
			button.setPosition(center.x - 600, center.y - 115);
			break;
		case 2:
			button.setPosition(center.x - 600, center.y - 15);
			break;
		case 3:
			button.setPosition(center.x - 600, center.y + 35);
			break;
		case 4:
			button.setPosition(center.x - 600, center.y + + 85);
			break;
		case 5:
			return;
		default:
			break;
		}
		m_Window->clear();
		m_Window->draw(background);
		m_Window->draw(button);
		m_Window->draw(play);
		m_Window->draw(options);
		m_Window->draw(exit);
		m_Window->draw(score);
		m_Window->display();
	}
}

void Game::Options() {
	int tmp_option = 1;
#pragma region button
	sf::Texture t_button;
	t_button.loadFromFile("../res/Textures/button_bg.png");
	sf::Sprite button;
	button.setColor(sf::Color(255, 255, 255, 210));
	button.setTexture(t_button);
#pragma endregion setting menu button texture
#pragma region background
	sf::Texture t_background;
	t_background.loadFromFile("../res/Textures/bg_menu.png");
	sf::Sprite background;
	background.setTexture(t_background);
#pragma endregion setting bg texture
#pragma region font
	sf::Font font;
	font.loadFromFile("../res/Fonts/dpcomic.ttf");
	sf::Text play, options, score, exit, difficulty_option, difficulty, back;
	play.setFont(font);
	options.setFont(font);
	score.setFont(font);
	exit.setFont(font);
	difficulty.setFont(font);
	difficulty_option.setFont(font);
	back.setFont(font);
#pragma endregion setting text font
#pragma region text
	play.setString("Play");
	options.setString("Options");
	score.setString("Scoreboard");
	exit.setString("Exit");
	difficulty.setString("Difficulty:");
	switch (m_Difficulty) {
	case easy:
		m_Difficulty = easy;
		difficulty_option.setString("Easy");
		break;
	case normal:
		m_Difficulty = normal;
		difficulty_option.setString("Normal");
		break;
	case hard:
		m_Difficulty = hard;
		difficulty_option.setString("Hard");
		break;
	}
	back.setString("Back");
#pragma endregion setting text
#pragma region color
	play.setFillColor(sf::Color(255, 255, 255));
	options.setFillColor(sf::Color(255, 255, 255));
	score.setFillColor(sf::Color(255, 255, 255));
	exit.setFillColor(sf::Color(255, 255, 255));
	difficulty.setFillColor(sf::Color(255, 255, 255));
	difficulty_option.setFillColor(sf::Color(255, 255, 255));
	back.setFillColor(sf::Color(255, 255, 255));
#pragma endregion setting text color
#pragma region position
	sf::Vector2f center = m_Window->getView().getCenter();
	play.setPosition(center.x - 300, center.y - 125);
	options.setPosition(center.x - 350, center.y - 25);
	score.setPosition(center.x - 420, center.y + 25);
	exit.setPosition(center.x - 295, center.y + 75);
	difficulty.setPosition(center.x + 100, center.y - 25);
	difficulty_option.setPosition(center.x + 100, center.y + 25);
	back.setPosition(center.x + 100, center.y + 75);
#pragma endregion setting text position
#pragma region size
	play.setCharacterSize(42);
	options.setCharacterSize(42);
	score.setCharacterSize(42);
	exit.setCharacterSize(42);
	difficulty.setCharacterSize(42);
	difficulty_option.setCharacterSize(42);
	back.setCharacterSize(42);
#pragma endregion setting text size
	while (m_Window->isOpen()) {
		sf::Event event;
		while (m_Window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_Option = 5;
				return;
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					return;
				}
				if (event.key.code == sf::Keyboard::Left && tmp_option == 1) {
					switch (m_Difficulty) {
					case easy:
						break;
					case normal:
						m_Difficulty = easy;
						difficulty_option.setString("Easy");
						break;
					case hard:
						m_Difficulty = normal;
						difficulty_option.setString("Normal");
						break;
					}
				}
				if (event.key.code == sf::Keyboard::Right && tmp_option == 1) {
					switch (m_Difficulty) {
					case easy:
						m_Difficulty = normal;
						difficulty_option.setString("Normal");
						break;
					case normal:
						m_Difficulty = hard;
						difficulty_option.setString("Hard");
						break;
					case hard:
						break;
					}
				}
				if (event.key.code == sf::Keyboard::Up) {
					tmp_option--;
					if (tmp_option < 1) {
						tmp_option = 1;
					}
				}
				if (event.key.code == sf::Keyboard::Down) {
					tmp_option++;
					if (tmp_option > 2) {
						tmp_option = 2;
					}
				}
				if (event.key.code == sf::Keyboard::Enter) {
					switch (tmp_option) {
					case 1:
						switch (m_Difficulty) {
						case easy:
							m_Difficulty = normal;
							difficulty_option.setString("Normal");
							break;
						case normal:
							m_Difficulty = hard;
							difficulty_option.setString("Hard");
							break;
						case hard:
							break;
						}
						break;
					case 2:
						return;
					}
				}
			}
		}
		switch (tmp_option) {
		case 1:
			button.setPosition(center.x + 66, center.y - 15);
			break;
		case 2:
			button.setPosition(center.x + 66, center.y + 85);
			break;
		}
		m_Window->clear();
		m_Window->draw(background);
		m_Window->draw(button);
		m_Window->draw(play);
		m_Window->draw(options);
		m_Window->draw(exit);
		m_Window->draw(score);
		m_Window->draw(difficulty);
		m_Window->draw(difficulty_option);
		m_Window->draw(back);
		m_Window->display();
	}
}

void Game::ScoreBoard() {
#pragma region button
	sf::Texture t_button;
	t_button.loadFromFile("../res/Textures/button_bg.png");
	sf::Sprite button;
	button.setColor(sf::Color(255, 255, 255, 210));
	button.setTexture(t_button);
#pragma endregion setting menu button texture
#pragma region background
	sf::Texture t_background;
	t_background.loadFromFile("../res/Textures/bg_menu.png");
	sf::Sprite background;
	background.setTexture(t_background);
#pragma endregion setting bg texture
#pragma region font
	sf::Font font;
	font.loadFromFile("../res/Fonts/dpcomic.ttf");
	sf::Text play, options, score, exit, back;
	sf::Text line1, line2, line3, line4;
	play.setFont(font);
	options.setFont(font);
	score.setFont(font);
	exit.setFont(font);
	back.setFont(font);
	line1.setFont(font);
	line2.setFont(font);
	line3.setFont(font);
	line4.setFont(font);
#pragma endregion setting text font
#pragma region text
	play.setString("Play");
	options.setString("Options");
	score.setString("Scoreboard");
	exit.setString("Exit");
	back.setString("Back");
	std::vector<std::string> a = LoadScore();
	line1.setString(a.at(0));
	line2.setString(a.at(1));
	line3.setString(a.at(2));
	line4.setString(a.at(3));
#pragma endregion setting text
#pragma region color
	play.setFillColor(sf::Color(255, 255, 255));
	options.setFillColor(sf::Color(255, 255, 255));
	score.setFillColor(sf::Color(255, 255, 255));
	exit.setFillColor(sf::Color(255, 255, 255));
	back.setFillColor(sf::Color(255, 255, 255));
	line1.setFillColor(sf::Color(255, 255, 255));
	line2.setFillColor(sf::Color(255, 255, 255));
	line3.setFillColor(sf::Color(255, 255, 255));
	line4.setFillColor(sf::Color(255, 255, 255));
#pragma endregion setting text color
#pragma region position
	sf::Vector2f center = m_Window->getView().getCenter();
	play.setPosition(center.x - 300, center.y - 125);
	options.setPosition(center.x - 350, center.y - 25);
	score.setPosition(center.x - 420, center.y + 25);
	exit.setPosition(center.x - 295, center.y + 75);
	line1.setPosition(center.x + 100, center.y - 125);
	line2.setPosition(center.x + 100, center.y - 75);
	line3.setPosition(center.x + 100, center.y - 25);
	line4.setPosition(center.x + 100, center.y + 25);
	back.setPosition(center.x + 100, center.y + 75);
#pragma endregion setting text position
#pragma region size
	play.setCharacterSize(42);
	options.setCharacterSize(42);
	score.setCharacterSize(42);
	exit.setCharacterSize(42);
	back.setCharacterSize(42);
	line1.setCharacterSize(42);
	line2.setCharacterSize(42);
	line3.setCharacterSize(42);
	line4.setCharacterSize(42);
#pragma endregion setting text size
	while (m_Window->isOpen()) {
		sf::Event event{};
		while (m_Window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_Option = 5;
				return;
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					return;
				}
				if (event.key.code == sf::Keyboard::Enter) {
					return;
				}
			}
		}
		button.setPosition(center.x + 66, center.y + 85);
		m_Window->clear();
		m_Window->draw(background);
		m_Window->draw(button);
		m_Window->draw(play);
		m_Window->draw(options);
		m_Window->draw(exit);
		m_Window->draw(score);
		m_Window->draw(line1);
		m_Window->draw(line2);
		m_Window->draw(line3);
		m_Window->draw(line4);
		m_Window->draw(back);
		m_Window->display();
	}
}

void Game::LevelLoad() {
	const auto a = LevelRunning();
	UpdateScore(m_PlayerScore);
 	if (a.first) {
		m_LevelNumber += a.second;
		if (m_LevelNumber >= 3) return;
		LevelLoad();
	}
}

void Game::UpdateScore(const int& Score) const {
	std::ofstream out_file;
	std::ifstream in_file;
	if (!FileExists("scores.txt") || IsEmpty("scores.txt")) {
		out_file.open("scores.txt");
		int a = 0;
		do {
			out_file << "........  -  00000\n";
			a++;
		} while (a < 4);
		out_file.close();
	}
	in_file.open("scores.txt");
	std::vector<std::string> lines;
	{
		std::string line;
		size_t counter = 0;
		while (std::getline(in_file, line)) {
			lines.emplace_back(line);
			counter++;
		}
		if (counter != 4) {
			in_file.close();
			out_file.open("scores.txt");
			int a = 0;
			do {
				out_file << "........  -  00000\n";
				a++;
			} while (a < 4);
			out_file.close();
			in_file.open("scores.txt");
			lines.clear();
			while (std::getline(in_file, line)) {
				lines.emplace_back(line);
			}
		}
	}
	std::vector<int> scores;
	for (auto& i : lines) {
		std::string tmp(i.end() - 4, i.end());
		scores.emplace_back(atoi(tmp.c_str()));
	}
	for (size_t i = 0; i < lines.size(); i++) {
		std::string tmp_name(lines.at(i).begin(), lines.at(i).begin() + m_PlayerName.size());
		std::string tmp_score_str(lines.at(i).end() - 4, lines.at(i).end());
		int tmp_score(atoi(tmp_score_str.c_str()));
		if (tmp_name == m_PlayerName) {
			if (tmp_score < m_PlayerScore) {
				in_file.close();
				lines.at(i).replace(lines.at(i).end() - 4, lines.at(i).end(), "00000");
				lines.at(i).replace(lines.at(i).end() - tmp_score_str.size() - 1, lines.at(i).end(), std::to_string(m_PlayerScore));
				scores.at(i) = m_PlayerScore;
				SortScores(lines, scores, 4);
				for (size_t j = 0; j < lines.size(); j++) {
					std::string tmp = std::to_string(scores.at(j));
					lines.at(j).replace(lines.at(j).end() - tmp.size(), lines.at(j).end(), tmp);
				}
				out_file.open("scores.txt");
				for (const auto& line : lines) {
					out_file << line << '\n';
				}
				return;
			}
			else return;
		}
	}
	{
		std::string tmp = m_PlayerName;
		for (size_t i = 0; i < (8 - m_PlayerName.size()); i++) {
			tmp += ' ';
		}
		tmp += "  -  00000";
		lines.emplace_back(tmp);
	}
	scores.emplace_back(Score);
	in_file.close();
	int line_to_replace = static_cast<int>(lines.size()) - 2;
	for (size_t i = 0; i < lines.size() - 1; i++) {
		if (lines.at(i) == "........  -  00000") {
			line_to_replace = i;
			break;
		}
	}
	if (scores.at(4) >= scores.at(line_to_replace)) {
		scores.at(line_to_replace) = scores.at(4);
		lines.at(line_to_replace) = lines.at(4);
		if (line_to_replace > 0) {
			SortScores(lines, scores, line_to_replace);
		}
		for (size_t j = 0; j < lines.size() - 1; j++) {
			std::string tmp = std::to_string(scores.at(j));
			lines.at(j).replace(lines.at(j).end() - tmp.size(), lines.at(j).end(), tmp);
		}
		out_file.open("scores.txt");
		for (size_t j = 0; j < lines.size() - 1; j++) {
			out_file << lines.at(j) << '\n';
		}
		out_file.close();
	}
}

void Game::SortScores(std::vector<std::string>& Lines, std::vector<int>& Scores, const int& Last) const {
	for (size_t i = 0; i < Scores.size() - 1 - (Scores.size() - Last); i++) {
		for (size_t j = 0; j < Scores.size() - i - 2; j++) {
			if (Scores.at(j) < Scores.at(j + 1)) {
				std::swap(Scores.at(j), Scores.at(j + 1));
				std::swap(Lines.at(j), Lines.at(j + 1));
			}
		}
	}
}

std::vector<std::string> Game::LoadScore() const {
	if (!FileExists("scores.txt") || IsEmpty("scores.txt")) {
		std::ofstream out_file("scores.txt");
		int a = 0;
		do {
			out_file << "........  -  00000\n";
			a++;
		}
		while (a < 4);
		out_file.close();
	}
	std::ifstream in_file("scores.txt");
	std::vector<std::string> lines;
	std::string line;
	int counter = 0;
	while (std::getline(in_file, line)) {
		lines.emplace_back(line);
		counter++;
	}
	in_file.close();
	if (counter != 4) {
		std::ofstream out_file("scores.txt");
		int a = 0;
		do {
			out_file << "........  -  00000\n";
			a++;
		} while (a < 4);
		out_file.close();
		for (size_t i = 0; i < lines.size(); i++) {
			lines.at(i) = "........ - 00000\n";
		}
	}
	return lines;
}

std::pair<bool, size_t> Game::LevelRunning() {
	//Level load
	Level level;
	ChoseLevel(level);
	m_TempPlayerScore = m_PlayerScore;
	//Creating player and resetting view
	Object player_obj = level.GetObject("player");
	Player* player = new Player();
	switch (m_Difficulty) {
	case easy:
		player = new Player(level, player_obj.Rect.left, player_obj.Rect.top, 100, 200);
		break;
	case normal:
		player = new Player(level, player_obj.Rect.left, player_obj.Rect.top, 50, 100);
		break;
	case hard:
		player = new Player(level, player_obj.Rect.left, player_obj.Rect.top, 25, 100);
		break;
	}
	player->GetView().reset(sf::FloatRect(0, 0, 1280, 720));

	//Creating enemies
	std::vector<Entity*> entities;
	std::vector<Object> enemy_objects = level.GetObjects("enemy");
	for (auto& enemy_object : enemy_objects) {
		switch (m_Difficulty) {
		case easy:
			entities.emplace_back(new Enemy(level, enemy_object.Rect.left, enemy_object.Rect.top, 10, 100));
			break;
		case normal:
			entities.emplace_back(new Enemy(level, enemy_object.Rect.left, enemy_object.Rect.top, 25, 100));
			break;
		case hard:
			entities.emplace_back(new Enemy(level, enemy_object.Rect.left, enemy_object.Rect.top, 50, 150));
			break;
		}
	}

	//Loadig background
	sf::Texture background_texture;
	sf::Sprite background;
	background_texture.loadFromFile("../res/Textures/main_background.png");
	sf::Vector2u texture_size;
	sf::Vector2u window_size;
	texture_size = background_texture.getSize();
	window_size = m_Window->getSize();
	float scale_x = static_cast<float>(window_size.x) / texture_size.x;
	float scale_y = static_cast<float>(window_size.y) / texture_size.y;
	background.setTexture(background_texture);
	background.setScale(scale_x * 1.5f, scale_y * 1.5f);
	background.setPosition(0, 512);

	sf::Font font;
	font.loadFromFile("../res/Fonts/dpcomic.ttf");
	sf::Text score;
	score.setFont(font);
	score.setString("score:" + std::to_string(m_PlayerScore));
	score.setFillColor(sf::Color(255, 255, 255));
	score.setCharacterSize(40);

	sf::Clock clock;
	bool window_change = false;
	while (m_Window->isOpen()) {
		if (window_change)
			break;
		float time = static_cast<float>(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		time /= 800;
		sf::Event event{};
		while (m_Window->pollEvent(event)) {
			switch (event.type) {

			case sf::Event::Closed:
				window_change = true;
				m_Option = 5;
				break;

			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					window_change = true;
				default: ;
				}
				break;
			default: ;
			}
		}
		player->Update(time, entities, *player);
		if (std::get<0>(player->LvlEnd())) {
			auto a = player->LvlEnd();
			if (a.second == 0) {
				m_PlayerScore += 0;
			}
			else m_PlayerScore += m_TempPlayerScore;
			m_Window->setView(m_Window->getDefaultView());
			return a;
		}
		for (size_t i = 0; i < entities.size(); i++) {
			entities.at(0)->Update(time, entities, *player);
		}
		m_Window->clear(sf::Color::Black);
		m_Window->draw(background);
		level.Draw(*m_Window);
		for (size_t i = 0; i < entities.size(); i++) {
			if(!entities.at(i)->Alive()) {
				if (entities.at(i)->GetName() == "swordsman") {
					switch (m_Difficulty) {
					case easy:
						m_TempPlayerScore += 50;
						break;
					case normal:
						m_TempPlayerScore += 100;
						break;
					case hard:
						m_TempPlayerScore += 200;
						break;
					}
				}
				score.setString("score: " + std::to_string(m_TempPlayerScore));
				entities.erase(entities.begin()+i);
				continue;
			}
			entities.at(i)->Draw(*m_Window);
		}
		m_Window->setView(player->GetView());
		player->Draw(*m_Window);
		player->GetHealthBar().Draw(*m_Window);
		score.setPosition(m_Window->getView().getCenter().x - m_Window->getView().getSize().x / 2 + 267.f, m_Window->getView().getCenter().y - m_Window->getView().getSize().y / 2 - 7.f);
		m_Window->draw(score);
		m_Window->display();
	}
	m_Window->setView(m_Window->getDefaultView());
	m_PlayerScore += 0;
	return { false , 0 };
}

void Game::EnterName() {
	#pragma region button
	sf::Texture t_button;
	t_button.loadFromFile("../res/Textures/button_bg.png");
	sf::Sprite button;
	button.setColor(sf::Color(255, 255, 255, 210));
	button.setTexture(t_button);
	#pragma endregion setting menu button texture
	#pragma region background
	sf::Texture t_background;
	t_background.loadFromFile("../res/Textures/bg_menu.png");
	sf::Sprite background;
	background.setTexture(t_background);
	#pragma endregion setting bg texture
	#pragma region font
	sf::Font font;
	font.loadFromFile("../res/Fonts/dpcomic.ttf");
	sf::Text name, name_option;
	name.setFont(font);
	name_option.setFont(font);
	#pragma endregion setting text font
	#pragma region text
	name.setString("Enter your name:");
	name_option.setString("");
	#pragma endregion setting text
	#pragma region color
	name.setFillColor(sf::Color(255, 255, 255));
	name_option.setFillColor(sf::Color(255, 255, 255));
	#pragma endregion setting text color
	#pragma region position
	sf::Vector2f center = m_Window->getView().getCenter();
	name.setPosition(center.x - 515, center.y - 125);
	name_option.setPosition(center.x - 450, center.y - 25);
	button.setPosition(center.x - 560, center.y - 15);
	#pragma endregion setting text position
	#pragma region size
	name.setCharacterSize(42);
	name_option.setCharacterSize(42);
	#pragma endregion setting text size
	FocusController focus_controller;
	TextBox text_box(name_option);
	focus_controller.SetFocusObject(&text_box);
	bool close = false;
	while (m_Window->isOpen()) {
		sf::Event event{};
		if (close) {
			break;
		}
		while (m_Window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_PlayerName = text_box.GetText();
				m_Option = 5;
				return;
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					m_PlayerName = text_box.GetText();
					return;
				}
				if (event.key.code == sf::Keyboard::Return) {
					m_PlayerName = text_box.GetText();
					return;
				}
			}
			FocusObject* focus_object = focus_controller.GetFocusObject();
			if (focus_object != nullptr) 
				focus_object->Event(event);
		}
		m_Window->clear();
		m_Window->draw(background);
		m_Window->draw(button);
		m_Window->draw(name);
		m_Window->draw(text_box);
		m_Window->display();
	}
}
