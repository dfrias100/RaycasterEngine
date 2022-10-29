#include "Player.h"

void Player::Turn(bool bLeftRight, float fDeltaTime)
{
    float fMultiplier = bLeftRight ? -1.0f : 1.0f;
    m_fTheta += fMultiplier * 90.0f * fDeltaTime;

    if (m_fTheta > 360.0) m_fTheta -= 360.0;
    if (m_fTheta < 0.0) m_fTheta += 360.0;

    m_sfPlayerRect.setRotation(m_fTheta);
}

void Player::MoveForward(bool bForwardBackward)
{
}

void Player::SetPosition(sf::Vector2f sfNewPos) {
    sfNewPos.x += (CELL_SIZE / 2.0f) - (PLAYER_SIZE / 2.0f);
    sfNewPos.y += (CELL_SIZE / 2.0f) - (PLAYER_SIZE / 2.0f);
    
    m_sfPlayerRect.setOrigin(sf::Vector2f(PLAYER_SIZE / 2.0f, PLAYER_SIZE / 2.0f));

    m_sfPlayerRect.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
    m_sfPlayerRect.setFillColor(sf::Color::Red);
    m_sfPlayerRect.setPosition(sfNewPos);
}

sf::RectangleShape* Player::GetPlayerRect()
{
    return &m_sfPlayerRect;
}
