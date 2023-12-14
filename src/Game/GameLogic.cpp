//
// Created by MINH NHUT on 10/25/2023.
//

#include "GameLogic.hpp"
#include "GameHandler.hpp"
#include "Evaluator.hpp"
#include "Piece.hpp"
#include "MoveTable.hpp"

#include <cassert>
#include <iostream>
#include <cstring>

const int GameLogic::BOARD_SIZE = 64;

GameLogic::GameLogic(const std::string& fen, GameHandler* handler)
    : mBoard(fen),
      mAttacks(mBoard),
      mHandler(handler),
      mStatus(OnGoing),
      mLastMove(Normal),
      mFENs(),
      mClock(),
      mLastMovePiece(-1) {
	updateStatus();
}

GameLogic::GameLogic(const GameLogic& other, GameHandler* handler)
    : mBoard(other.mBoard),
      mAttacks(mBoard),
      mHandler(handler),
      mStatus(other.mStatus),
      mLastMove(other.mLastMove),
      mClock(other.mClock),
      mFENs(other.mFENs),
      mLastMovePiece(other.mLastMovePiece) {
}

void GameLogic::updateTime(sf::Time dt) {
	if (mStatus != OnGoing || mBoard.getFullMove() < 2)
		return;
	mClock.update(mBoard.getTurn(), dt);
	if (mClock.isTimeOut(mBoard.getTurn()))
		mStatus = Timeout;
}

float GameLogic::getRemainingTime(bool turn) const {
	return mClock.get(turn);
}

std::vector<Move> GameLogic::getLegalMoves() const {
	std::vector<Move> moveList;
	MoveTable *table = MoveTable::getInstance();
	for (int i = 0; i < 64; i++) {
		if (mBoard.get(i) == 0 || Piece::getColor(mBoard.get(i)) != mBoard.getTurn())
			continue;
		int64_t moves = table->getMoves(mBoard.get(i), i);
		for (int j; moves; moves &= ~(1LL << j)) {
			j = __builtin_ctzll(moves);
			if (isLegalPromotion(i, j) && isLegalMove(Move(i, j, Piece::Queen))) {
				int color = mBoard.getTurn() ? Piece::Black : Piece::White;
				moveList.emplace_back(i, j, Piece::Queen | color);
				moveList.emplace_back(i, j, Piece::Rook | color);
				moveList.emplace_back(i, j, Piece::Bishop | color);
				moveList.emplace_back(i, j, Piece::Knight | color);
			} else if (isLegalMove(Move(i, j))) {
				moveList.emplace_back(i, j, Piece::None);
			}
		}
	}
	return moveList;
}

std::vector<Move> GameLogic::getMoveList(int from) const {
	std::vector<Move> moveList;
	int piece = mBoard.get(from);
	if (piece == 0 || Piece::getColor(piece) != mBoard.getTurn())
		return moveList;
	MoveTable *table = MoveTable::getInstance();
	int64_t moves = table->getMoves(piece, from);
	for (int j; moves; moves &= ~(1LL << j)) {
		j = __builtin_ctzll(moves);
		if (isLegalPromotion(from, j) && isLegalMove(Move(from, j, Piece::Queen))) {
			int color = mBoard.getTurn() ? Piece::Black : Piece::White;
			moveList.emplace_back(from, j, Piece::Queen | color);
			moveList.emplace_back(from, j, Piece::Rook | color);
			moveList.emplace_back(from, j, Piece::Bishop | color);
			moveList.emplace_back(from, j, Piece::Knight | color);
		} else if (isLegalMove(Move(from, j))) {
			moveList.emplace_back(from, j, Piece::None);
		}
	}
	return moveList;
}

int GameLogic::getLastMovePiece() const {
	return mLastMovePiece;
}

int GameLogic::getKing(int color) const {
	return mBoard.getKing(color);
}

int GameLogic::getEvaluation() const {
	return Evaluator::evaluateBoard(*this);
}

int GameLogic::getCastling() const {
	return mBoard.getCastling();
}

bool GameLogic::isChecked() const {
	return mLastMove & Check;
}

bool GameLogic::isCaptured() const {
	return mLastMove & Capture;
}

bool GameLogic::isCastled() const {
	return mLastMove & Castling;
}

bool GameLogic::isPromoted() const {
	return mLastMove & Promotion;
}

