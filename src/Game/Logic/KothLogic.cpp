//
// Created by MINH NHUT on 12/23/2023.
//

#include "KothLogic.hpp"
#include "FenGenerator.hpp"
#include "Game/GameHandler.hpp"

KothLogic::KothLogic(GameHandler* handler) : StandardLogic(handler) {
	updateStatus();
}

KothLogic::KothLogic(const KothLogic& other, GameHandler* handler) : StandardLogic(other, handler) {}

KothLogic* KothLogic::clone() const {
	return new KothLogic(*this, nullptr);
}

KothLogic::~KothLogic() = default;

bool KothLogic::kingReachCenter(bool color) const {
	int king = mBoard.getKing(color);
	bool middleFile = Board::getFile(king) == 3 || Board::getFile(king) == 4;
	bool middleRank = Board::getRank(king) == 3 || Board::getRank(king) == 4;
	return middleFile && middleRank;
}

void KothLogic::updateStatus() {
	saveHistory();
	if (kingReachCenter(!mBoard.getTurn())) {
		mStatus = TopOfTheHill;
		//		std::cout << "King reach center\n";
	} else if (!hasLegalMove()) {
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

GameOptions::Type KothLogic::getType() const {
	return GameOptions::Type::KingOfTheHill;
}