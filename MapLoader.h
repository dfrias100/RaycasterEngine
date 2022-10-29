#ifndef MAPLOADER_H
#define MAPLOADER_H

#include <fstream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

class MapLoader {
private:
    static bool sm_bBoundingBoxChanged;
public:
    enum class TileTypes {
	Wall,
	Floor,
	PlayerStart
    };
    
    static std::vector<std::vector<TileTypes>> sm_vMap;
    static sf::Vector2f sm_sfPlayerCenter;
    static sf::Vector2f sm_sfBoundingBox;

    static std::vector<std::vector<TileTypes>>& LoadMap(std::string szFilePath);
};

#endif