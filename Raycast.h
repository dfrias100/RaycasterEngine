#ifndef RAYCAST_H
#define RAYCAST_H

#include <vector>

#include "Constants.h"
#include "GameApp.h"
#include "MapLoader.h"
#include "Player.h"

class Raycast : public GameApp {
private:
    sf::RenderTexture m_sfMiniMapTexture;
    sf::Sprite m_sfMiniMapSprite;
    std::vector<sf::RectangleShape> m_sfMiniMapWalls;

    Player m_Player;

    void DrawMapToTexture();
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