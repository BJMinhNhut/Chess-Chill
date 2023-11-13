//
// Created by MINH NHUT on 11/13/2023.
//

#include "Board.hpp"
#include "Piece.hpp"

#include <cassert>
#include <cstring>

Board::Board() {
	clear();
}

int Board::getRank(int square) {
	return square >> 3;
}

int Board::getFile(int square) {
	return square & 7;
}

int Board::getSquareID(int rank, int file) {
	assert(rank >= 0 && rank < 8 && file >= 0 && file < 8);
	return (rank << 3) | file;
}

bool Board::validSquare(int square) {
	return square >= 0 && square < 64;
}

void Board::clear() {
	std::memset(mBoard, 0, sizeof mBoard);
}

void Board::set(int square, int piece) {
	assert(validSquare(square));
	assert(Piece::valid(piece));
	mBoard[square] = piece;
}

void Board::set(int rank, int file, int piece) {
	assert(rank >= 0 && rank < 8 && file >= 0 && file < 8);
	mBoard[getSquareID(rank, file)] = piece;
}

int Board::get(int square) const {
	assert(validSquare(square));
	return mBoard[square];
}

int Board::get(int rank, int file) const {
	assert(rank >= 0 && rank < 8 && file >= 0 && file < 8);
	return mBoard[getSquareID(rank, file)];
}

bool Board::getColor(int square) const {
	assert(validSquare(square));
	// if empty then return -1
	if (mBoard[square] == 0)
		return -1;
	return Piece::getColor(mBoard[square]);
}

bool Board::getColor(int rank, int file) const {
	assert(rank >= 0 && rank < 8 && file >= 0 && file < 8);
	// if empty then return -1
	if (mBoard[getSquareID(rank, file)] == 0)
		return -1;
	return Piece::getColor(mBoard[getSquareID(rank, file)]);
}

int Board::getType(int square) const {
	assert(validSquare(square));
	// if empty then return -1
	if (mBoard[square] == 0)
		return -1;
	return Piece::getType(mBoard[square]);
}

int Board::getType(int rank, int file) const {
	assert(rank >= 0 && rank < 8 && file >= 0 && file < 8);
	// if empty then return -1
	if (mBoard[getSquareID(rank, file)] == 0)
		return -1;
	return Piece::getType(mBoard[getSquareID(rank, file)]);
}

int Board::getKing(bool turn) const {
	for (int i = 0; i < 64; ++i)
		if (getType(i) == Piece::King && getColor(i) == turn)
			return i;
	assert(false);
}

void Board::move(int from, int to) {
	assert(validSquare(from) && validSquare(to));
	mBoard[to] = mBoard[from];
	mBoard[from] = 0;
}
