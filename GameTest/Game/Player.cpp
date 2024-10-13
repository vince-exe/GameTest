#include "Player.h"

Player::Player() {
    m_Indicator.setPointCount(3);

    m_Indicator.setPoint(0, sf::Vector2f(-m_indicatorBaseHalf, 0.f));
    m_Indicator.setPoint(1, sf::Vector2f(m_indicatorBaseHalf, 0.f));
    m_Indicator.setPoint(2, sf::Vector2f(0.f, m_indicatorHeight));

    m_isSprinting = false;
    m_targetReached = true;
    m_Moving = false;
    m_vertexLine = std::make_shared<sf::VertexArray>(sf::Lines, 2);
    m_drawVertexLine = false;
}

void Player::setSize(sf::Vector2f vec) {
    m_Rectangle.setSize(vec);
}

void Player::setColor(sf::Color color) {
    m_Rectangle.setFillColor(color);
}

void Player::setIndicator(sf::Color color, float distance) {
    m_Indicator.setFillColor(color);
    m_distanceAbove = distance;
}

void Player::setSpeed(float speed) {
    m_Speed = speed;
}

void Player::setSprint(float power, float timeout) {
    m_sprintPower = power;
    m_sprintTimeout = timeout;
}

Player::CollisionSide& Player::getCollidedSide() {
    return m_collidedSide;
}

void Player::calcPlayerTrend(const sf::Vector2f& newPos) {
    sf::Vector2f oldPos = m_Rectangle.getPosition();

    if (newPos.y > oldPos.y) {
        if (newPos.y > oldPos.y && newPos.y < (oldPos.y + 70.f)) {
            // RIGHT
            if (newPos.x > oldPos.x) {
                (*m_vertexLine)[0].position = newPos;
                (*m_vertexLine)[1].position = sf::Vector2f(newPos.x, newPos.y + m_Rectangle.getSize().y);
                (*m_vertexLine)[0].color = sf::Color::Green;
                (*m_vertexLine)[1].color = sf::Color::Green;
                return;
            }
            // LEFT
            else {
                (*m_vertexLine)[0].position = newPos;
                (*m_vertexLine)[1].position = sf::Vector2f(newPos.x, newPos.y + m_Rectangle.getSize().y);

                (*m_vertexLine)[0].color = sf::Color::Green;
                (*m_vertexLine)[1].color = sf::Color::Green;
                return;
            }
        }
        // BOTTOM 
        if (newPos.x >= oldPos.x && newPos.x <= (oldPos.x + 70.f)) {
            (*m_vertexLine)[0].position = newPos;
            (*m_vertexLine)[1].position = sf::Vector2f(newPos.x + m_Rectangle.getSize().x, newPos.y);

            (*m_vertexLine)[0].color = sf::Color::Green;
            (*m_vertexLine)[1].color = sf::Color::Green;
            return;
        }
        // BOTTOM RIGHT
        else if (newPos.x > (oldPos.x + 70)) {
            (*m_vertexLine)[0].position = newPos;
            (*m_vertexLine)[1].position = sf::Vector2f(newPos.x + 100.f, newPos.y);

            (*m_vertexLine)[0].color = sf::Color::Green;
            (*m_vertexLine)[1].color = sf::Color::Green;
            return;
        }
        // BOTTOM LEFT
        else {
            (*m_vertexLine)[0].position = newPos;
            (*m_vertexLine)[1].position = sf::Vector2f(newPos.x, newPos.y + 100.f);

            (*m_vertexLine)[0].color = sf::Color::Green;
            (*m_vertexLine)[1].color = sf::Color::Green;
            return;
        }
    }
    // TOP 
    if (newPos.y < oldPos.y) {
        if (newPos.x >= oldPos.x && newPos.x <= (oldPos.x + 70.f)) {
            (*m_vertexLine)[0].position = newPos;
            (*m_vertexLine)[1].position = sf::Vector2f(newPos.x + m_Rectangle.getSize().x, newPos.y);

            (*m_vertexLine)[0].color = sf::Color::Green;
            (*m_vertexLine)[1].color = sf::Color::Green;
            return;
        }
        // TOP RIGHT
        else if (newPos.x > (oldPos.x + 70)) {
            (*m_vertexLine)[0].position = newPos;
            (*m_vertexLine)[1].position = sf::Vector2f(newPos.x + 100.f, newPos.y);
  
            (*m_vertexLine)[0].color = sf::Color::Green;
            (*m_vertexLine)[1].color = sf::Color::Green;
            return;
        }
        // TOP LEFT
        else {
            (*m_vertexLine)[0].position = newPos;
            (*m_vertexLine)[1].position = sf::Vector2f(newPos.x - 100.f, newPos.y);

            (*m_vertexLine)[0].color = sf::Color::Green;
            (*m_vertexLine)[1].color = sf::Color::Green;
            return;
        }
    }
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
                if (!m_hitByEnemy && SkyfallUtils::doesRectangleIntersectLine(m_Rectangle, *m_vertexLine)) {
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
    m_hitByEnemy = false;

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
    if (m_drawVertexLine) {
        target.draw(*m_vertexLine);
    }

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

void Player::setHitByEnemy(bool flag) {
    m_hitByEnemy = flag;
}

bool Player::hitByEnemy() {
    return m_hitByEnemy;
}

void Player::setDebugMode(bool flag) {
    m_drawVertexLine = flag;
}

const bool Player::getDebugMode() const {
    return m_drawVertexLine;
}