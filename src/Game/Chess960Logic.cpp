//
// Created by MINH NHUT on 12/17/2023.
//

#include "Chess960Logic.hpp"

#include "FenGenerator.hpp"
#include "GameHandler.hpp"

Chess960Logic::Chess960Logic(GameHandler* handler)
    : GameLogic(FenGenerator::getRandom960(), handler), aRook(-1), hRook(-1) {
	updateStatus();
	for (int8_t col = 0; col < 8; ++col) {
		if (mBoard.get(col) == Piece::Rook) {
			if (aRook == -1) {
				aRook = col;
			} else {
				hRook = col;
			}
		}
	}
}

Chess960Logic::Chess960Logic(const Chess960Logic& other, GameHandler* handler)
    : GameLogic(other, handler), aRook(other.aRook), hRook(other.hRook) {}

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
	return false;
}
