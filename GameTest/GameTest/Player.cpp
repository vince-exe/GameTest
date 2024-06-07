#include "Player.h"

Player::Player(sf::Vector2f rectSize, sf::Color rectColor, sf::Color indicatorColor, float distanceAbove, float speed) {
    this->rectangle.setSize(rectSize);

    this->rectangle.setFillColor(rectColor);

    this->indicator.setPointCount(3);
    this->indicator.setFillColor(indicatorColor);

    this->distanceAbove = distanceAbove;
    this->speed = speed;

    this->indicator.setPoint(0, sf::Vector2f(-this->indicatorBaseHalf, 0.f));
    this->indicator.setPoint(1, sf::Vector2f(this->indicatorBaseHalf, 0.f));
    this->indicator.setPoint(2, sf::Vector2f(0.f, this->indicatorHeight));

    this->targetReached = true;
    this->moving = false;
}

void Player::update(sf::Time deltaTime) {
    if (!targetReached) {
        sf::Vector2f direction = targetPosition - getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length > 0) {
            direction /= length;
            sf::Vector2f movement = direction * speed * deltaTime.asSeconds();

            if (length < speed * deltaTime.asSeconds()) {
                setPosition(targetPosition);
                moving = false;
                targetReached = true;
            }
            else {
                if (hasReachedTarget()) {
                    stopMove();
                }
                else {
                    move(movement);
                }
            }
        }
    }
}

float Player::getVelocity() {
    return this->speed;
}

void Player::updateIndicatorPos() {
    this->indicator.setPosition(this->rectangle.getPosition().x + this->rectangle.getSize().x / 2, this->rectangle.getPosition().y - this->distanceAbove - this->indicatorHeight);
}

void Player::setTarget(const sf::Vector2f& target) {
    this->targetPosition = target;
    this->targetReached = false;
    this->moving = true;
}

bool Player::isMoving() const {
    return this->moving;
}

void Player::stopMove() {
    this->targetReached = true;
    this->moving = false;
}

sf::RectangleShape& Player::getRect() {
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

sf::Vector2f Player::getPosition() const {
    return this->rectangle.getPosition();
}

bool Player::hasReachedTarget() const {
    float epsilon = 1.0f;
    sf::Vector2f position = getPosition();
    return (std::abs(position.x - targetPosition.x) < epsilon && std::abs(position.y - targetPosition.y) < epsilon);
}

bool Player::intersect(sf::RectangleShape& rect) {
    return this->rectangle.getGlobalBounds().intersects(rect.getGlobalBounds());
}

void Player::move(const sf::Vector2f& offset) {
    this->rectangle.move(offset);
    this->updateIndicatorPos();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(this->rectangle, states);
    target.draw(this->indicator, states);
}

sf::FloatRect Player::getGlobalBounds() const {
    return this->rectangle.getGlobalBounds();
}