bool GameLogic::isLegalMove(Move move) const {
	int from = move.from(), to = move.to();
	if (from == to || !Board::validSquare(from) || !Board::validSquare(to))
		return false;
	int piece = mBoard.get(from);
	int capture = mBoard.get(to);

	//	std::cout << piece << ' ' << mBoard.getTurn() << '\n';
	if (piece == 0 || Piece::getColor(piece) != mBoard.getTurn())
		return false;
	if (capture != 0 &&
	    (Piece::getColor(capture) == mBoard.getTurn() || Piece::getType(capture) == Piece::King))
		return false;
	bool legal = true;
	switch (piece & 7) {
		case Piece::Pawn:
			legal = isLegalPawnMove(from, to);
			break;
		case Piece::Knight:
			legal = isLegalKnightMove(from, to);
			break;
		case Piece::Bishop:
			legal = isLegalBishopMove(from, to);
			break;
		case Piece::Rook:
			legal = isLegalRookMove(from, to);
			break;
		case Piece::Queen:
			legal = isLegalQueenMove(from, to);
			break;
		case Piece::King:
			legal = isLegalKingMove(from, to);
			break;
		default:
			assert(false);
	};

	if (legal)
		return !isPseudoLegalMove(move);
	else
		return false;
}

bool GameLogic::isPseudoLegalMove(Move move) const {
	GameLogic copy(*this, nullptr);
	copy.pureMove(move);
	//	if (copy.mAttacks.isKingInCheck(mBoard.getTurn())) {
	//		std::cout << from << ' ' << to << " pseudo legal pureMove\n";
	//	}
	return copy.mAttacks.isKingInCheck(mBoard.getTurn());
}

bool GameLogic::isAttacked(int square) const {
	return mAttacks.isAttacked(square);
}

bool GameLogic::isFinished() const {
	return mStatus != OnGoing;
}

void GameLogic::makeMove(Move move) {
	mClock.increment(mBoard.getTurn());
	pureMove(move);
	mLastMovePiece = move.to();
	updateStatus();
}

void GameLogic::setClock(sf::Time time, sf::Time bonus) {
	mClock.set(time, bonus);
}

GameLogic::Status GameLogic::status() const {
	return mStatus;
}

std::string GameLogic::getWinner() const {
	assert(mStatus != OnGoing && mStatus != Stalemate);
	return mBoard.getTurn() ? "White" : "Black";
}

void GameLogic::pureMove(Move move) {
	assert(mStatus == OnGoing);
	mLastMove = Normal;

	int from = move.from(), to = move.to();

	if (isEnPassant(from, to)) {
		capturePiece(mBoard.getEnPassant() + (mBoard.getTurn() ? 8 : -8));
		mLastMove |= Capture;
	}
	mBoard.updateEnPassant(from, to);

	if (mBoard.get(to) != 0) {
		capturePiece(to);
		mLastMove |= Capture;
	}

	if (isLegalCastling(from, to)) {
		int dir = to - from < 0 ? -1 : 1;
		int rook = dir < 0 ? from - 4 : from + 3;
		movePiece(rook, from + dir);
		mLastMove |= Castling;
	}

	mBoard.updateCastling(from);
	mBoard.updateHalfMove(from, to);
	movePiece(from, to);
	// check for promotion
	if (mBoard.getType(to) == Piece::Pawn && (Board::getRank(to) == 0 || Board::getRank(to) == 7)) {
		mLastMove |= Promotion;
		promotePiece(to, move.promote());
	}

	postMove();
}

void GameLogic::capturePiece(int square) {
	//	std::cout << "capture " << square << '\n';
	assert(mBoard.getType(square) != Piece::King);
	mBoard.set(square, 0);
	if (mHandler) mHandler->capturePiece(square);
}

void GameLogic::movePiece(int from, int to) {
	mBoard.move(from, to);
	if (mHandler) mHandler->movePiece(from, to);
}

void GameLogic::postMove() {
	if (mBoard.getTurn())
		mBoard.nextFullMove();
	mAttacks.update();
	mBoard.nextTurn();
	if (mAttacks.isKingInCheck())
		mLastMove |= Check;
	if (mHandler) mHandler->postMove();
}

void GameLogic::promotePiece(int square, int piece) {
	assert(mBoard.getType(square) == Piece::Pawn);
	mBoard.set(square, 0);
	mBoard.set(square, piece);
	if (mHandler) mHandler->promotePiece(square, piece);
}

