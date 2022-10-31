#include "Raycast.h"

#include <iostream>

bool Raycast::OnInitialize() {
    MapLoader::LoadMap("map1.txt");

    sf::Vector2i sfTexSize(MapLoader::sm_sfBoundingBox.x * CELL_SIZE, MapLoader::sm_sfBoundingBox.y * CELL_SIZE);
    
    if (!m_sfWallTexture.loadFromFile("walls.png")) {
	return false;
    }

    if (!m_sfMiniMapTexture.create(sfTexSize.x, sfTexSize.y)) {
	return false;
    }

    if (!m_sfViewTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT)) {
	return false;
    }

    m_sfViewSprite.setPosition(SCREEN_WIDTH, 0);

    for (sf::RectangleShape& sfRect : m_asfViewWalls) {
	sfRect.setFillColor(sf::Color(180, 180, 180));
	sfRect.setSize(sf::Vector2f(SCREEN_WIDTH / NUM_RAYS, SCREEN_HEIGHT));
	sfRect.setTexture(&m_sfWallTexture);
    }

    for (sf::RectangleShape& sfRect : m_asfViewWallsFog) {
	sfRect.setFillColor(sf::Color(0, 255, 255));
	sfRect.setSize(sf::Vector2f(SCREEN_WIDTH / NUM_RAYS, SCREEN_HEIGHT));
    }

    sf::Vector2f sfCurrPos;

    for (std::vector<MapLoader::TileTypes>& vRow : MapLoader::sm_vMap)
    {
	for (MapLoader::TileTypes mtyTile : vRow)
	{
	    switch (mtyTile) 
	    {
		case MapLoader::TileTypes::Wall:
		{
		    m_sfMiniMapWalls.push_back(sf::RectangleShape());
		    sf::RectangleShape& refShape = m_sfMiniMapWalls[m_sfMiniMapWalls.size() - 1];
		    refShape.setSize(sf::Vector2f(CELL_SIZE - 1.5, CELL_SIZE - 1.5));
		    refShape.setOutlineColor(sf::Color(127, 127, 127));
		    refShape.setOutlineThickness(2.0f);
		    refShape.setFillColor(sf::Color(180, 180, 180));
		    refShape.setPosition(sfCurrPos.x + 1, sfCurrPos.y + 1);
		}
		break;

		case MapLoader::TileTypes::PlayerStart:
		    m_Player.SetPosition(sfCurrPos);
		    break;

		case MapLoader::TileTypes::Floor:
		default:
		    break;
	    }

	    sfCurrPos.x += CELL_SIZE;
	}

	sfCurrPos.x = 0.0f;
	sfCurrPos.y += CELL_SIZE;
    }

    sf::Vector2f sfNewScale;
    sfNewScale.x = SCREEN_WIDTH / sfTexSize.x;
    sfNewScale.y = SCREEN_HEIGHT / SCREEN_WIDTH;

    float fyNewPos = (SCREEN_HEIGHT - sfTexSize.y * sfNewScale.y) * 0.5f;

    m_sfMiniMapSprite.setScale(sfNewScale);
    m_sfMiniMapSprite.setPosition(0.0f, fyNewPos);

    return true;
}

bool Raycast::OnUpdate(float fFrameTime) {
    if (GetKeyStatus(sf::Keyboard::Left) == KeyStatus::Pressed) m_Player.Turn(true, fFrameTime);
    if (GetKeyStatus(sf::Keyboard::Right) == KeyStatus::Pressed) m_Player.Turn(false, fFrameTime);
    if (GetKeyStatus(sf::Keyboard::Up) == KeyStatus::Pressed) m_Player.Move(true, fFrameTime);
    if (GetKeyStatus(sf::Keyboard::Down) == KeyStatus::Pressed) m_Player.Move(false, fFrameTime);

    CastRays();

    DrawMapToTexture();
    DrawWorldToTexture();

    const sf::Texture& sfMiniMapRenderTexture = m_sfMiniMapTexture.getTexture();
    const sf::Texture& sfViewRenderTexture = m_sfViewTexture.getTexture();

    m_sfMiniMapSprite.setTexture(sfMiniMapRenderTexture);
    m_sfViewSprite.setTexture(sfViewRenderTexture);
    
    PushDrawableObject(&m_sfMiniMapSprite);
    PushDrawableObject(&m_sfViewSprite);

    return true;
}

void Raycast::DrawMapToTexture() {
    m_sfMiniMapTexture.clear();

    for (auto& sfShapes : m_sfMiniMapWalls)
	m_sfMiniMapTexture.draw(sfShapes);
    m_sfMiniMapTexture.draw(*m_Player.GetPlayerRect());

    m_sfMiniMapTexture.draw(&m_vsfRayVertices[0], m_vsfRayVertices.size(), sf::TriangleFan);

    m_sfMiniMapTexture.display();
}

