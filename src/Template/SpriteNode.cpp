//
// Created by MINH NHUT on 10/23/2023.
//

#include "SpriteNode.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

SpriteNode::SpriteNode(const sf::Texture& texture) : mSprite(texture) {}

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect)
    : mSprite(texture, textureRect) {}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}

void SpriteNode::setTexture(const sf::Texture& texture) {
	mSprite.setTexture(texture, true);
}

void SpriteNode::centerOrigin() {
	Utility::centerOrigin(mSprite);
}

void SpriteNode::setOpacity(int rate) {
	sf::Color color = mSprite.getColor();
	color.a = 255*rate/100;
	mSprite.setColor(color);
}

bool SpriteNode::contains(int x, int y) const {
	sf::IntRect bound(getPosition().x, getPosition().y, mSprite.getTextureRect().width,
	                  mSprite.getTextureRect().height);
	return bound.contains(x, y);
}