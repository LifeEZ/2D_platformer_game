#pragma once
#include <string>
#include <vector>
#include <map>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "TinyXML/tinyxml.h"

struct Object {
	int GetPropertyInt(std::string name);//proprety's number in list
	float GetPropertyFloat(std::string name);
	std::string GetPropertyString(std::string name);

	std::string name;
	std::string type;
	sf::Rect<float> rect;
	std::map<std::string, std::string> properties;

	sf::Sprite sprite;
};

struct Layer {
	int opacity = 0;
	std::vector<sf::Sprite> tiles;
};

class Level {
public:
	bool LoadFromFile(std::string filename);//return false if loading is failed
	Object GetObject(std::string name);
	std::vector<Object> GetObjects(std::string name);//getting all found objects
	std::vector<Object> GetAllObjects();//getting all objects
	void Draw(sf::RenderWindow &window);//drawing level
	sf::Vector2i GetTileSize();//getting size of tile

private:
	int width = 0, height = 0, tileWidth = 0, tileHeight = 0;
	int firstTileID = 0;
	sf::Rect<float> drawingBounds;
	sf::Texture tilesetImage;
	std::vector<Object> objects;
	std::vector<Layer> layers;
};