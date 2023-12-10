//
// Created by MINH NHUT on 12/9/2023.
//

#include "Move.hpp"

Move::Move(int from, int to, int promotion)
	: mMove(from | (to << 6)), mPromote(promotion) {}

int Move::from() const { return mMove & 63; }

int Move::to() const { return (mMove >> 6) & 63; }

int Move::promote() const { return mPromote; }