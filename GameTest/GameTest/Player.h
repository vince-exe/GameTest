#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Player : public sf::Drawable {
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape rectangle;
    sf::Vector2f targetPosition;
    sf::ConvexShape indicator;
    float distanceAbove;
    float speed;
    bool targetReached;
    bool moving;
    float indicatorBaseHalf = 10.f;
    float indicatorHeight = 15.f;

public:
    Player(sf::Vector2f rectSize, sf::Color rectColor, sf::Color indicatorColor, float distanceAbove, float speed);

    void update(sf::Time deltaTime);

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

    bool intersect(sf::RectangleShape& rect);

    void move(const sf::Vector2f& offset);

    sf::FloatRect getGlobalBounds() const;
};