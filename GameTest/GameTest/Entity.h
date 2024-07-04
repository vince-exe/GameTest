#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class Entity : public sf::Drawable {
protected:
	virtual void draw(sf::RenderTarget& window, sf::RenderStates state) const;

protected:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

public:
	Entity() = default;

	Entity(sf::Texture& texture);

	sf::Sprite& getSprite();

	sf::Texture& getTexture();

	virtual void setTexture(sf::Texture& texture);

	virtual bool loadTexture(const std::string& path);

	virtual bool isInside(const sf::Vector2f& pos);
};

