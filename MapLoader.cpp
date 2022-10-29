#include "MapLoader.h"

bool MapLoader::sm_bBoundingBoxChanged;

std::vector<std::vector<MapLoader::TileTypes>> MapLoader::sm_vMap;

sf::Vector2f MapLoader::sm_sfPlayerCenter;

sf::Vector2f MapLoader::sm_sfBoundingBox;

std::vector<std::vector<MapLoader::TileTypes>>& MapLoader::LoadMap(std::string szFilePath) {
    std::ifstream ifsMapFile(szFilePath);
    std::string szLineRepresentation;

    sm_vMap.clear();

    sm_sfPlayerCenter.x = 0;
    sm_sfPlayerCenter.y = 0;

    sm_sfBoundingBox.x = 0;
    sm_sfBoundingBox.y = 0;
    int xAux = 0;

    while (std::getline(ifsMapFile, szLineRepresentation)) 
    {
	std::vector<MapLoader::TileTypes> vRow;

	for (char cTile : szLineRepresentation) 
	{
	    if (cTile == '#') 
	    {
		vRow.push_back(MapLoader::TileTypes::Wall);
	    }
	    else if (cTile == '.')
	    {
		vRow.push_back(MapLoader::TileTypes::Floor);
	    }
	    else if (cTile == 'P')
	    {
		vRow.push_back(MapLoader::TileTypes::PlayerStart);
		sm_sfPlayerCenter = sm_sfBoundingBox;
	    }
	    
	    if (++xAux > sm_sfBoundingBox.x)
		sm_sfBoundingBox.x++;
	}

	sm_vMap.push_back(vRow);
	xAux = 0;
	sm_sfBoundingBox.y++;
    }

    return sm_vMap;
}
