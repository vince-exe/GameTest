#include "Player.h"

Player::Player(sf::Vector2f rectSize, sf::Color rectColor, sf::Color indicatorColor, float distanceAbove, float speed, float sprintPower, float sprintTimeout) {
    m_Rectangle.setSize(rectSize);

    m_Rectangle.setFillColor(rectColor);

    m_Indicator.setPointCount(3);
    m_Indicator.setFillColor(indicatorColor);

    m_distanceAbove = distanceAbove;
    m_Speed = speed;

    m_Indicator.setPoint(0, sf::Vector2f(-m_indicatorBaseHalf, 0.f));
    m_Indicator.setPoint(1, sf::Vector2f(m_indicatorBaseHalf, 0.f));
    m_Indicator.setPoint(2, sf::Vector2f(0.f, m_indicatorHeight));

    m_sprintTimeout = sprintTimeout;
    m_sprintPower = sprintPower;
    m_isSprinting = false;
    m_targetReached = true;
    m_Moving = false;
}

Player::CollisionSide& Player::getCollidedSide() {
    return m_collidedSide;
}

void Player::setCollidedSide(const sf::RectangleShape& other) {
    sf::FloatRect intersection;
    if (m_Rectangle.getGlobalBounds().intersects(other.getGlobalBounds(), intersection)) {
        // Calculate overlap in both directions
        float overlapLeft = m_Rectangle.getPosition().x + m_Rectangle.getSize().x - other.getPosition().x;
        float overlapRight = other.getPosition().x + other.getSize().x - m_Rectangle.getPosition().x;
        float overlapTop = m_Rectangle.getPosition().y + m_Rectangle.getSize().y - other.getPosition().y;
        float overlapBottom = other.getPosition().y + other.getSize().y - m_Rectangle.getPosition().y;

        // Determine the minimum overlap direction
        float minOverlapX = std::min(overlapLeft, overlapRight);
        float minOverlapY = std::min(overlapTop, overlapBottom);

        if (minOverlapX < minOverlapY) {
            // Collision along the x-axis
            if (overlapLeft > overlapRight) {
                m_collidedSide = CollisionSide::Right;
            }
            else {
                m_collidedSide = CollisionSide::Left;
            }
        }
        else {
            // Collision along the y-axis
            if (overlapTop > overlapBottom) {
                m_collidedSide = CollisionSide::Bottom;
            }
            else {
                m_collidedSide = CollisionSide::Top;
            }
        }
    }
    else {
        m_collidedSide = CollisionSide::None;
    }
}

void Player::update(sf::Time deltaTime, const sf::RectangleShape& other) {
    if (!m_targetReached) {
        sf::Vector2f direction = m_targetPosition - getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length > 0) {
            direction /= length;

            sf::Vector2f movement = direction * m_Speed * deltaTime.asSeconds();

            if (length < m_Speed * deltaTime.asSeconds()) {
                setPosition(m_targetPosition);
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
    return (std::abs(position.x - m_targetPosition.x) < epsilon && std::abs(position.y - m_targetPosition.y) < epsilon);
}

void Player::move(const sf::Vector2f& offset, const sf::RectangleShape& other) {
    sf::Vector2f newPosition = m_Rectangle.getPosition() + offset;
    m_Rectangle.setPosition(newPosition);

    // check collision
    if (intersect(other)) {
        setCollidedSide(other);
        m_Rectangle.setPosition(m_Rectangle.getPosition() - offset);
        m_enemyHit = true;
    }
    else {
        updateIndicatorPos();
    }
    m_Moving = true;
}

void Player::startSprint(float flag) {
    if (flag) {
        m_sprintClock.restart();
    }
    m_Moving = true;
    m_isSprinting = true;
    m_Speed += m_sprintPower;
}

void Player::stopSprint() {
    m_isSprinting = false;
    m_Speed -= m_sprintPower;
}

bool Player::canSprint() {
    return (m_sprintClock.getElapsedTime().asSeconds() >= m_sprintTimeout);
}

void Player::handleEnemyCollision(const Player::CollisionSide collisionSide) {
    switch (collisionSide) {
    case Player::CollisionSide::Top:
        setTarget(sf::Vector2f(m_Rectangle.getPosition().x, m_Rectangle.getPosition().y + 2000));
        startSprint(false);
        break;

    case Player::CollisionSide::Bottom:
        setTarget(sf::Vector2f(m_Rectangle.getPosition().x, m_Rectangle.getPosition().y - 2000));
        startSprint(false);
        break;

    case Player::CollisionSide::Left:
        setTarget(sf::Vector2f(m_Rectangle.getPosition().x + 2000, m_Rectangle.getPosition().y));
        startSprint(false);
        break;

    case Player::CollisionSide::Right:
        setTarget(sf::Vector2f(m_Rectangle.getPosition().x - 2000, m_Rectangle.getPosition().y));
        startSprint(false);
        break;
    }
}

bool Player::isSprinting() {
    return m_isSprinting;
}

float Player::getVelocity() {
    return m_Speed;
}

void Player::updateIndicatorPos() {
    m_Indicator.setPosition(m_Rectangle.getPosition().x + m_Rectangle.getSize().x / 2, m_Rectangle.getPosition().y - m_distanceAbove - m_indicatorHeight);
}

void Player::setTarget(const sf::Vector2f& target) {
    m_targetPosition = target;
    m_targetReached = false;
    m_Moving = true;
}

bool Player::isMoving() const {
    return m_Moving;
}

void Player::stopMove() {
    m_targetReached = true;
    m_Moving = false;

    if (m_isSprinting) {
        stopSprint();
    }
}

sf::RectangleShape& Player::getRect() {
    return m_Rectangle;
}

void Player::setPosition(sf::Vector2f pos) {
    m_Rectangle.setPosition(pos);
    updateIndicatorPos();
}

void Player::setPosition(float x, float y) {
    m_Rectangle.setPosition(x, y);
    updateIndicatorPos();
}

sf::Vector2f Player::getPosition() const {
    return m_Rectangle.getPosition();
}

bool Player::intersect(const  sf::RectangleShape& rect) {
    return m_Rectangle.getGlobalBounds().intersects(rect.getGlobalBounds());
}

void Player::resetSprint() {
    m_sprintClock.restart();
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_Rectangle, states);
    target.draw(m_Indicator, states);
}

sf::FloatRect Player::getGlobalBounds() const {
    return m_Rectangle.getGlobalBounds();
}

sf::Clock Player::getClock() {
    return m_sprintClock;
}

float Player::getSprintTimeout() {
    return m_sprintTimeout;
}

bool Player::isEnemyHit() {
    return m_enemyHit;
}

void Player::resetEnemyHit() {
    m_enemyHit = false;
}