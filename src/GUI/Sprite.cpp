//
// Created by MINH NHUT on 7/4/2023.
//

#include "Sprite.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace GUI {

Sprite::Sprite(const sf::Texture& texture) {
	setTexture(texture);
}

bool Sprite::isSelectable() const {
	return false;
}

void Sprite::setTexture(const sf::Texture& texture) {
	mSprite.setTexture(texture, true);
}

void Sprite::crop(int width, int height) {
	mSprite.setTextureRect(sf::IntRect(0, 0, width, height));
}

void Sprite::setSize(float x, float y) {
	mSprite.setScale(x / mSprite.getLocalBounds().width, y / mSprite.getLocalBounds().height);
}

void Sprite::centerOrigin() {
	Utility::centerOrigin(mSprite);
}

void Sprite::handleEvent(const sf::Event& event) {}

void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

}  // namespace GUI