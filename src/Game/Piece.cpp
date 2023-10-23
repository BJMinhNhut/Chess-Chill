//
// Created by MINH NHUT on 10/23/2023.
//

#include "Piece.hpp"

#include <cassert>

const int Piece::NAME = 7;
const int Piece::SIZE = 85;

Piece::Piece(const sf::Texture& textures, int type)
    : SpriteNode(textures, getRectByType(type)), mType(type) {
	assert(type <= (Pawn|Black) && type >= 0 && (type & NAME) <= Pawn);
}

sf::IntRect Piece::getRectByType(int type) {
	int x = (type & 7) * SIZE;
	int y = ((type >> 3) & 1) * SIZE;
	return {x, y, SIZE, SIZE};
}

bool Piece::color() const {
	return (mType >> 3) & 1;
}
