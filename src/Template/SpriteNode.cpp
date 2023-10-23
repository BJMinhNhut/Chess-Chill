//
// Created by MINH NHUT on 10/23/2023.
//

#include "SpriteNode.hpp"
#include "Template/Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

SpriteNode::SpriteNode(const sf::Texture& texture) : mSprite(texture) {}

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect)
    : mSprite(texture, textureRect) {}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mSprite, states);
}
void SpriteNode::centerOrigin() {
	Utility::centerOrigin(mSprite);
}
