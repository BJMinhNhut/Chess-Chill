//
// Created by MINH NHUT on 10/24/2023.
//

#include "RectNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

RectNode::RectNode(int width, int height, const sf::Color& color)
    : mRect(sf::Vector2f((float)width, (float)height)) {
	mRect.setFillColor(color);
}

void RectNode::setOpacity(int rate) {
	sf::Color color = mRect.getFillColor();
	color.a = 255 * rate / 100;
	mRect.setFillColor(color);
}

void RectNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mRect, states);
}
