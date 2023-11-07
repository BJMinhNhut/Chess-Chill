//
// Created by MINH NHUT on 10/23/2023.
//

#include "Piece.hpp"

#include <cassert>
#include <cmath>

const int Piece::NAME = 7;
const int Piece::SIZE = 85;

Piece::Piece(const sf::Texture& textures, int piece)
    : SpriteNode(textures, getRectByType(piece)), mType(piece), mVelocity(), mTargetPosition() {
	assert((piece >> 3) < 2 && piece >= 0 && (piece & NAME) <= Pawn);
}

void Piece::updateCurrent(sf::Time dt) {
	// move piece to target position with static velocity (40 frames)
	sf::Vector2f distance = mTargetPosition - getPosition();
	if (std::abs(distance.x) < 2.f && std::abs(distance.y) < 2.f) {
		SpriteNode::setPosition(mTargetPosition);
		mVelocity = sf::Vector2f(0.f, 0.f);
	} else move(mVelocity);
}

void Piece::setPosition(sf::Vector2f position, bool smooth) {
	mTargetPosition = position;
	if (smooth) {
		mVelocity = (mTargetPosition - getPosition()) / 12.f;
	} else {
		SpriteNode::setPosition(position);
	}
}

void Piece::setPosition(float x, float y, bool smooth) {
	setPosition(sf::Vector2f(x, y), smooth);
}

void Piece::snap(int x, int y) {
	setPosition(std::floor((float)x - (float)SIZE / 2), std::floor((float)y - (float)SIZE / 2),
	            false);
}

void Piece::updateTargetPosition() {
	mTargetPosition = getPosition();
}

sf::IntRect Piece::getRectByType(int type) {
	int x = ((type & 7) - 1) * SIZE;
	int y = ((type >> 3) & 1) * SIZE;
	return {x, y, SIZE, SIZE};
}

Piece* Piece::clone() const {
	auto* clonePiece = new Piece(*mSprite.getTexture(), mType);
	return clonePiece;
}

bool Piece::color() const {
	return (mType >> 3) & 1;
}
