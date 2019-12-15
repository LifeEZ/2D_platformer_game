#include "level.h"
#include  <iostream>
int Object::GetPropertyInt(const std::string& Name){
	return atoi(Properties[Name].c_str());
}

float Object::GetPropertyFloat(const std::string& Name) {
	return static_cast<float>(strtod(Properties[Name].c_str(), nullptr));
}

std::string Object::GetPropertyString(const std::string& Name) {
	return Properties[Name];
}

bool Level::LoadFromFile(const std::string& Filename) {
	TiXmlDocument level_file(Filename.c_str()); //loading file into TiXmlDocument

	// loading XML-map
	if (!level_file.LoadFile()) {
		std::cout << "Loading level \"" << Filename << "\" failed." << std::endl;
		return false;
	}

	// work with map container
	TiXmlElement* map;
	map = level_file.FirstChildElement("map");

	// map example: <map version="1.0" orientation="orthogonal"
	// width="10" height="10" tilewidth="34" tileheight="34">
	m_Width = atoi(map->Attribute("width")); //extrecting propreties
	m_Height = atoi(map->Attribute("height"));
	m_TileWidth = atoi(map->Attribute("tilewidth"));
	m_TileHeight = atoi(map->Attribute("tileheight"));

	// Taking the description of the tileset and the indetifier of first tile
	TiXmlElement* tile_set_element;
	tile_set_element = map->FirstChildElement("tileset");
	m_FirstTileId = atoi(tile_set_element->Attribute("firstgid"));

	// source - path to the image container
	TiXmlElement* image;
	image = tile_set_element->FirstChildElement("image");
	std::string imagepath = image->Attribute("source");

	// loading tileset
	sf::Image img;

	if (!img.loadFromFile(imagepath)) {
		std::cout << "Failed to load tile sheet." << std::endl;
		return false;
	}


	img.createMaskFromColor(sf::Color(255, 255, 255)); //mask for color
	m_TileSetImage.loadFromImage(img);
	m_TileSetImage.setSmooth(false); //smoothnes

	// number of rows and cols of tileset
	int columns = m_TileSetImage.getSize().x / m_TileWidth;
	int rows = m_TileSetImage.getSize().y / m_TileHeight;

	// vector of images (IntRect)
	std::vector<sf::Rect<int>> sub_rects;

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			sf::Rect<int> rect;

			rect.top = y * m_TileHeight;
			rect.height = m_TileHeight;
			rect.left = x * m_TileWidth;
			rect.width = m_TileWidth;

			sub_rects.emplace_back(rect);
		}
	}
	// work with layers
	TiXmlElement* layerElement;
	layerElement = map->FirstChildElement("layer");
	while (layerElement) {
		Layer layer;

		// giving layer opacity
		if (layerElement->Attribute("opacity") != nullptr) {
			float opacity = static_cast<float>(strtod(layerElement->Attribute("opacity"), nullptr));
			layer.Opacity = static_cast<int>(255 * opacity);
		}
		else {
			layer.Opacity = 255;
		}

		//  <data> container
		TiXmlElement* layer_data_element;
		layer_data_element = layerElement->FirstChildElement("data");

		if (layer_data_element == nullptr) {
			std::cout << "Bad map. No layer information found." << std::endl;
		}

		//  <tile> container - tile description
		TiXmlElement* tile_element;
		tile_element = layer_data_element->FirstChildElement("tile");

		if (tile_element == nullptr) {
			std::cout << "Bad map. No tile information found." << std::endl;
			return false;
		}

		int x = 0;
		int y = 0;

		while (tile_element) {
			int tile_gid = atoi(tile_element->Attribute("gid"));
			int sub_rect_to_use = tile_gid - m_FirstTileId;

			// Setting TextureRect for each tile
			if (sub_rect_to_use >= 0) {
				sf::Sprite sprite;
				sprite.setTexture(m_TileSetImage);
				sprite.setTextureRect(sub_rects[sub_rect_to_use]);
				sprite.setPosition(static_cast<float>(x * m_TileWidth), static_cast<float>(y * m_TileHeight));
				sprite.setColor(sf::Color(255, 255, 255, layer.Opacity));

				layer.Tiles.emplace_back(sprite); //Placing tiles into layer
			}

			tile_element = tile_element->NextSiblingElement("tile");

			x++;
			if (x >= m_Width) {
				x = 0;
				y++;
				if (y >= m_Height)
					y = 0;
			}
		}

		m_Layers.emplace_back(layer);

		layerElement = layerElement->NextSiblingElement("layer");
	}

	// Work with objects
	TiXmlElement* objectGroupElement;

	// If object layer found
	if (map->FirstChildElement("objectgroup") != nullptr) {
		objectGroupElement = map->FirstChildElement("objectgroup");
		while (objectGroupElement) {
			//  <object> container
			TiXmlElement* object_element;
			object_element = objectGroupElement->FirstChildElement("object");

			while (object_element) {
				// Getting all the props
				std::string object_type;
				if (object_element->Attribute("type") != nullptr) {
					object_type = object_element->Attribute("type");
				}
				std::string object_name;
				if (object_element->Attribute("name") != nullptr) {
					object_name = object_element->Attribute("name");
				}
				int x = atoi(object_element->Attribute("x"));
				int y = atoi(object_element->Attribute("y"));

				int width, height;

				sf::Sprite sprite;
				sprite.setTexture(m_TileSetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition(static_cast<float>(x), static_cast<float>(y));

				if (object_element->Attribute("width") != nullptr) {
					width = atoi(object_element->Attribute("width"));
					height = atoi(object_element->Attribute("height"));
				}
				else {
					width = sub_rects[atoi(object_element->Attribute("gid")) - m_FirstTileId].width;
					height = sub_rects[atoi(object_element->Attribute("gid")) - m_FirstTileId].height;
					sprite.setTextureRect(sub_rects[atoi(object_element->Attribute("gid")) - m_FirstTileId]);
				}

				// Object instance
				Object object;
				object.Name = object_name;
				object.Type = object_type;
				object.Sprite = sprite;

				sf::Rect<float> objectRect;
				objectRect.top = static_cast<float>(y);
				objectRect.left = static_cast<float>(x);
				objectRect.height = static_cast<float>(height);
				objectRect.width = static_cast<float>(width);
				object.Rect = objectRect;

				// Object props
				TiXmlElement* properties;
				properties = object_element->FirstChildElement("properties");
				if (properties != nullptr) {
					TiXmlElement* prop;
					prop = properties->FirstChildElement("property");
					if (prop != nullptr) {
						while (prop) {
							std::string property_name = prop->Attribute("name");
							std::string property_value = prop->Attribute("value");

							object.Properties[property_name] = property_value;

							prop = prop->NextSiblingElement("property");
						}
					}
				}


				m_Objects.push_back(object);

				object_element = object_element->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else {
		std::cout << "No object layers found..." << std::endl;
	}

	return true;
}

Object Level::GetObject(const std::string& Name) {
	// First found object
	for (auto& object : m_Objects)
		if (object.Name == Name)
			return object;
}

std::vector<Object> Level::GetObjects(const std::string& Name) {
	// All found objects
	std::vector<Object> vec;
	for (auto& object : m_Objects)
		if (object.Name == Name)
			vec.push_back(object);
	return vec;
}


std::vector<Object> Level::GetAllObjects() const {
	return m_Objects;
};


sf::Vector2i Level::GetTileSize() const {
	return {m_TileWidth, m_TileHeight};
}

const int& Level::GetWidth() const {
	return m_Width;
}

const int& Level::GetHeight() const {
	return m_Height;
}

void Level::Draw(sf::RenderWindow& Window) {
	// Drawing all tiles
	for (auto& layer : m_Layers)
		for (const auto& tile : layer.Tiles)
			Window.draw(tile);
}
