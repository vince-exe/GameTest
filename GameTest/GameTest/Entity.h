#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

class Entity : public sf::Drawable {
public:
	Entity() = default;

	Entity(sf::Texture& texture);

	sf::Sprite& getSprite();

	sf::Texture& getTexture();

	virtual bool loadTexture(const std::string& path);

	virtual bool isInside(const sf::Vector2f& pos);

private:
	virtual void draw(sf::RenderTarget& window, sf::RenderStates state) const;

private:
	sf::Texture texture;
	sf::Sprite sprite;
};

