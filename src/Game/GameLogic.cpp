//
// Created by MINH NHUT on 10/25/2023.
//

#include "GameLogic.hpp"
#include "Piece.hpp"

#include <cassert>
#include <iostream>

const int GameLogic::BOARD_SIZE = 64;

GameLogic::GameLogic(const std::string& fen)
    : mTurn(false), mCastling(0), mEnPassant(-1), mHalfMove(0), mFullMove(0) {
	loadFEN(fen);
}

bool GameLogic::isLegalMove(int move) const {
	int piece = mBoard[move & 63];
	int capture = mBoard[(move >> 6) & 63];

	std::cout << piece << ' ' << mTurn << '\n';
	if (piece == 0 || getColor(piece) != mTurn)
		return false;
	if (capture != 0 && getColor(capture) == mTurn)
		return false;
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
	return (piece >> 3) & 1;
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

bool GameLogic::getTurn() const {
	return mTurn;
}

void GameLogic::getMovePositions(int move, int& start, int& target) {
	start = move & 63;
	target = (move >> 6) & 63;
}

void GameLogic::makeMove(int move) {
	assert(isLegalMove(move));
	int from, to;
	getMovePositions(move, from, to);
	checkEnPassant(move);
	mBoard[to] = mBoard[from];
	mBoard[from] = 0;
	mTurn ^= 1;
}

void GameLogic::loadFEN(const std::string& fen) {
	std::fill(mBoard, mBoard + BOARD_SIZE, 0);
	int row = 7, col = 0;
	for (char ch : fen) {
		if (std::isdigit(ch))
			col += ch - '0';
		else if (ch == '/')
			row--, col = 0;
		else if (std::isalpha(ch)) {
			mBoard[getBoxID(row, col++)] = getPieceFromChar(ch);
		} else
			break;
	}
}

void GameLogic::checkEnPassant(int move) {
	// check and update mEnPassant when pawn move 2 square
	int from = move & 63;
	int to = (move >> 6) & 63;
	int piece = mBoard[from];
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = getColor(piece) ? -8 : 8;
	if ((piece & 7) == Piece::Pawn && absDiff == 16) {
		mEnPassant = from + dir;
	} else mEnPassant = -1;
	std::cout << from << ' ' << to << ' ' << mEnPassant << '\n';
}

bool GameLogic::isLegalPawnMove(int move) const {
	int from = move & 63;
	int to = (move >> 6) & 63;
	int piece = mBoard[from];
	int capture = mBoard[to];
	int color = getColor(piece);
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = color ? -8 : 8;
	int start = color ? 48 : 8;
	int end = color ? 55 : 15;

	if (diff * dir <= 0) return false;
	if (absDiff == 8) return capture == 0;
	if (absDiff == 16 && from >= start && from <= end) return capture == 0 && mBoard[from + dir] == 0;
	if (absDiff == 7 && capture != 0) return getColor(capture) != color;
	if (absDiff == 9 && capture != 0) return getColor(capture) != color;

	// check for enpassant
	if (mEnPassant != -1) {
		if (to == mEnPassant) {
			int enPassantCapture = mBoard[mEnPassant + dir];
			if (enPassantCapture != 0 && getColor(enPassantCapture) != color) return true;
		}
	}
	return false;
}

bool GameLogic::isLegalKnightMove(int move) {
	int from = move & 63;
	int to = (move >> 6) & 63;
	int r1 = from >> 3, c1 = from & 7;
	int r2 = to >> 3, c2 = to & 7;
	int dr = r2 - r1, dc = c2 - c1;
	int absDr = dr < 0 ? -dr : dr;
	int absDc = dc < 0 ? -dc : dc;
	if (absDr == 2 && absDc == 1) return true;
	if (absDr == 1 && absDc == 2) return true;
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
