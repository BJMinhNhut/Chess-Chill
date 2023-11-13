//
// Created by MINH NHUT on 10/25/2023.
//

#include "GameLogic.hpp"
#include "Piece.hpp"
#include "AttackGenerator.hpp"

#include <cassert>
#include <iostream>

const int GameLogic::BOARD_SIZE = 64;

GameLogic::GameLogic()
    : mTurn(false), mCastling(0), mEnPassant(-1), mHalfMove(0), mFullMove(0), mBoard() {
}

bool GameLogic::isLegalMove(int from, int to) const {
	if (from == to || !Board::validSquare(from) || !Board::validSquare(to))
		return false;
	int piece = mBoard.get(from);
	int capture = mBoard.get(to);

//	std::cout << piece << ' ' << mTurn << '\n';
	if (piece == 0 || Piece::getColor(piece) != mTurn)
		return false;
	if (capture != 0 && Piece::getColor(capture) == mTurn)
		return false;
	switch (piece & 7) {
		case Piece::Pawn:
			return isLegalPawnMove(from, to);
		case Piece::Knight:
			return isLegalKnightMove(from, to);
		case Piece::Bishop:
			return isLegalBishopMove(from, to);
		case Piece::Rook:
			return isLegalRookMove(from, to);
		case Piece::Queen:
			return isLegalQueenMove(from, to);
		case Piece::King:
			return isLegalKingMove(from, to);
		default:
			assert(false);
	};
	return false;
}

bool GameLogic::isKingInCheck() const {
	return isKingInCheck(mTurn);
}

bool GameLogic::getTurn() const {
	return mTurn;
}

void GameLogic::makeMove(int from, int to) {
	assert(isLegalMove(from, to));

	bool captured = false;

	if (isEnPassant(from, to)) {
		capturePiece(mEnPassant + (mTurn ? 8 : -8));
		captured = true;
	}

	if (mBoard.get(to) != 0) {
		capturePiece(to);
		captured = true;
	}

	updateEnPassant(from, to);
	movePiece(from, to);
	postMove(captured);
}

void GameLogic::addPiece(int piece, int square) {
	std::cout << "add " << piece << ' ' << square << '\n';
	mBoard.set(square, piece);
}

void GameLogic::capturePiece(int square) {
	std::cout << "capture " << square << '\n';
	mBoard.set(square, 0);
}

void GameLogic::movePiece(int from, int to) {
	mBoard.move(from, to);
}

void GameLogic::postMove(bool captured) {
	if (mTurn) mFullMove++;
	updateAttacks(mTurn);
	mTurn ^= 1;
}

void GameLogic::loadFEN(const std::string& fen) {
	mBoard.clear();
	int rank = 7, file = 0;
	for (char ch : fen) {
		if (std::isdigit(ch))
			file += ch - '0';
		else if (ch == '/')
			rank--, file = 0;
		else if (std::isalpha(ch)) {
			addPiece(Piece::getPieceFromChar(ch), Board::getSquareID(rank, file++));
		} else
			break;
	}
}

void GameLogic::updateEnPassant(int from, int to) {
	// check and update mEnPassant when pawn move 2 square
	int piece = mBoard.get(from);
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = Piece::getColor(piece) ? -8 : 8;
	if (Piece::getType(piece) == Piece::Pawn && absDiff == 16) {
		mEnPassant = from + dir;
	} else mEnPassant = -1;
	std::cout << from << ' ' << to << ' ' << mEnPassant << '\n';
}

void GameLogic::updateAttacks(bool turn) {
	mAttackBoard[turn] = 0;
	for (int i = 0; i < 64; i++) {
		int piece = mBoard.get(i);
		if (piece == 0 || Piece::getColor(piece) != turn) continue;
		switch (Piece::getType(piece)) {
			case Piece::Pawn:
				mAttackBoard[turn] |= AttackGenerator::generatePawnAttacks(i, turn);
				break;
			case Piece::Knight:
				mAttackBoard[turn] |= AttackGenerator::generateKnightAttacks(i);
				break;
			case Piece::Bishop:
				mAttackBoard[turn] |= AttackGenerator::generateBishopAttacks(i, mBoard);
				break;
			case Piece::Rook:
				mAttackBoard[turn] |= AttackGenerator::generateRookAttacks(i, mBoard);
				break;
			case Piece::Queen:
				mAttackBoard[turn] |= AttackGenerator::generateQueenAttacks(i, mBoard);
				break;
			case Piece::King:
				mAttackBoard[turn] |= AttackGenerator::generateKingAttacks(i);
				break;
			default:
				assert(false);
		}
	}
}

