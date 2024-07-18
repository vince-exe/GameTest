#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "utils.h"

class Player : public sf::Drawable {
public:
    enum class CollisionSide {
        None,
        Top,
        Bottom,
        Left,
        Right
    };
    
protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape m_Rectangle;
    sf::Vector2f m_targetPosition;
    sf::ConvexShape m_Indicator;
    sf::Clock m_sprintClock;

    bool m_drawVertexLine;
    /* this is used to have a more precise player movement */
    std::shared_ptr<sf::VertexArray> m_vertexLine;

    float m_sprintTimeout, m_distanceAbove, m_sprintPower;
    float m_Speed;
    bool m_targetReached, m_Moving, m_isSprinting, m_enemyHit, m_hitByEnemy;
    float m_indicatorBaseHalf = 10.f;
    float m_indicatorHeight = 15.f;
    CollisionSide m_collidedSide;

private:
    void move(const sf::Vector2f& offset, const sf::RectangleShape& other);

    void setCollidedSide(const sf::RectangleShape& other);

    void updateIndicatorPos();

    void calcPlayerTrend(const sf::Vector2f& newPos);

public:
    Player();

    CollisionSide& getCollidedSide();

    void resetSprint();

    void handlePlayerMovement(std::atomic<bool> actionsBlocked, sf::RenderWindow& window, bool wantSprint);

    void setSize(sf::Vector2f size);

    void setColor(sf::Color color);

    void setIndicator(sf::Color color, float distance);

    void setSpeed(float speed);

    void setSprint(float power, float timeout);

    void setDebugMode(bool flag);

    void handleEnemyCollision(const CollisionSide collidionSide);

    void update(sf::Time deltaTime, const sf::RectangleShape& other);

    bool canSprint();

    void startSprint(float flag);

    void stopSprint();

    bool isSprinting();

    float getVelocity();

    void setTarget(const sf::Vector2f& target);

    bool isMoving() const;

    void stopMove();

    sf::RectangleShape& getRect();

    void setPosition(sf::Vector2f pos);

    void setPosition(float x, float y);

    sf::Vector2f getPosition() const;

    bool intersect(const sf::RectangleShape& rect);

    sf::FloatRect getGlobalBounds() const;

    sf::Clock getClock();

    float getSprintTimeout();

    bool isEnemyHit();

    void resetEnemyHit();

    void setHitByEnemy(bool flag);
};