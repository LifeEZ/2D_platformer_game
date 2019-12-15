#pragma once
#include <string>
#include <vector>
#include <map>
#include "SFML/Graphics.hpp"
#include "TinyXML/tinyxml.h"

struct Object {
	int GetPropertyInt(const std::string& Name);//property's number in list
	float GetPropertyFloat(const std::string& Name);
	std::string GetPropertyString(const std::string& Name);

	std::string Name;
	std::string Type;
	sf::Rect<float> Rect;
	std::map<std::string, std::string> Properties;

	sf::Sprite Sprite;
};

struct Layer {
	int Opacity = 0;
	std::vector<sf::Sprite> Tiles;
};

class Level {
public:
	bool LoadFromFile(const std::string& Filename);//return false if loading is failed
	Object GetObject(const std::string& Name);
	std::vector<Object> GetObjects(const std::string& Name);//getting all found objects
	std::vector<Object> GetAllObjects() const;//getting all objects
	void Draw(sf::RenderWindow &Window);//drawing level
	sf::Vector2i GetTileSize() const;//getting size of tile
	const int& GetWidth() const;
	const int& GetHeight() const;
private:
	int m_Width = 0, m_Height = 0, m_TileWidth = 0, m_TileHeight = 0;
	int m_FirstTileId = 0;
	sf::Rect<float> m_DrawingBounds;
	sf::Texture m_TileSetImage;
	std::vector<Object> m_Objects;
	std::vector<Layer> m_Layers;
};