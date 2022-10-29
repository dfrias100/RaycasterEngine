#ifndef PLAYER_H
#define PLAYER_H

#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Constants.h"

class Player {
private:
    sf::Vector2f m_sfPos;
    sf::RectangleShape m_sfPlayerRect;
    float m_fTheta = 0.0f;
public:
    void Turn(bool bLeftRight, float fDeltaTime);
    void MoveForward(bool bForwardBackward);
    void SetPosition(sf::Vector2f sfNewPos);
    sf::RectangleShape* GetPlayerRect();
};

#endif