#include "Entity.h"

Entity::Entity(sf::Texture& texture) {
	this->texture = texture;
	this->sprite.setTexture(this->texture);
}

sf::Sprite& Entity::getSprite() {
	return this->sprite;
}

sf::Texture& Entity::getTexture() {
	return this->texture;
}

void Entity::setTexture(sf::Texture& texture) {
	this->texture = texture;
	this->sprite.setTexture(texture, true);
}

bool Entity::loadTexture(const std::string& path) {
	if (this->texture.loadFromFile(path)) {
		this->sprite.setTexture(this->texture);
		return true;
	}

	return false;
}

bool Entity::isInside(const sf::Vector2f& pos) {
	return this->sprite.getGlobalBounds().contains(pos);

	sf::Text cao;

}

void Entity::draw(sf::RenderTarget& window, sf::RenderStates state) const {
	window.draw(this->sprite, state);
}
