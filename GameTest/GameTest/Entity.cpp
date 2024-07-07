#include "Entity.h"

Entity::Entity(sf::Texture& texture) {
	m_Texture= texture;
	m_Sprite.setTexture(m_Texture);
}

sf::Sprite& Entity::getSprite() {
	return m_Sprite;
}

sf::Texture& Entity::getTexture() {
	return m_Texture;
}

void Entity::setTexture(const sf::Texture& texture) {
	m_Texture = texture;
	m_Sprite.setTexture(texture, true);
}

bool Entity::loadTexture(const std::string& path) {
	if (m_Texture.loadFromFile(path)) {
		m_Sprite.setTexture(m_Texture);
		return true;
	}

	return false;
}

bool Entity::isInside(const sf::Vector2f& pos) {
	return m_Sprite.getGlobalBounds().contains(pos);

	sf::Text cao;

}

void Entity::draw(sf::RenderTarget& window, sf::RenderStates state) const {
	window.draw(m_Sprite, state);
}
