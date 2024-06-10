#include "Player.h"

Player::Player(sf::Vector2f rectSize, sf::Color rectColor, sf::Color indicatorColor, float distanceAbove, float speed, float sprintPower, float sprintTimeout) {
    this->rectangle.setSize(rectSize);

    this->rectangle.setFillColor(rectColor);

    this->indicator.setPointCount(3);
    this->indicator.setFillColor(indicatorColor);

    this->distanceAbove = distanceAbove;
    this->speed = speed;

    this->indicator.setPoint(0, sf::Vector2f(-this->indicatorBaseHalf, 0.f));
    this->indicator.setPoint(1, sf::Vector2f(this->indicatorBaseHalf, 0.f));
    this->indicator.setPoint(2, sf::Vector2f(0.f, this->indicatorHeight));

    this->sprintTimeout = sprintTimeout;
    this->sprintPower = sprintPower;
    this->m_isSprinting = false;
    this->targetReached = true;
    this->moving = false;
}

void Player::update(sf::Time deltaTime, const sf::RectangleShape& other) {
    if (!targetReached) {
        sf::Vector2f direction = targetPosition - getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length > 0) {
            direction /= length;

            sf::Vector2f movement = direction * speed * deltaTime.asSeconds();

            if (length < speed * deltaTime.asSeconds()) {
                setPosition(targetPosition);
                stopMove();
            }
            else {
                if (hasReachedTarget()) {
                    stopMove();
                }
                else {
                    move(movement, other);
                }
            }
        }
    }
}

void Player::move(const sf::Vector2f& offset, const sf::RectangleShape& other) {
    sf::Vector2f newPosition = this->rectangle.getPosition() + offset;
    this->rectangle.setPosition(newPosition);

    // check collision
    if (this->intersect(other)) {
        this->rectangle.setPosition(this->rectangle.getPosition() - offset);
        this->enemyHit = true;
    }
    else {
        this->updateIndicatorPos();
    }
    this->moving = true;
}

void Player::startSprint() {
    this->sprintClock.restart();
    this->m_isSprinting = true;
    this->speed += this->sprintPower;
}

void Player::stopSprint() {
    this->m_isSprinting = false;
    this->speed -= this->sprintPower;
}

bool Player::canSprint() {
    return (this->sprintClock.getElapsedTime().asSeconds() >= this->sprintTimeout);
}

bool Player::isSprinting() {
    return this->m_isSprinting;
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
    
    if (this->m_isSprinting) {
        this->stopSprint();
    }
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

bool Player::intersect(const  sf::RectangleShape& rect) {
    return this->rectangle.getGlobalBounds().intersects(rect.getGlobalBounds());
}

void Player::resetSprint() {
    this->sprintClock.restart();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(this->rectangle, states);
    target.draw(this->indicator, states);
}

sf::FloatRect Player::getGlobalBounds() const {
    return this->rectangle.getGlobalBounds();
}

sf::Clock Player::getClock() {
    return this->sprintClock;
}

float Player::getSprintTimeout() {
    return this->sprintTimeout;
}

bool Player::isEnemyHit() {
    return this->enemyHit;
}

void Player::resetEnemyHit() {
    this->enemyHit = false;
}
