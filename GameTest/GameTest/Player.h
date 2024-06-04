#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Player : public sf::Drawable  {
private:
	sf::RectangleShape rectangle;
	sf::ConvexShape indicator;

	float indicatorHeight = 20.f;
	float indicatorBaseHalf = 10.f;
	float distanceAbove;
	float movementVelocity;
	sf::Sprite s;
	
private:
	void updateIndicatorPos();

public:
	Player(sf::Vector2f rectSize, sf::Color rectColor, sf::Color indicatorColor, float distanceAbove, float movementVelocity);

	float getVelocity();

	sf::RectangleShape& getRect();

	void setPosition(sf::Vector2f pos);

	void setPosition(float x, float y);

	void move(const sf::Vector2f& offset);

	sf::Vector2f getPosition();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

