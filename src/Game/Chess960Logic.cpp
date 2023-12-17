//
// Created by MINH NHUT on 12/17/2023.
//

#include "Chess960Logic.hpp"

#include "FenGenerator.hpp"
#include "GameHandler.hpp"

Chess960Logic::Chess960Logic(GameHandler* handler)
    : GameLogic(FenGenerator::getRandom960(), handler) {
	updateStatus();
}

Chess960Logic::Chess960Logic(const Chess960Logic& other, GameHandler* handler)
    : GameLogic(other, handler) {}

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
	} else {
		mStatus = OnGoing;
	}
}

GameOptions::Type Chess960Logic::getType() const {
	return GameOptions::Type::Chess960;
}

bool Chess960Logic::isLegalCastling(int from, int to) const {
	int color = mBoard.getTurn();
	if (mBoard.get(from) != Piece::King || mBoard.get(to) != Piece::Rook)
		return false;
	if (Board::getRank(from) != Board::getRank(to))
		return false;
	if (Board::getRank(from) != (color ? 7 : 0))
			return false;

	bool side = to-from < 0 ? 0 : 1; // queen side = 0, king side = 1
	int kingNewPos = Board::getSquareID(color ? 7 : 0, side ? 6 : 2);

	for(int i = std::min(from, kingNewPos); i <= std::max(from, kingNewPos); i++) {
		if (mAttacks.isAttacked(i))
			return false;
		if (i != from && mBoard.get(i) != 0)
			return false;
	}

	return true;
}