void Raycast::DrawWorldToTexture() {
    m_sfViewTexture.clear();

    sf::RectangleShape sfGround;
    sf::RectangleShape sfSky;

    sfSky.setSize(sf::Vector2f(SCREEN_WIDTH, 0.5f * SCREEN_HEIGHT));
    sfSky.setFillColor(sf::Color(0, 255, 255));

    sfGround.setSize(sf::Vector2f(SCREEN_WIDTH, 0.5f * SCREEN_HEIGHT));
    sfGround.setPosition(sf::Vector2f(0.0f, 0.5f * SCREEN_HEIGHT));
    sfGround.setFillColor(sf::Color(0, 180, 64));

    m_sfViewTexture.draw(sfGround);
    m_sfViewTexture.draw(sfSky);

    float fPlayerAngle = m_Player.GetPositionAndAngle().second;

    float fRecipTan = 0.5f / std::tan(DegToRad(0.5f * FOV));

    float fViewRectHeight;
    float fAngleDiff;

    float fRayProjectionPos = fRecipTan * std::tan(DegToRad(0.5f * FOV));
    float fScrPos = std::round(SCREEN_WIDTH * (0.5f - fRayProjectionPos));
    float fNextScrPos;

    float fRayAngle = fPlayerAngle - (FOV * 0.5f);
    SetEquivalentAngle(fRayAngle);

    for (size_t i = 0; i < NUM_RAYS; i++, fRayAngle += DELTA_THETA) {
	fAngleDiff = fPlayerAngle - fRayAngle;
	SetEquivalentAngle(fAngleDiff);

	m_aDistances[i] *= std::cos(DegToRad(fAngleDiff));

	float fLerp = std::round(std::min(8.0f, m_aDistances[i]));
	fLerp = 1.0f - (std::log((8.0f - fLerp) + 1.0f) / std::log(9.0f));

	sf::Color sfFogTransparency = m_asfViewWallsFog[i].getFillColor();
	sfFogTransparency.a = fLerp * 255.0f;

	m_asfViewWallsFog[i].setFillColor(sfFogTransparency);

	fAngleDiff -= DELTA_THETA;
	fRayProjectionPos = fRecipTan * std::tan(DegToRad(fAngleDiff));
	fNextScrPos = std::round(SCREEN_WIDTH * (0.5f - fRayProjectionPos));

	fViewRectHeight = RECT_HEIGHT / m_aDistances[i];

	sf::Vector2f sfCurrSize = m_asfViewWalls[i].getSize();
	sf::Vector2f sfCurrPos = m_asfViewWalls[i].getPosition();

	sfCurrSize.x = std::max(sfCurrSize.x, fNextScrPos - fScrPos);
	sfCurrSize.y = fViewRectHeight;

	sfCurrPos.x = fScrPos;
	sfCurrPos.y = (SCREEN_HEIGHT - sfCurrSize.y) / 2.0f;

	m_asfViewWalls[i].setSize(sfCurrSize);
	m_asfViewWalls[i].setPosition(sfCurrPos);

	m_asfViewWallsFog[i].setSize(sfCurrSize);
	m_asfViewWallsFog[i].setPosition(sfCurrPos);

	fScrPos = fNextScrPos;

	m_sfViewTexture.draw(m_asfViewWalls[i]);
	m_sfViewTexture.draw(m_asfViewWallsFog[i]);
    }

    m_sfViewTexture.display();
}

void Raycast::CastRays() {
    PlayerData plyrData = m_Player.GetPositionAndAngle();

    sf::Vector2f sfHorizVector;
    sf::Vector2f sfVertVector;
    sf::Vector2f sfFinalVector;

    m_vsfRayVertices.clear();
    m_vsfRayVertices.push_back(plyrData.first * CELL_SIZE);

    plyrData.second -= FOV * 0.5f;
    SetEquivalentAngle(plyrData.second);

    for (size_t i = 0; i < NUM_RAYS; i++) {
	sfHorizVector = CastOneRay(true, plyrData);
	sfVertVector = CastOneRay(false, plyrData);

	float fDistH = Distance(sfHorizVector);
	float fDistV = Distance(sfVertVector);

	if (fDistH < fDistV) 
	{
	    sfFinalVector = sfHorizVector + plyrData.first;
	    m_aCollisions[i] = RayCollisionType::Horizontal;
	    m_aDistances[i] = fDistH;
	    MapTexture(sfFinalVector.x, m_asfViewWalls[i]);
	}
	else 
	{
	    sfFinalVector = sfVertVector + plyrData.first;
	    m_aCollisions[i] = RayCollisionType::Vertical;
	    m_aDistances[i] = fDistV;
	    MapTexture(sfFinalVector.y, m_asfViewWalls[i]);
	}
	
	m_vsfRayVertices.push_back(sfFinalVector * CELL_SIZE);

	plyrData.second += DELTA_THETA;
	SetEquivalentAngle(plyrData.second);
    }
}

