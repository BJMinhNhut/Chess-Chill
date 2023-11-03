//
// Created by MINH NHUT on 10/25/2023.
//

#include "GameLogic.hpp"
#include "Piece.hpp"

#include <cassert>

const int GameLogic::BOARD_SIZE = 64;

GameLogic::GameLogic(const std::string& fen):
	mTurn(false), mCastling(0), mEnPassant(0), mHalfMove(0), mFullMove(0) {
	loadFEN(fen);
}

bool GameLogic::isLegalMove(int move) const {
	int piece = mBoard[move & 63];
	if (piece == 0 || getColor(piece) != mTurn) return false;
	switch (piece & 7) {
		case Piece::Pawn:
			return isLegalPawnMove(move);
		case Piece::Knight:
			return isLegalKnightMove(move);
		case Piece::Bishop:
			return isLegalBishopMove(move);
		case Piece::Rook:
			return isLegalRookMove(move);
		case Piece::Queen:
			return isLegalQueenMove(move);
		case Piece::King:
			return isLegalKingMove(move);
		default:
			assert(false);
	};
	return false;
}

bool GameLogic::isKingInCheck() const {
	return isKingInCheck(mTurn);
}

int GameLogic::getPiece(int box) const {
	return mBoard[box];
}

int GameLogic::getColor(int piece) {
	return piece & 8;
}

int GameLogic::getPieceFromChar(char ch) {
	int piece = std::islower(ch) ? Piece::Black : Piece::White;
	switch (tolower(ch)) {
		case 'p':
			piece |= Piece::Pawn;
			break;
		case 'n':
			piece |= Piece::Knight;
			break;
		case 'b':
			piece |= Piece::Bishop;
			break;
		case 'r':
			piece |= Piece::Rook;
			break;
		case 'q':
			piece |= Piece::Queen;
			break;
		case 'k':
			piece |= Piece::King;
			break;
		default:
			assert(false);
	}
	return piece;
}

int GameLogic::getBoxID(int row, int column) {
	assert(row >= 0 && row < 8 && column >= 0 && column < 8);
	return (row << 3) | column;
}

void GameLogic::getMovePositions(int move, int& start, int& target) {
	start = move & 63;
	target = (move >> 6) & 63;
}

void GameLogic::makeMove(int move) {
	assert(isLegalMove(move));
	int from, to;
	getMovePositions(move, from, to);
	mBoard[to] = mBoard[from];
	mBoard[from] = 0;
}

void GameLogic::loadFEN(const std::string& fen) {
	std::fill(mBoard, mBoard + BOARD_SIZE, 0);
	int row = 7, col = 7;
	for (char ch : fen) {
		if (std::isdigit(ch))
			col -= ch - '0';
		else if (ch == '/')
			row--, col = 7;
		else if (std::isalpha(ch)) {
			mBoard[getBoxID(row, col--)] = getPieceFromChar(ch);
		} else
			break;
	}
}

bool GameLogic::isLegalPawnMove(int move) const {
//	int from = move & 63;
//	int to = (move >> 6) & 63;
//	int piece = mBoard[from];
//	int capture = mBoard[to];
//	int isBlack = piece & 8;
//	int diff = to - from;
//	int absDiff = diff < 0 ? -diff : diff;
//	int dir = isBlack ? -8 : 8;
//	int start = isBlack ? 48 : 8;
//	int end = isBlack ? 55 : 15;
//	int ep = mEnPassant;
//	int epDiff = ep - from;
//	int epAbsDiff = epDiff < 0 ? -epDiff : epDiff;
//	int epDir = isBlack ? -7 : 9;
//	int epCapture = mBoard[ep];
//
//	if (capture == 0) {
//		if (diff == dir) {
//			if (to < start || to > end) return false;
//			if (ep != -1 && epAbsDiff == 1 && epCapture == (Piece::Pawn | !isBlack)) return true;
//			return true;
//		}
//		if (diff == 2 * dir && from >= start && from <= end) {
//			if (mBoard[from + dir] != 0) return false;
//			if (mBoard[to] != 0) return false;
//			return true;
//		}
//		return false;
//	}
//	if (getColor(capture) == isBlack) return false;
//	if (diff == dir + 1 || diff == dir - 1) return true;
//	if (ep != -1 && epAbsDiff == 1 && epCapture == (Piece::Pawn | !isBlack)) return true;
	return false;
}

bool GameLogic::isLegalKnightMove(int move) const {
	int from = move & 63;
	int to = (move >> 6) & 63;
	int piece = mBoard[from];
	int capture = mBoard[to];
	int color = piece & 8;
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;

	if (capture != 0 && getColor(capture) == color) return false;
	if (absDiff == 6 || absDiff == 10 || absDiff == 15 || absDiff == 17) return true;
	return false;
}

bool GameLogic::isLegalBishopMove(int move) const {
	return false;
}

bool GameLogic::isLegalRookMove(int move) const {
	return false;
}

bool GameLogic::isLegalQueenMove(int move) const {
	return false;
}

bool GameLogic::isLegalKingMove(int move) const {
//	int from = move & 63;
//	int to = (move >> 6) & 63;
//	int piece = mBoard[from];
//	int capture = mBoard[to];
//	int isBlack = piece & 8;
//	int diff = to - from;
//	int absDiff = diff < 0 ? -diff : diff;
//	int dir = isBlack ? -8 : 8;
//	int start = isBlack ? 60 : 4;
//	int end = isBlack ? 63 : 7;
//	int castle = mCastling;
//	int castleDiff = castle - from;
//	int castleAbsDiff = castleDiff < 0 ? -castleDiff : castleDiff;
//	int castleDir = castleDiff < 0 ? -1 : 1;
//	int castleRook = mBoard[castle];
//	int castleTo = from + castleDir;
//	int castleCapture = mBoard[castleTo];
//
//	if (capture != 0 && getColor(capture) == isBlack) return false;
//	if (absDiff == 1 || absDiff == 8 || absDiff == 9) return true;
//	if (castle != -1 && castleAbsDiff == 2 && castleRook == (Piece::Rook | isBlack) &&
//	    castleCapture == 0) {
//		if (isKingInCheck()) return false;
//		if (isKingInCheck(isBlack)) return false;
//		return true;
//	}
	return false;
}

bool GameLogic::isKingInCheck(bool turn) const {
	return false;
}
