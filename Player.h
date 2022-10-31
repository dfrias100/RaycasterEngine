#ifndef PLAYER_H
#define PLAYER_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <utility>

#include <SFML/Graphics.hpp>

#include "Constants.h"

class Player {
private:
    sf::Vector2f m_sfPos;
    sf::Vector2f m_sfStepDelta;
    sf::RectangleShape m_sfPlayerRect;
    float m_fTheta = 0.0f;
public:
    void Turn(bool bLeftRight, float fDeltaTime);
    void MoveForward(bool bForwardBackward, float fDeltaTime);
    void SetPosition(sf::Vector2f sfNewPos);
    std::pair<sf::Vector2f, float> GetPositionAndAngle();
    sf::RectangleShape* GetPlayerRect();
};

#endif