inline sf::Vector2f Raycast::CastOneRay(bool bHorizontal, PlayerData& plyrData)
{
    int nDepthOfField = 0;
   
    std::vector<bool> vbPredicates;
    vbPredicates.resize(3);

    float fTrigVal = 0.0f;

    sf::Vector2f sfOffset;
    sf::Vector2f sfRayIntersection;
    sf::Vector2i sfMapLocation;

    bool bRunLoop = true;

    if (bHorizontal) {
	vbPredicates[0] = plyrData.second > 180.0f;
	vbPredicates[1] = plyrData.second < 180.0f;
	vbPredicates[2] = plyrData.second == 0.0f || plyrData.second == 180.0f;
	fTrigVal = -1.0 / (float)std::tan(DegToRad(plyrData.second));
    } else {
	vbPredicates[0] = plyrData.second > 90.0f && plyrData.second < 270.0f;
	vbPredicates[1] = plyrData.second < 90.0f || plyrData.second > 270.0f;
	vbPredicates[2] = plyrData.second == 90.0f || plyrData.second == 270.0f;
	fTrigVal = -std::tan(DegToRad(plyrData.second));
	std::swap(plyrData.first.x, plyrData.first.y);
    }

    if (vbPredicates[0])
    {
	sfRayIntersection.y = std::floor(plyrData.first.y) - 0.00001f;
	sfRayIntersection.x = (plyrData.first.y - sfRayIntersection.y) * fTrigVal + plyrData.first.x;
	sfOffset.y = -1.0f;
	sfOffset.x = -sfOffset.y * fTrigVal;
    }
    else if (vbPredicates[1])
    {
	sfRayIntersection.y = std::floor(plyrData.first.y) + 1.0f;
	sfRayIntersection.x = (plyrData.first.y - sfRayIntersection.y) * fTrigVal + plyrData.first.x;
	sfOffset.y = 1.0f;
	sfOffset.x = -sfOffset.y * fTrigVal;
    }
    else if (vbPredicates[2])
    {
	sfRayIntersection = { INFINITY, INFINITY };
	bRunLoop = false;
    }

    if (!bHorizontal) {
	std::swap(sfRayIntersection.y, sfRayIntersection.x);
	std::swap(plyrData.first.x, plyrData.first.y);
	std::swap(sfOffset.x, sfOffset.y);
    }

    while (nDepthOfField < 8 && bRunLoop) {
	sfMapLocation = { (int)sfRayIntersection.x, (int)sfRayIntersection.y };

	if (sfMapLocation.y >= 0
	    && sfMapLocation.x >= 0
	    && sfMapLocation.y < MapLoader::sm_vMap.size()
	    && sfMapLocation.x < MapLoader::sm_vMap[sfMapLocation.y].size()
	    && MapLoader::sm_vMap[sfMapLocation.y][sfMapLocation.x] == MapLoader::TileTypes::Wall) {
	    bRunLoop = false;
	}
	else
	{
	    sfRayIntersection += sfOffset;
	    nDepthOfField++;
	}
    }

    return sfRayIntersection - plyrData.first;
}

inline float Raycast::Distance(sf::Vector2f sfVec) {
    return std::sqrt(sfVec.x * sfVec.x + sfVec.y * sfVec.y);
}

float Raycast::DegToRad(float fDeg) {
    return fDeg * M_PI / 180.0f;
}

void Raycast::SetEquivalentAngle(float& fDeg) {
    if (fDeg > 360.0f) fDeg -= 360.0f;
    if (fDeg < 0.0f) fDeg += 360.0f;
}

void Raycast::MapTexture(float fPos, sf::RectangleShape& sfRect) {
    float fOffset = std::ceil(std::round(fPos * CELL_SIZE) / CELL_SIZE) * CELL_SIZE;
    fOffset = fOffset - fPos * CELL_SIZE;
    sf::IntRect sfWallRect(fOffset, 0, std::ceil(CELL_SIZE / NUM_RAYS), CELL_SIZE);
    sfRect.setTextureRect(sfWallRect);
}

Raycast::Raycast()
    : GameApp(sf::VideoMode(1600, 600), "Raycast Engine", FPSControl::Locked30) 
{
}
