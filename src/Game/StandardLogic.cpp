//
// Created by MINH NHUT on 12/17/2023.
//

#include "StandardLogic.hpp"
#include "GameHandler.hpp"
#include "FenGenerator.hpp"

StandardLogic::StandardLogic(GameHandler* handler) : GameLogic(FenGenerator::START_FEN, handler) {
	updateStatus();
}

StandardLogic::StandardLogic(const StandardLogic& other, GameHandler* handler) : GameLogic(other, handler) {
}

StandardLogic* StandardLogic::clone() const {
	return new StandardLogic(*this, nullptr);
}

StandardLogic::~StandardLogic() = default;

void StandardLogic::updateStatus() {
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

bool StandardLogic::isLegalCastling(int from, int to) const {
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffFile == 2 && absDiffRank == 0) {
		bool color = mBoard.getTurn();
		int dir = diffFile < 0 ? -1 : 1;
		if (Board::getFile(from) != 4)
			return false;
		if (Board::getRank(from) != (color ? 7 : 0))
			return false;
		if (mAttacks.isAttacked(from) || mAttacks.isAttacked(from + dir) ||
		    mAttacks.isAttacked(from + 2 * dir))
			return false;
		if (mBoard.get(from + dir) != 0 || mBoard.get(from + 2 * dir) != 0)  // check for blocking
			return false;
		if (to == (color ? 58 : 2)) {  // Queen side castling
			if (mBoard.get(from + 3 * dir) != 0)
				return false;
		}

		if (dir == 1)
			return mBoard.getCastling() & (color ? 4 : 1);
		else
			return mBoard.getCastling() & (color ? 8 : 2);
	}
	return false;
}

GameOptions::Type StandardLogic::getType() const {
	return GameOptions::Type::Chess960;
}