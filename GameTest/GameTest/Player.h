#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

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

    float m_sprintTimeout;
    float m_distanceAbove;
    float m_Speed;
    bool m_targetReached;
    bool m_Moving;
    float m_indicatorBaseHalf = 10.f;
    float m_indicatorHeight = 15.f;
    float m_sprintPower;
    bool m_isSprinting;
    bool m_enemyHit;
    CollisionSide m_collidedSide;

private:
    void move(const sf::Vector2f& offset, const sf::RectangleShape& other);

    void setCollidedSide(const sf::RectangleShape& other);

    void updateIndicatorPos();

    bool hasReachedTarget() const;

    void resetSprint();

public:
    Player(sf::Vector2f rectSize, sf::Color rectColor, sf::Color indicatorColor, float distanceAbove, float speed, float sprintPower, float sprintTimeout);

    CollisionSide& getCollidedSide();

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
};