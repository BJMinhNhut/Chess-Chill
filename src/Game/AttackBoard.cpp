//
// Created by MINH NHUT on 11/19/2023.
//

#include "AttackBoard.hpp"
#include "Piece.hpp"
#include "AttackGenerator.hpp"

#include <cassert>

AttackBoard::AttackBoard(Board* board) : mBoard(board) {
	update();
}

bool AttackBoard::isAttacked(int square) const {
	return isAttacked(square, !mBoard->getTurn());
}

bool AttackBoard::isAttacked(int square, bool turn) const {
	return mAttackBoard[turn] & (1LL << square);
}

bool AttackBoard::isKingInCheck() const {
	return isKingInCheck(mBoard->getTurn());
}

bool AttackBoard::isKingInCheck(bool turn) const {
	return isAttacked(mBoard->getKing(turn), !turn);
}

void AttackBoard::update() {
	update(0);
	update(1);
}

void AttackBoard::update(bool turn) {
	mAttackBoard[turn] = 0;
	for (int i = 0; i < 64; i++) {
		int piece = mBoard->get(i);
		if (piece == 0 || Piece::getColor(piece) != turn)
			continue;
		switch (Piece::getType(piece)) {
			case Piece::Pawn:
				mAttackBoard[turn] |= AttackGenerator::generatePawnAttacks(i, turn);
				break;
			case Piece::Knight:
				mAttackBoard[turn] |= AttackGenerator::generateKnightAttacks(i);
				break;
			case Piece::Bishop:
				mAttackBoard[turn] |= AttackGenerator::generateBishopAttacks(i, *mBoard);
				break;
			case Piece::Rook:
				mAttackBoard[turn] |= AttackGenerator::generateRookAttacks(i, *mBoard);
				break;
			case Piece::Queen:
				mAttackBoard[turn] |= AttackGenerator::generateQueenAttacks(i, *mBoard);
				break;
			case Piece::King:
				mAttackBoard[turn] |= AttackGenerator::generateKingAttacks(i);
				break;
			default:
				assert(false);
		}
	}
}