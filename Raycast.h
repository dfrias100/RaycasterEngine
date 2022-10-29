#ifndef RAYCAST_H
#define RAYCAST_H

#include "GameApp.h"
#include "MapLoader.h"

#include <vector>

static constexpr float CELL_SIZE = 64.0f;

class Raycast : public GameApp {
private:
    sf::RenderTexture m_sfMiniMapTexture;
    sf::Sprite m_sfMiniMapSprite;
    std::vector<sf::RectangleShape> m_sfMiniMapWalls;
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