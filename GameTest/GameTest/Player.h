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
    sf::RectangleShape rectangle;
    sf::Vector2f targetPosition;
    sf::ConvexShape indicator;
    sf::Clock sprintClock;

    float sprintTimeout;
    float distanceAbove;
    float speed;
    bool targetReached;
    bool moving;
    float indicatorBaseHalf = 10.f;
    float indicatorHeight = 15.f;
    float sprintPower;
    bool m_isSprinting;
    bool enemyHit;
    CollisionSide collidedSide;

private:
    void move(const sf::Vector2f& offset, const sf::RectangleShape& other);

public:
    Player(sf::Vector2f rectSize, sf::Color rectColor, sf::Color indicatorColor, float distanceAbove, float speed, float sprintPower, float sprintTimeout);

    CollisionSide& getCollidedSide();

    void checkCollidedSide(const sf::RectangleShape& other);

    void update(sf::Time deltaTime, const sf::RectangleShape& other);

    bool canSprint();

    void startSprint();

    void stopSprint();

    bool isSprinting();

    float getVelocity();

    void updateIndicatorPos();

    void setTarget(const sf::Vector2f& target);

    bool isMoving() const;

    void stopMove();

    sf::RectangleShape& getRect();

    void setPosition(sf::Vector2f pos);

    void setPosition(float x, float y);

    sf::Vector2f getPosition() const;

    bool hasReachedTarget() const;

    bool intersect(const sf::RectangleShape& rect);

    void resetSprint();

    sf::FloatRect getGlobalBounds() const;

    sf::Clock getClock();

    float getSprintTimeout();
    
    bool isEnemyHit();
    
    void resetEnemyHit();

};