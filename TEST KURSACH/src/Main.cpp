#include <iostream>
#include "Game.h"

int main() {
	sf::RenderWindow window;

	window.create(sf::VideoMode(1280, 720), "TEST", sf::Style::Titlebar | sf::Style::Close);

	window.setKeyRepeatEnabled(true);

	//Main Loop:
	while (window.isOpen()) {

		sf::Event Event;

		//Event Loop:
		while (window.pollEvent(Event)) {
			switch (Event.type) {

			case sf::Event::Closed:
				window.close();
			}

		}
		window.clear();
		window.display();
	}
}