bool GameLogic::isEnPassant(int from, int to) const {
	int piece = mBoard.get(from);
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = Piece::getColor(piece) ? -8 : 8;
	int targetRow = to >> 3;
	int startRow = from >> 3;
	if (mEnPassant != to) return false;
	if (diff * dir < 0) return false;
	if (targetRow != startRow + (dir >> 3)) return false;
	if (Piece::getType(piece) != Piece::Pawn) return false;
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

	if (diff * dir <= 0) return false;
	if (absDiff == 8) return capture == 0;
	if (absDiff == 16 && from >= start && from <= end) return capture == 0 && mBoard.get(from + dir) == 0;
	if (targetRow != startRow + (dir >> 3)) return false;
	if (absDiff == 7 && capture != 0) return Piece::getColor(capture) != color;
	if (absDiff == 9 && capture != 0) return Piece::getColor(capture) != color;

	// check for enpassant
	if (isEnPassant(from, to)) return true;

	return false;
}


bool GameLogic::isLegalKnightMove(int from, int to) {
	int r1 = from >> 3, c1 = from & 7;
	int r2 = to >> 3, c2 = to & 7;
	int dr = r2 - r1, dc = c2 - c1;
	int absDr = dr < 0 ? -dr : dr;
	int absDc = dc < 0 ? -dc : dc;
	if (absDr == 2 && absDc == 1) return true;
	if (absDr == 1 && absDc == 2) return true;
	return false;
}

bool GameLogic::isLegalBishopMove(int from, int to) const {
	// check for diagonal
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffRank != absDiffFile) return false;

	// check for blocking
	int dirRow = diffRank < 0 ? -1 : 1;
	int dirCol = diffFile < 0 ? -1 : 1;
	int rank = from >> 3, file = from & 7;
	for (int i = 1; i < absDiffRank; i++) {
		rank += dirRow;
		file += dirCol;
		if (mBoard.get(rank, file) != 0) return false;
	}

	return true;
}

bool GameLogic::isLegalRookMove(int from, int to) const {
	// check for straight
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffRank != 0 && absDiffFile != 0) return false;

	// check for block
	int dirRow = diffRank != 0 ? diffRank/absDiffRank : 0;
	int dirCol = diffFile != 0 ? diffFile/absDiffFile : 0;
	int rank = from >> 3, file = from & 7;
	for (int i = 1; i < absDiffRank + absDiffFile; i++) {
		rank += dirRow;
		file += dirCol;
		if (mBoard.get(rank, file) != 0) return false;
	}
	return true;
}

bool GameLogic::isLegalQueenMove(int from, int to) const {
	if (isLegalBishopMove(from, to)) return true;
	if (isLegalRookMove(from, to)) return true;
	return false;
}

bool GameLogic::isLegalKingMove(int from, int to) const {
	int diffRank = (to >> 3) - (from >> 3);
	int diffFile = (to & 7) - (from & 7);
	int absDiffRank = diffRank < 0 ? -diffRank : diffRank;
	int absDiffFile = diffFile < 0 ? -diffFile : diffFile;
	if (absDiffRank <= 1 && absDiffFile <= 1 && !isAttacked(to))
		return true;
	return false;
}

bool GameLogic::isKingInCheck(bool turn) const {
	int king = mBoard.getKing(turn);
	return isAttacked(king, turn);
}

bool GameLogic::isAttacked(int square) const {
	return mAttackBoard[(!mTurn)] & (1LL << square);
}

bool GameLogic::isAttacked(int square, bool turn) const {
	return mAttackBoard[!turn] & (1LL << square);
}
