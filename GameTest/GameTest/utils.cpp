#include "utils.h"

bool SkyfallUtils::doLinesIntersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4) {
    auto cross = [](sf::Vector2f a, sf::Vector2f b) -> float {
        return a.x * b.y - a.y * b.x;
        };

    sf::Vector2f r = p2 - p1;
    sf::Vector2f s = p4 - p3;
    float denominator = cross(r, s);

    if (denominator == 0) {
        return false; // Le linee sono parallele
    }

    sf::Vector2f qp = p3 - p1;
    float t = cross(qp, s) / denominator;
    float u = cross(qp, r) / denominator;

    return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
}

bool SkyfallUtils::doesRectangleIntersectLine(const sf::RectangleShape& rectangle, const sf::VertexArray& line) {
    if (line.getVertexCount() != 2) {
        return false;
    }

    sf::Vector2f p1 = line[0].position;
    sf::Vector2f p2 = line[1].position;

    sf::Vector2f corners[4];
    for (int i = 0; i < 4; ++i) {
        corners[i] = rectangle.getTransform().transformPoint(rectangle.getPoint(i));
    }

    for (int i = 0; i < 4; ++i) {
        if (SkyfallUtils::doLinesIntersect(p1, p2, corners[i], corners[(i + 1) % 4])) {
            return true;
        }
    }

    return false;
}
