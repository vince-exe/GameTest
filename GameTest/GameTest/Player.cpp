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

Player::CollisionSide& Player::getCollidedSide() {
    return this->collidedSide;
}

void Player::setCollidedSide(const sf::RectangleShape& other) {
    sf::FloatRect intersection;
    if (this->rectangle.getGlobalBounds().intersects(other.getGlobalBounds(), intersection)) {
        // Calculate overlap in both directions
        float overlapLeft = this->rectangle.getPosition().x + this->rectangle.getSize().x - other.getPosition().x;
        float overlapRight = other.getPosition().x + other.getSize().x - this->rectangle.getPosition().x;
        float overlapTop = this->rectangle.getPosition().y + this->rectangle.getSize().y - other.getPosition().y;
        float overlapBottom = other.getPosition().y + other.getSize().y - this->rectangle.getPosition().y;

        // Determine the minimum overlap direction
        float minOverlapX = std::min(overlapLeft, overlapRight);
        float minOverlapY = std::min(overlapTop, overlapBottom);

        if (minOverlapX < minOverlapY) {
            // Collision along the x-axis
            if (overlapLeft > overlapRight) {
                this->collidedSide = CollisionSide::Right;
            }
            else {
                this->collidedSide = CollisionSide::Left;
            }
        }
        else {
            // Collision along the y-axis
            if (overlapTop > overlapBottom) {
                this->collidedSide = CollisionSide::Bottom;
            }
            else {
                this->collidedSide = CollisionSide::Top;
            }
        }
    }
    else {
        this->collidedSide = CollisionSide::None;
    }
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

bool Player::hasReachedTarget() const {
    float epsilon = 1.f;
    sf::Vector2f position = getPosition();
    return (std::abs(position.x - targetPosition.x) < epsilon && std::abs(position.y - targetPosition.y) < epsilon);
}

void Player::move(const sf::Vector2f& offset, const sf::RectangleShape& other) {
    sf::Vector2f newPosition = this->rectangle.getPosition() + offset;
    this->rectangle.setPosition(newPosition);

    // check collision
    if (this->intersect(other)) {
        this->setCollidedSide(other);
        this->rectangle.setPosition(this->rectangle.getPosition() - offset);
        this->enemyHit = true;
    }
    else {
        this->updateIndicatorPos();
    }
    this->moving = true;
}

void Player::startSprint(float flag) {
    if (flag) {
        this->sprintClock.restart();
    }
    this->moving = true;
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

void Player::handleEnemyCollision(const Player::CollisionSide collisionSide) {
    switch (collisionSide) {
    case Player::CollisionSide::Top:
        this->setTarget(sf::Vector2f(this->rectangle.getPosition().x, this->rectangle.getPosition().y + 2000));
        this->startSprint(false);
        break;

    case Player::CollisionSide::Bottom:
        this->setTarget(sf::Vector2f(this->rectangle.getPosition().x, this->rectangle.getPosition().y - 2000));
        this->startSprint(false);
        break;

    case Player::CollisionSide::Left:
        this->setTarget(sf::Vector2f(this->rectangle.getPosition().x + 2000, this->rectangle.getPosition().y));
        this->startSprint(false);
        break;

    case Player::CollisionSide::Right:
        this->setTarget(sf::Vector2f(this->rectangle.getPosition().x - 2000, this->rectangle.getPosition().y));
        this->startSprint(false);
        break;
    }
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