void GameLogic::updateStatus() {
	mFENs[mBoard.getFEN(false)]++;
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

bool GameLogic::isThreefoldRepetition() {
	// check for threefold repetition
	//	std::cout << "current FEN \"" << mHistory.back() << "\" " << count << '\n';
	return mFENs[mBoard.getFEN(false)] >= 3;
}

bool GameLogic::isInsufficientMaterial() {
	// check for insufficient material
	int whiteLight = 0, blackLight = 0;
	for (int i = 0; i < 64; i++) {
		int piece = mBoard.get(i);
		if (piece == 0)
			continue;
		switch (Piece::getType(piece)) {
			case Piece::Knight:
			case Piece::Bishop:
				if (Piece::getColor(piece))
					blackLight++;
				else
					whiteLight++;
				break;
			case Piece::King:
				break;
			default:
				return false;
		}
	}
	return (whiteLight <= 1 && blackLight <= 1);
}

bool GameLogic::hasLegalMove() const {
	for (int i = 0; i < 64; i++) {
		int piece = mBoard.get(i);
		if (piece == 0 || Piece::getColor(piece) != mBoard.getTurn())
			continue;

		for (int j = 0; j < 64; j++) {
			Move move(i, j, isLegalPromotion(i, j) ? Piece::Queen : Piece::None);
			if (isLegalMove(move))
				return true;
		}
	}
	return false;
}

bool GameLogic::isEnPassant(int from, int to) const {
	int piece = mBoard.get(from);
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = Piece::getColor(piece) ? -8 : 8;
	int targetRow = to >> 3;
	int startRow = from >> 3;
	if (mBoard.getEnPassant() != to)
		return false;
	if (diff * dir < 0)
		return false;
	if (targetRow != startRow + (dir >> 3))
		return false;
	if (Piece::getType(piece) != Piece::Pawn)
		return false;
	//	std::cout <<"yay " << mEnPassant << ' ' << from << ' ' << to << ' ' << to+dir << '\n';
	return (absDiff == 7 || absDiff == 9);
}

bool GameLogic::isLegalPawnMove(int from, int to) const {
	int piece = mBoard.get(from);
	int capture = mBoard.get(to);
	int color = Piece::getColor(piece);
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = color ? -8 : 8;
	int start = color ? 48 : 8;
	int end = color ? 55 : 15;
	int targetRow = to >> 3;
	int startRow = from >> 3;

	if (diff * dir <= 0)
		return false;
	if (absDiff == 8)
		return capture == 0;
	if (absDiff == 16 && from >= start && from <= end)
		return capture == 0 && mBoard.get(from + dir) == 0;
	if (targetRow != startRow + (dir >> 3))
		return false;
	if (absDiff == 7 && capture != 0)
		return Piece::getColor(capture) != color;
	if (absDiff == 9 && capture != 0)
		return Piece::getColor(capture) != color;

	// check for enpassant
	if (isEnPassant(from, to))
		return true;

	return false;
}

bool GameLogic::isLegalKnightMove(int from, int to) {
	int r1 = from >> 3, c1 = from & 7;
	int r2 = to >> 3, c2 = to & 7;
	int dr = r2 - r1, dc = c2 - c1;
	int absDr = dr < 0 ? -dr : dr;
	int absDc = dc < 0 ? -dc : dc;
	if (absDr == 2 && absDc == 1)
		return true;
	if (absDr == 1 && absDc == 2)
		return true;
	return false;
}

bool GameLogic::isLegalBishopMove(int from, int to) const {
	// check for diagonal
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffRank != absDiffFile)
		return false;

	// check for blocking
	int dirRow = diffRank < 0 ? -1 : 1;
	int dirCol = diffFile < 0 ? -1 : 1;
	int rank = from >> 3, file = from & 7;
	for (int i = 1; i < absDiffRank; i++) {
		rank += dirRow;
		file += dirCol;
		if (mBoard.get(rank, file) != 0)
			return false;
	}

	return true;
}

bool GameLogic::isLegalRookMove(int from, int to) const {
	// check for straight
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffRank != 0 && absDiffFile != 0)
		return false;

	// check for block
	int dirRow = diffRank != 0 ? diffRank / absDiffRank : 0;
	int dirCol = diffFile != 0 ? diffFile / absDiffFile : 0;
	int rank = from >> 3, file = from & 7;
	for (int i = 1; i < absDiffRank + absDiffFile; i++) {
		rank += dirRow;
		file += dirCol;
		if (mBoard.get(rank, file) != 0)
			return false;
	}
	return true;
}

bool GameLogic::isLegalQueenMove(int from, int to) const {
	if (isLegalBishopMove(from, to))
		return true;
	if (isLegalRookMove(from, to))
		return true;
	return false;
}

bool GameLogic::isLegalKingMove(int from, int to) const {
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffRank <= 1 && absDiffFile <= 1 && !mAttacks.isAttacked(to))
		return true;

	// check whether pureMove is castling, and castling is legal, using mCastling
	return isLegalCastling(from, to);
}

// [BUG] castling when a piece blocks the way
bool GameLogic::isLegalCastling(int from, int to) const {
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

bool GameLogic::isLegalPromotion(int from, int to) const {
	int piece = mBoard.get(from);
	int color = Piece::getColor(piece);
	int type = Piece::getType(piece);
	if (type != Piece::Pawn)
		return false;
	if (color && Board::getRank(to) != 0)
		return false;
	if (!color && Board::getRank(to) != 7)
		return false;
	return isLegalPawnMove(from, to);
}

int GameLogic::getPiece(int square) const {
	return mBoard.get(square);
}

bool GameLogic::getTurn() const {
	return mBoard.getTurn();
}

