#include "level.h"

int Object::GetPropertyInt(std::string name)//возвращаем номер свойства в нашем списке
{
	return atoi(properties[name].c_str());
}

float Object::GetPropertyFloat(std::string name) {
	return static_cast<float>(strtod(properties[name].c_str(), NULL));
}

std::string Object::GetPropertyString(std::string name)//получить имя в виде строки.вроде понятно
{
	return properties[name];
}

bool Level::LoadFromFile(std::string filename)
{
	TiXmlDocument levelFile(filename.c_str());//loading file into TiXmlDocument

	// loading XML-map
	if (!levelFile.LoadFile())
	{
		std::cout << "Loading level \"" << filename << "\" failed." << std::endl;//выдаем ошибку
		return false;
	}

	// work with map container
	TiXmlElement* map;
	map = levelFile.FirstChildElement("map");

	// map example: <map version="1.0" orientation="orthogonal"
	// width="10" height="10" tilewidth="34" tileheight="34">
	width = atoi(map->Attribute("width"));//extrecting propreties
	height = atoi(map->Attribute("height"));
	tileWidth = atoi(map->Attribute("tilewidth"));
	tileHeight = atoi(map->Attribute("tileheight"));

	// Taking the description of the tileset and the indetifier of first tile
	TiXmlElement* tilesetElement;
	tilesetElement = map->FirstChildElement("tileset");
	firstTileID = atoi(tilesetElement->Attribute("firstgid"));

	// source - path to the image container
	TiXmlElement* image;
	image = tilesetElement->FirstChildElement("image");
	std::string imagepath = image->Attribute("source");

	// loading tileset
	sf::Image img;

	if (!img.loadFromFile(imagepath)) {
		std::cout << "Failed to load tile sheet." << std::endl;
		return false;
	}


	img.createMaskFromColor(sf::Color(255, 255, 255));//mask for color
	tilesetImage.loadFromImage(img);
	tilesetImage.setSmooth(false);//smooth

	// number of row and cols of tileset
	int columns = tilesetImage.getSize().x / tileWidth;
	int rows = tilesetImage.getSize().y / tileHeight;

	// vector of images (TextureRect)
	std::vector<sf::Rect<int>> subRects;

	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < columns; x++) {
			sf::Rect<int> rect;

			rect.top = y * tileHeight;
			rect.height = tileHeight;
			rect.left = x * tileWidth;
			rect.width = tileWidth;

			subRects.emplace_back(rect);
		}
	}
	// work with layers
	TiXmlElement* layerElement;
	layerElement = map->FirstChildElement("layer");
	while (layerElement) {
		Layer layer;

		// giving layer opacity
		if (layerElement->Attribute("opacity") != NULL) {
			float opacity = static_cast<float>(strtod(layerElement->Attribute("opacity"), NULL));
			layer.opacity = static_cast<int>(255 * opacity);
		}
		else {
			layer.opacity = 255;
		}

		//  <data> container
		TiXmlElement* layerDataElement;
		layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == NULL) {
			std::cout << "Bad map. No layer information found." << std::endl;
		}

		//  <tile> container - tile description
		TiXmlElement* tileElement;
		tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == NULL) {
			std::cout << "Bad map. No tile information found." << std::endl;
			return false;
		}

		int x = 0;
		int y = 0;

		while (tileElement) {
			int tileGID = atoi(tileElement->Attribute("gid"));
			int subRectToUse = tileGID - firstTileID;

			// Setting TextureRect for each tile
			if (subRectToUse >= 0) {
				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(subRects[subRectToUse]);
				sprite.setPosition(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight));
				sprite.setColor(sf::Color(255, 255, 255, layer.opacity));

				layer.tiles.emplace_back(sprite);//Placing tiles into lyaer
			}

			tileElement = tileElement->NextSiblingElement("tile");

			x++;
			if (x >= width) {
				x = 0;
				y++;
				if (y >= height)
					y = 0;
			}
		}

		layers.emplace_back(layer);

		layerElement = layerElement->NextSiblingElement("layer");
	}

	// Work with objects
	TiXmlElement* objectGroupElement;

	// If object layer found
	if (map->FirstChildElement("objectgroup") != NULL) {
		objectGroupElement = map->FirstChildElement("objectgroup");
		while (objectGroupElement) {
			//  <object> container
			TiXmlElement* objectElement;
			objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement) {
				// Getting all the props
				std::string objectType;
				if (objectElement->Attribute("type") != NULL) {
					objectType = objectElement->Attribute("type");
				}
				std::string objectName;
				if (objectElement->Attribute("name") != NULL) {
					objectName = objectElement->Attribute("name");
				}
				int x = atoi(objectElement->Attribute("x"));
				int y = atoi(objectElement->Attribute("y"));

				int width, height;

				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition(static_cast<float>(x), static_cast<float>(y));

				if (objectElement->Attribute("width") != NULL) {
					width = atoi(objectElement->Attribute("width"));
					height = atoi(objectElement->Attribute("height"));
				}
				else {
					width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
					height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
					sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - firstTileID]);
				}

				// Object instance
				Object object;
				object.name = objectName;
				object.type = objectType;
				object.sprite = sprite;

				sf::Rect <float> objectRect;
				objectRect.top = static_cast<float>(y);
				objectRect.left = static_cast<float>(x);
				objectRect.height = static_cast<float>(height);
				objectRect.width = static_cast<float>(width);
				object.rect = objectRect;

				// Object props
				TiXmlElement* properties;
				properties = objectElement->FirstChildElement("properties");
				if (properties != NULL) {
					TiXmlElement* prop;
					prop = properties->FirstChildElement("property");
					if (prop != NULL) {
						while (prop) {
							std::string propertyName = prop->Attribute("name");
							std::string propertyValue = prop->Attribute("value");

							object.properties[propertyName] = propertyValue;

							prop = prop->NextSiblingElement("property");
						}
					}
				}


				objects.push_back(object);

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else {
		std::cout << "No object layers found..." << std::endl;
	}

	return true;
}

Object Level::GetObject(std::string name) {
	// First found object
	for (unsigned int i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			return objects[i];
}

std::vector<Object> Level::GetObjects(std::string name) {
	// All found objects
	std::vector<Object> vec;
	for (unsigned int i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			vec.push_back(objects[i]);

	return vec;
}


std::vector<Object> Level::GetAllObjects() {
	return objects;
};


sf::Vector2i Level::GetTileSize() {
	return sf::Vector2i(tileWidth, tileHeight);
}

void Level::Draw(sf::RenderWindow& window) {
	// Drawing all tiles
	for (unsigned int layer = 0; layer < layers.size(); layer++)
		for (unsigned int tile = 0; tile < layers[layer].tiles.size(); tile++)
			window.draw(layers[layer].tiles[tile]);
}