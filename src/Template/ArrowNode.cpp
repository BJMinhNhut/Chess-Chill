//
// Created by MINH NHUT on 1/8/2024.
//

#include "ArrowNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>

const float ArrowNode::HEAD_RADIUS = 30.f;
const float ArrowNode::THICKNESS = 20.f;

ArrowNode::ArrowNode(sf::Vector2f line)
	: mLine(getLineShape(line)),
	  mHead(getHeadShape(line)) {
}

void ArrowNode::setOpacity(int rate) {
	sf::Color mColor = mLine.getFillColor();
	mColor.a = 255 * rate / 100;
	mLine.setFillColor(mColor);
	mHead.setFillColor(mColor);
}

void ArrowNode::setFillColor(sf::Color color) {
	mLine.setFillColor(color);
	mHead.setFillColor(color);
}

void ArrowNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(mLine, states);
	target.draw(mHead, states);
}

sf::RectangleShape ArrowNode::getLineShape(sf::Vector2f line) {
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(std::sqrt(line.x * line.x + line.y * line.y) - std::floor(HEAD_RADIUS * 1.5f), THICKNESS));
	shape.setOrigin(0.f, THICKNESS / 2.f);
	shape.setPosition(0.f, 0.f);
	shape.setRotation(std::atan2(line.y, line.x) * 180.f / 3.141592f);
	shape.setFillColor(sf::Color::White);
	return shape;
}

sf::CircleShape ArrowNode::getHeadShape(sf::Vector2f line) {
	sf::CircleShape shape;
	shape.setPointCount(3);
	shape.setRadius(HEAD_RADIUS);
	shape.setOrigin(HEAD_RADIUS, 0.f);
	shape.setPosition(line);
	shape.setRotation(std::atan2(line.y, line.x) * 180.f / 3.141592f + 90.f);
	shape.setFillColor(sf::Color::White);
	return shape;
}