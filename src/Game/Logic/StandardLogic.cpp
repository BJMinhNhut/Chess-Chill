//
// Created by MINH NHUT on 12/17/2023.
//

#include "StandardLogic.hpp"
#include "FenGenerator.hpp"
#include "Game/GameHandler.hpp"

StandardLogic::StandardLogic(GameHandler* handler) : GameLogic(FenGenerator::START_FEN, handler) {
	updateStatus();
}

StandardLogic::StandardLogic(const std::string& fen, GameHandler* handler)
    : GameLogic(fen, handler) {
	updateStatus();
}

StandardLogic::StandardLogic(const StandardLogic& other, GameHandler* handler)
    : GameLogic(other, handler) {}

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
	} else if (isFiftyMoveRule()) {
		mStatus = FiftyMoveRule;
		//		std::cout << "Fifty move rule\n";
	} else {
		mStatus = OnGoing;
	}
}

bool StandardLogic::isLegalCastling(int from, int to) const {
	if (mBoard.getType(from) != Piece::King)
		return false;
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;

	int rookPos = getRook(mBoard.getTurn(), diffFile < 0 ? 0 : 1);
	if (mBoard.getType(rookPos) != Piece::Rook)
		return false;
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

int StandardLogic::getRook(bool color, bool side) const {
	// queen side = 0, king side = 1
	// color black = 1, white = 0
	return Board::getSquareID(color ? 7 : 0, side ? 7 : 0);
}

bool StandardLogic::getRookSide(int square) const {
	return Board::getFile(square) == 7;
}

GameOptions::Type StandardLogic::getType() const {
	return GameOptions::Type::Standard;
}