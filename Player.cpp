#include "Player.h"

#include <iostream>

void Player::Turn(bool bLeftRight, float fDeltaTime)
{
    float fDirMultiplier = bLeftRight ? -1.0f : 1.0f;
    m_fTheta += fDirMultiplier * 90.0f * fDeltaTime;

    if (m_fTheta > 360.0) m_fTheta -= 360.0;
    if (m_fTheta < 0.0) m_fTheta += 360.0;

    m_sfPlayerRect.setRotation(m_fTheta);

    m_sfStepDelta.x = 2.0f * CELL_SIZE * std::cos(m_fTheta * M_PI / 180.0f);
    m_sfStepDelta.y = 2.0f * CELL_SIZE * std::sin(m_fTheta * M_PI / 180.0f);
}

void Player::MoveForward(bool bForwardBackward, float fDeltaTime) {
    float fDirMultiplier = bForwardBackward ? 1.0f : -1.0f;
    m_sfPos += fDirMultiplier * (m_sfStepDelta / CELL_SIZE) * fDeltaTime;

    m_sfPlayerRect.setPosition(m_sfPos * CELL_SIZE);
}

void Player::SetPosition(sf::Vector2f sfNewPos) {
    sfNewPos.x += (CELL_SIZE / 2.0f) - (PLAYER_SIZE / 2.0f);
    sfNewPos.y += (CELL_SIZE / 2.0f) - (PLAYER_SIZE / 2.0f);

    m_sfPlayerRect.setOrigin(sf::Vector2f(PLAYER_SIZE / 2.0f, PLAYER_SIZE / 2.0f));

    m_sfPlayerRect.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
    m_sfPlayerRect.setFillColor(sf::Color::Red);
    m_sfPlayerRect.setPosition(sfNewPos);

    m_sfPos = sfNewPos / CELL_SIZE;

    m_sfStepDelta = { 2.0f * CELL_SIZE, 0.0f };
}

std::pair<sf::Vector2f, float> Player::GetPositionAndAngle() {
    return std::pair<sf::Vector2f, float>(m_sfPos, m_fTheta);
}

sf::RectangleShape* Player::GetPlayerRect() {
    return &m_sfPlayerRect;
}
