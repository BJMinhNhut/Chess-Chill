//
// Created by MINH NHUT on 10/23/2023.
//

#include "Piece.hpp"

#include <cassert>
#include <cmath>

const int Piece::NAME = 7;
const int Piece::SIZE = 85;

Piece::Piece(const sf::Texture& textures, int type)
    : SpriteNode(textures, getRectByType(type)), mType(type) {
	assert(type <= (Pawn|Black) && type >= 0 && (type & NAME) <= Pawn);
}

void Piece::snap(int x, int y) {
    setPosition(std::floor((float)x - (float)SIZE/2), std::floor((float)y - (float)SIZE/2));
}

sf::IntRect Piece::getRectByType(int type) {
	int x = (type & 7) * SIZE;
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
