//
// Created by MINH NHUT on 11/13/2023.
//

#include "Board.hpp"
#include "Piece.hpp"

#include <cassert>
#include <cstring>
#include <sstream>

Board::Board(const std::string& fen)
    : mTurn(false), mCastling(0), mEnPassant(-1), mHalfMove(0), mFullMove(0), mBoard() {
	clear();
	loadFEN(fen);
}

Board::Board(const Board& other)
    : mTurn(other.mTurn),
      mCastling(other.mCastling),
      mEnPassant(other.mEnPassant),
      mHalfMove(other.mHalfMove),
      mFullMove(other.mFullMove) {
	memcpy(mBoard, other.mBoard, sizeof mBoard);
}

int Board::getRank(int square) {
	return square >> 3;
}

int Board::getFile(int square) {
	return square & 7;
}

int Board::getSquareID(int rank, int file) {
#ifdef DEBUG_BOARD
	if (!(rank >= 0 && rank < 8 && file >= 0 && file < 8))
		throw(std::invalid_argument("Invalid rank or file"));
#endif
	assert(rank >= 0 && rank < 8 && file >= 0 && file < 8);
	return (rank << 3) | file;
}

std::string Board::getSquareName(int square) {
	assert(validSquare(square));
	std::string name;
	name += (char)('a' + getFile(square));
	name += (char)('1' + getRank(square));
	return name;
}

bool Board::validSquare(int square) {
	return square >= 0 && square < 64;
}

void Board::loadFEN(const std::string& fen) {
	std::istringstream iss(fen);
	std::string board;
	iss >> board;
	clear();
	int rank = 7, file = 0;
	for (char ch : board) {
		if (std::isdigit(ch))
			file += ch - '0';
		else if (ch == '/')
			rank--, file = 0;
		else if (std::isalpha(ch)) {
			set(Board::getSquareID(rank, file++), Piece::getPieceFromChar(ch));
		} else
			break;
	}
	std::string turn;
	iss >> turn;
	mTurn = turn == "b";
	std::string castling;
	iss >> castling;
	mCastling = 0;
	for (char ch : castling) {
		if (ch == 'K')
			mCastling |= 1;
		if (ch == 'Q')
			mCastling |= 2;
		if (ch == 'k')
			mCastling |= 4;
		if (ch == 'q')
			mCastling |= 8;
	}
	std::string enPassant;
	iss >> enPassant;
	if (enPassant == "-")
		mEnPassant = -1;
	else
		mEnPassant = Board::getSquareID(enPassant[1] - '1', enPassant[0] - 'a');
	iss >> mHalfMove >> mFullMove;
	assert(getKing(0) != -1 && getKing(1) != -1);
}

std::string Board::getFEN(bool withMove) const {
	std::ostringstream oss;
	for (int rank = 7; rank >= 0; rank--) {
		int empty = 0;
		for (int file = 0; file < 8; file++) {
			int piece = get(rank, file);
			if (piece == 0)
				empty++;
			else {
				if (empty > 0) {
					oss << empty;
					empty = 0;
				}
				oss << Piece::getCharFromPiece(piece);
			}
		}
		if (empty > 0)
			oss << empty;
		if (rank > 0)
			oss << '/';
	}
	oss << ' ';
	oss << (mTurn ? 'b' : 'w') << ' ';
	if (mCastling == 0)
		oss << '-';
	else {
		if (mCastling & 1)
			oss << 'K';
		if (mCastling & 2)
			oss << 'Q';
		if (mCastling & 4)
			oss << 'k';
		if (mCastling & 8)
			oss << 'q';
	}
	oss << ' ';
	if (mEnPassant == -1)
		oss << '-';
	else {
		oss << (char)('a' + Board::getFile(mEnPassant));
		oss << (char)('1' + Board::getRank(mEnPassant));
	}
	if (withMove) {
		oss << ' ';
		oss << mHalfMove << ' ' << mFullMove;
	}
	return oss.str();
}

int Board::getHashFEN(bool withMove) const {
	int hash = 0;
	for (int i = 0; i < 64; ++i)
		hash ^= mBoard[i] * (i + 1);
	hash ^= mTurn * 65;
	hash ^= mCastling * 67;
	hash ^= mEnPassant * 71;
	hash ^= mHalfMove * 73;
	hash ^= mFullMove * 79;
	return hash;
}

void Board::clear() {
	memset(mBoard, 0, sizeof mBoard);
}

void Board::set(int square, int8_t piece) {
	assert(validSquare(square));
	assert(Piece::valid(piece));
	mBoard[square] = piece;
}

void Board::set(int rank, int file, int8_t piece) {
	assert(rank >= 0 && rank < 8 && file >= 0 && file < 8);
	mBoard[getSquareID(rank, file)] = piece;
}

void Board::move(int from, int to) {
	assert(validSquare(from) && validSquare(to));
	assert(from != to);
	mBoard[to] = mBoard[from];
	mBoard[from] = Piece::None;
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
	return -1;
}

bool Board::getTurn() const {
	return mTurn;
}

int Board::getCastling() const {
	return mCastling;
}

int Board::getEnPassant() const {
	return mEnPassant;
}

int Board::getHalfMove() const {
	return mHalfMove;
}

int Board::getFullMove() const {
	return mFullMove;
}

void Board::setTurn(bool turn) {
	mTurn = turn;
}

void Board::nextTurn() {
	mTurn = !mTurn;
}

void Board::setCastling(int castling) {
	mCastling = castling;
}

void Board::setEnPassant(int enPassant) {
	mEnPassant = enPassant;
}

void Board::setHalfMove(int halfMove) {
	mHalfMove = halfMove;
}

void Board::setFullMove(int fullMove) {
	mFullMove = fullMove;
}

void Board::nextFullMove() {
	mFullMove++;
}

void Board::updateCastling(int piece, bool side) {
	int color = Piece::getColor(piece);
	int type = Piece::getType(piece);
	if (type == Piece::King) {
		mCastling &= ~(color ? 12 : 3);
	} else if (type == Piece::Rook) {
		if (side)  // King side
			mCastling &= ~(color ? 4 : 1);
		else  // Queen side
			mCastling &= ~(color ? 8 : 2);
	}
}

void Board::updateEnPassant(int from, int to) {
	int piece = get(from);
	if (Piece::getType(piece) == Piece::Pawn && std::abs(from - to) == 16)
		mEnPassant = (from + to) / 2;
	else
		mEnPassant = -1;
}

void Board::updateHalfMove(int pieceFrom, int pieceTo) {
	if (Piece::getType(pieceFrom) == Piece::Pawn || pieceTo != 0)
		mHalfMove = 0;
	else
		mHalfMove++;
}
