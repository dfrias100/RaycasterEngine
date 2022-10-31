#ifndef RAYCAST_H
#define RAYCAST_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <array>

#include "Constants.h"
#include "GameApp.h"
#include "MapLoader.h"
#include "Player.h"

typedef std::pair<sf::Vector2f, float> PlayerData;

class Raycast : public GameApp {
private:
    enum class RayCollisionType {
	Vertical,
	Horizontal
    };

    sf::RenderTexture m_sfMiniMapTexture;
    sf::Sprite m_sfMiniMapSprite;

    sf::RenderTexture m_sfViewTexture;
    sf::Sprite m_sfViewSprite;

    sf::Texture m_sfWallTexture;

    std::vector<sf::RectangleShape> m_sfMiniMapWalls;
    std::vector<sf::Vertex> m_vsfRayVertices;
    std::array<sf::RectangleShape, NUM_RAYS> m_asfViewWalls;
    std::array<sf::RectangleShape, NUM_RAYS> m_asfViewWallsFog;
    std::array<RayCollisionType, NUM_RAYS> m_aCollisions;
    std::array<float, NUM_RAYS> m_aDistances;

    Player m_Player;

    void DrawMapToTexture();
    void DrawWorldToTexture();
    void CastRays();
    inline sf::Vector2f CastOneRay(bool bHorizontal, PlayerData& plyrData);
    inline float Distance(sf::Vector2f sfVec);
    float DegToRad(float fDeg);
    void SetEquivalentAngle(float& fDeg);
    void MapTexture(float fPos, sf::RectangleShape& sfRect);

protected:
    bool OnInitialize() override;
    bool OnUpdate(float fFrameTime) override;
public:
    Raycast();

    Raycast(Raycast&&) = delete;
    Raycast(const Raycast&) = delete;
    Raycast& operator=(const Raycast&) = delete;
    Raycast& operator=(Raycast&&) = delete;
};

#endif