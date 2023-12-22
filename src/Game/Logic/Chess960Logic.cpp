//
// Created by MINH NHUT on 12/17/2023.
//

#include "Chess960Logic.hpp"
#include "FenGenerator.hpp"
#include "Game/GameHandler.hpp"

#include <iostream>

Chess960Logic::Chess960Logic(GameHandler* handler)
    : GameLogic(FenGenerator::getRandom960(), handler), aRookFile(-1), hRookFile(-1) {
	updateStatus();
	for (int8_t i = 0; i < 8; i++)
		if (mBoard.getType(i) == Piece::Rook) {
			if (aRookFile == -1)
				aRookFile = i;
			else
				hRookFile = i;
		}
	std::cerr << "aRookFile = " << (int)aRookFile << ", hRookFile = " << (int)hRookFile << '\n';
	assert(aRookFile != -1 && hRookFile != -1);
}

Chess960Logic::Chess960Logic(const Chess960Logic& other, GameHandler* handler)
    : GameLogic(other, handler), aRookFile(other.aRookFile), hRookFile(other.hRookFile) {}

Chess960Logic* Chess960Logic::clone() const {
	return new Chess960Logic(*this, nullptr);
}

Chess960Logic::~Chess960Logic() = default;

void Chess960Logic::updateStatus() {
	saveHistory();
	if (!hasLegalMove()) {
		if (mAttacks.isKingInCheck()) {
			mStatus = Checkmate;
			//			std::cout << "Checkmate " << (mBoard.getTurn() ? "White" : "Black") << " wins\n";
		} else {
			mStatus = Stalemate;
			//			std::cout << "Stalemate\n";
		}
	} else if (isInsufficientMaterial()) {
		mStatus = InsufficientMaterial;
		//		std::cout << "Insufficient material\n";
	} else if (isThreefoldRepetition()) {
		mStatus = ThreefoldRepetition;
		//		std::cout << "Threefold repetition\n";
	} else if (isFiftyMoveRule()) {
		mStatus = FiftyMoveRule;
		//		std::cout << "Fifty move rule\n";
	} else {
		mStatus = OnGoing;
	}
}

GameOptions::Type Chess960Logic::getType() const {
	return GameOptions::Type::Chess960;
}

bool Chess960Logic::isLegalCastling(int from, int to) const {
	int castling = mBoard.getCastling();
	if (castling == 0)
		return false;

	int color = mBoard.getTurn();
	if (mBoard.getType(from) != Piece::King || mBoard.getType(to) != Piece::Rook)
		return false;
	if (Board::getRank(from) != Board::getRank(to))
		return false;
	if (Board::getRank(from) != (color ? 7 : 0))
		return false;

	//	std::cout << "Check for castle " << from << ' ' << to << '\n';
	if (mAttacks.isAttacked(from))  // check for check
		return false;
	bool side = to - from < 0 ? 0 : 1;  // queen side = 0, king side = 1

	if (color) {
		if (side && !(castling & 4))
			return false;
		if (!side && !(castling & 8))
			return false;
	} else {
		if (side && !(castling & 1))
			return false;
		if (!side && !(castling & 2))
			return false;
	}

	int kingNewPos = Board::getSquareID(color ? 7 : 0, side ? 6 : 2);
	int rookNewPos = Board::getSquareID(color ? 7 : 0, side ? 5 : 3);
	if (mAttacks.isAttacked(kingNewPos))
		return false;
	if (kingNewPos != from && kingNewPos != to && mBoard.get(kingNewPos) != 0)
		return false;
	if (rookNewPos != from && rookNewPos != to &&
	    mBoard.get(rookNewPos) != 0)  // check for blocking
		return false;
	for (int i = std::min(from, to) + 1; i < std::max(from, to); i++) {
		if (mBoard.get(i) != 0)
			return false;
	}

	for (int i = std::min(from, kingNewPos); i <= std::max(from, kingNewPos);
	     i++) {  // check for attacked
		if (mAttacks.isAttacked(i))
			return false;
	}

	return true;
}

int Chess960Logic::getRook(bool color, bool side) const {
	return Board::getSquareID(color ? 7 : 0, side ? hRookFile : aRookFile);
}

bool Chess960Logic::getRookSide(int square) const {
	return Board::getFile(square) == hRookFile;
}