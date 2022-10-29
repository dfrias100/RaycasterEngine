#include "Raycast.h"

bool Raycast::OnInitialize() {
    MapLoader::LoadMap("map1.txt");

    sf::Vector2i sfTexSize(MapLoader::sm_sfBoundingBox.x * CELL_SIZE, MapLoader::sm_sfBoundingBox.y * CELL_SIZE);
    
    if (!m_sfMiniMapTexture.create(sfTexSize.x, sfTexSize.y)) {
	return false;
    }

    sf::Vector2f sfCurrPos;

    sfCurrPos.y = (MapLoader::sm_sfBoundingBox.y - 1) * CELL_SIZE;

    for (std::vector<MapLoader::TileTypes>& vRow : MapLoader::sm_vMap) 
    {
	for (MapLoader::TileTypes mtyTile : vRow) 
	{
	    switch (mtyTile) {
	    case MapLoader::TileTypes::Wall: 
	    {
		m_sfMiniMapWalls.push_back(sf::RectangleShape());
		sf::RectangleShape& refShape = m_sfMiniMapWalls[m_sfMiniMapWalls.size() - 1];
		refShape.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
		refShape.setOutlineColor(sf::Color(127, 127, 127));
		refShape.setOutlineThickness(2.0f);
		refShape.setFillColor(sf::Color::White);
		refShape.setPosition(sfCurrPos.x + 1, sfCurrPos.y + 1);
	    }
		break;
	    case MapLoader::TileTypes::PlayerStart:
		break;
	    case MapLoader::TileTypes::Floor:
	    default:
		break;
	    }

	    sfCurrPos.x += CELL_SIZE;
	}

	sfCurrPos.x = 0.0f;
	sfCurrPos.y -= CELL_SIZE;
    }

    for (auto& sfShapes : m_sfMiniMapWalls)
	m_sfMiniMapTexture.draw(sfShapes);

    return true;
}

bool Raycast::OnUpdate(float fFrameTime) {
    const sf::Texture& renderTexture = m_sfMiniMapTexture.getTexture();
    m_sfMiniMapSprite.setTexture(renderTexture);
    auto texDims = renderTexture.getSize();
    sf::Vector2f sfNewScale;
    sfNewScale.x = 800.0f / texDims.x;
    sfNewScale.y = 600.0f / texDims.y;
    m_sfMiniMapSprite.setScale(sfNewScale);
    PushDrawableObject(&m_sfMiniMapSprite);
    return true;
}

Raycast::Raycast()
    : GameApp("Raycast Engine", FPSControl::Locked60) 
{
}