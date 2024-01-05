//
// Created by MINH NHUT on 12/9/2023.
//

#include "Move.hpp"

#include <string>

Move::Move(int from, int to, int promotion) : mMove(from | (to << 6)), mPromote(promotion) {}

Move::Move(std::string move, int promotion) : mPromote(promotion) {
	mMove =
	    (move[0] - 'a') | ((move[1] - '1') << 3) | ((move[2] - 'a') << 6) | ((move[3] - '1') << 9);
}

int Move::from() const {
	return mMove & 63;
}

int Move::to() const {
	return (mMove >> 6) & 63;
}

int Move::promote() const {
	return mPromote;
}