#include "Player.h"

void Player::updateIndicatorPos() {
	this->indicator.setPosition(this->rectangle.getPosition().x + this->rectangle.getSize().x / 2, this->rectangle.getPosition().y - this->distanceAbove - this->indicatorHeight);
}

Player::Player(sf::Vector2f rectSize, sf::Color rectColor, sf::Color indicatorColor, float distanceAbove) {
	this->rectangle.setSize(rectSize);
	this->rectangle.setFillColor(rectColor);

	this->indicator.setPointCount(3);
	this->indicator.setFillColor(indicatorColor);

	this->distanceAbove = distanceAbove;
	this->indicator.setPoint(0, sf::Vector2f(-this->indicatorBaseHalf, 0.f));
	this->indicator.setPoint(1, sf::Vector2f(this->indicatorBaseHalf, 0.f));
	this->indicator.setPoint(2, sf::Vector2f(0.f, this->indicatorHeight));
}

sf::RectangleShape Player::getRect() {
	return this->rectangle;
}

void Player::setPosition(sf::Vector2f pos) {
	this->rectangle.setPosition(pos);
	this->updateIndicatorPos();
}

void Player::setPosition(float x, float y) {
	this->rectangle.setPosition(x, y);
	this->updateIndicatorPos();
}

sf::Vector2f Player::getPosition() {
	return this->rectangle.getPosition();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->rectangle, states);
	target.draw(this->indicator, states);
}
