//
// Created by MINH NHUT on 10/25/2023.
//

#include "GameLogic.hpp"
#include "Piece.hpp"

#include <cassert>
#include <iostream>

const int GameLogic::BOARD_SIZE = 64;

GameLogic::GameLogic()
    : mTurn(false), mCastling(0), mEnPassant(-1), mHalfMove(0), mFullMove(0) {
}

bool GameLogic::validSquare(int square) {
	return square >= 0 && square < 64;
}

bool GameLogic::isLegalMove(int from, int to) const {
	if (!validSquare(from) || !validSquare(to))
		return false;
	int piece = mBoard[from];
	int capture = mBoard[to];

//	std::cout << piece << ' ' << mTurn << '\n';
	if (piece == 0 || getColor(piece) != mTurn)
		return false;
	if (capture != 0 && getColor(capture) == mTurn)
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

int GameLogic::getPiece(int box) const {
	return mBoard[box];
}

int GameLogic::getColor(int piece) {
	return (piece >> 3) & 1;
}

int GameLogic::getType(int piece) {
	return piece & 7;
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

void GameLogic::makeMove(int from, int to) {
	assert(isLegalMove(from, to));

	bool captured = false;

	if (isEnPassant(from, to)) {
		capturePiece(mEnPassant + (mTurn ? 8 : -8));
		captured = true;
	}

	if (mBoard[to] != 0) {
		capturePiece(to);
		captured = true;
	}

	updateEnPassant(from, to);
	movePiece(from, to, captured);
	mTurn ^= 1;
}

void GameLogic::addPiece(int piece, int square) {
	std::cout << "add " << piece << ' ' << square << '\n';
	mBoard[square] = piece;
}

void GameLogic::capturePiece(int square) {
	std::cout << "capture " << square << '\n';
	mBoard[square] = 0;
}

void GameLogic::movePiece(int from, int to, bool captured) {
	mBoard[to] = mBoard[from];
	mBoard[from] = 0;
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
			addPiece(getPieceFromChar(ch), getBoxID(row, col++));
		} else
			break;
	}
}

void GameLogic::updateEnPassant(int from, int to) {
	// check and update mEnPassant when pawn move 2 square
	int piece = mBoard[from];
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = getColor(piece) ? -8 : 8;
	if (getType(piece) == Piece::Pawn && absDiff == 16) {
		mEnPassant = from + dir;
	} else mEnPassant = -1;
	std::cout << from << ' ' << to << ' ' << mEnPassant << '\n';
}

bool GameLogic::isEnPassant(int from, int to) const {
	int piece = mBoard[from];
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = getColor(piece) ? -8 : 8;
	int targetRow = to >> 3;
	int startRow = from >> 3;
	if (mEnPassant != to) return false;
	if (diff * dir < 0) return false;
	if (targetRow != startRow + (dir >> 3)) return false;
	if (getType(piece) != Piece::Pawn) return false;
//	std::cout <<"yay " << mEnPassant << ' ' << from << ' ' << to << ' ' << to+dir << '\n';
	return (absDiff == 7 || absDiff == 9);
}

bool GameLogic::isLegalPawnMove(int from, int to) const {
	int piece = mBoard[from];
	int capture = mBoard[to];
	int color = getColor(piece);
	int diff = to - from;
	int absDiff = diff < 0 ? -diff : diff;
	int dir = color ? -8 : 8;
	int start = color ? 48 : 8;
	int end = color ? 55 : 15;
	int targetRow = to >> 3;
	int startRow = from >> 3;

	if (diff * dir <= 0) return false;
	if (absDiff == 8) return capture == 0;
	if (absDiff == 16 && from >= start && from <= end) return capture == 0 && mBoard[from + dir] == 0;
	if (targetRow != startRow + (dir >> 3)) return false;
	if (absDiff == 7 && capture != 0) return getColor(capture) != color;
	if (absDiff == 9 && capture != 0) return getColor(capture) != color;

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
	return false;
}

bool GameLogic::isLegalRookMove(int from, int to) const {
	return false;
}

bool GameLogic::isLegalQueenMove(int from, int to) const {
	return false;
}

bool GameLogic::isLegalKingMove(int from, int to) const {
	int diffRow = (to >> 3) - (from >> 3);
	int diffCol = (to & 7) - (from & 7);
	int absDiffRow = diffRow < 0 ? -diffRow : diffRow;
	int absDiffCol = diffCol < 0 ? -diffCol : diffCol;
	if (absDiffRow <= 1 && absDiffCol <= 1)
		return true;
	return false;
}

bool GameLogic::isKingInCheck(bool turn) const {
	return false;
}
