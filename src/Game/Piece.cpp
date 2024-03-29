//
// Created by MINH NHUT on 10/23/2023.
//

#include "Piece.hpp"

#include <cassert>
#include <cmath>
#include <locale>

const int Piece::NAME = 7;
const int Piece::SIZE = 85;

Piece::Piece(const sf::Texture& textures, int piece)
    : SpriteNode(textures, getRectByType(piece)), mType(piece), mVelocity(), mTargetPosition() {
	assert(valid(piece));
}

std::string Piece::getPieceName(int type) {
	switch (type) {
		case Pawn:
			return "";
		case Knight:
			return "N";
		case Bishop:
			return "B";
		case Rook:
			return "R";
		case Queen:
			return "Q";
		case King:
			return "K";
		default:
			assert(false);
	}
}

bool Piece::valid(int piece) {
	return (piece >> 3) < 2 && piece >= 0 && (piece & NAME) <= Pawn;
}

int Piece::getColor(int piece) {
	return (piece >> 3) & 1;
}

int Piece::getType(int piece) {
	return piece & 7;
}

int Piece::getPieceFromChar(char ch) {
	int piece = std::islower(ch) ? Black : White;
	switch (tolower(ch)) {
		case 'p':
			piece |= Pawn;
			break;
		case 'n':
			piece |= Knight;
			break;
		case 'b':
			piece |= Bishop;
			break;
		case 'r':
			piece |= Rook;
			break;
		case 'q':
			piece |= Queen;
			break;
		case 'k':
			piece |= King;
			break;
		default:
			assert(false);
	}
	return piece;
}

char Piece::getCharFromPiece(int piece) {
	char ch = ' ';
	switch (piece & NAME) {
		case Pawn:
			ch = 'p';
			break;
		case Knight:
			ch = 'n';
			break;
		case Bishop:
			ch = 'b';
			break;
		case Rook:
			ch = 'r';
			break;
		case Queen:
			ch = 'q';
			break;
		case King:
			ch = 'k';
			break;
		default:
			assert(false);
	}
	return (piece & White) ? std::toupper(ch) : ch;
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

