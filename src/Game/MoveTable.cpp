//
// Created by MINH NHUT on 12/14/2023.
//

#include "MoveTable.hpp"
#include "Board.hpp"
#include "Piece.hpp"

#include <cassert>

MoveTable *MoveTable::getInstance() {
	static MoveTable* instance = nullptr;
	if (instance == nullptr) {
		instance = new MoveTable();
		instance->generate();
	}
	return instance;
}

void MoveTable::generate() {
	assert(!mIsGenerated);
	for (int i = 0; i < 64; i++) {
		generatePawnMoves(i, true);
		generatePawnMoves(i, false);
		generateKnightMoves(i);
		generateBishopMoves(i);
		generateRookMoves(i);
		generateQueenMoves(i);
		generateKingMoves(i);
	}
	mIsGenerated = true;
}

int64_t MoveTable::getMoves(GameOptions::Type type, int piece, int square) {
	assert(mIsGenerated);
	switch (Piece::getType(piece)) {
		case Piece::Pawn:
			return getPawnMoves(square, Piece::getColor(piece));
		case Piece::Knight:
			return getKnightMoves(square);
		case Piece::Bishop:
			return getBishopMoves(square);
		case Piece::Rook:
			return getRookMoves(square);
		case Piece::Queen:
			return getQueenMoves(square);
		case Piece::King:
			return getKingMoves(type, square);
		default:
			assert(false);
			return 0;
	}
}

int64_t MoveTable::getPawnMoves(int square, bool color) {
	return mPawnMoves[color][square];
}

int64_t MoveTable::getKnightMoves(int square) {
	return mKnightMoves[square];
}

int64_t MoveTable::getBishopMoves(int square) {
	return mBishopMoves[square];
}

int64_t MoveTable::getRookMoves(int square) {
	return mRookMoves[square];
}

int64_t MoveTable::getQueenMoves(int square) {
	return mQueenMoves[square];
}

int64_t MoveTable::getKingMoves(GameOptions::Type type, int square) {
	return mKingMoves[type][square];
}

void MoveTable::generatePawnMoves(int square, bool color) {
	mPawnMoves[color][square] = 0;
	int rank = Board::getRank(square);
	int file = Board::getFile(square);
	int direction = color ? -1 : 1;

	if (rank + direction >= 0 && rank + direction < 8) {
		mPawnMoves[color][square] |= 1LL << Board::getSquareID(rank + direction, file);
		if ((color && rank == 6) || (!color && rank == 1)) {
			mPawnMoves[color][square] |= 1LL << Board::getSquareID(rank + 2 * direction, file);
		}
	}

	// pawn attacks
	if (rank + direction >= 0 && rank + direction < 8) {
		if (file + 1 < 8) {
			mPawnMoves[color][square] |= 1LL << Board::getSquareID(rank + direction, file + 1);
		}
		if (file - 1 >= 0) {
			mPawnMoves[color][square] |= 1LL << Board::getSquareID(rank + direction, file - 1);
		}
	}
}

void MoveTable::generateKnightMoves(int square) {
	int64_t moves = 0;
	int rank = Board::getRank(square);
	int file = Board::getFile(square);

	if (rank + 2 < 8) {
		if (file + 1 < 8) {
			moves |= 1LL << Board::getSquareID(rank + 2, file + 1);
		}
		if (file - 1 >= 0) {
			moves |= 1LL << Board::getSquareID(rank + 2, file - 1);
		}
	}
	if (rank - 2 >= 0) {
		if (file + 1 < 8) {
			moves |= 1LL << Board::getSquareID(rank - 2, file + 1);
		}
		if (file - 1 >= 0) {
			moves |= 1LL << Board::getSquareID(rank - 2, file - 1);
		}
	}
	if (file + 2 < 8) {
		if (rank + 1 < 8) {
			moves |= 1LL << Board::getSquareID(rank + 1, file + 2);
		}
		if (rank - 1 >= 0) {
			moves |= 1LL << Board::getSquareID(rank - 1, file + 2);
		}
	}
	if (file - 2 >= 0) {
		if (rank + 1 < 8) {
			moves |= 1LL << Board::getSquareID(rank + 1, file - 2);
		}
		if (rank - 1 >= 0) {
			moves |= 1LL << Board::getSquareID(rank - 1, file - 2);
		}
	}

	mKnightMoves[square] = moves;
}

void MoveTable::generateBishopMoves(int square) {
	int64_t moves = 0;
	int rank = Board::getRank(square);
	int file = Board::getFile(square);

	for (int i = 1; i < 8; i++) {
		if (rank + i < 8 && file + i < 8) {
			moves |= 1LL << Board::getSquareID(rank + i, file + i);
		}
	}
	for (int i = 1; i < 8; i++) {
		if (rank + i < 8 && file - i >= 0) {
			moves |= 1LL << Board::getSquareID(rank + i, file - i);
		}
	}
	for (int i = 1; i < 8; i++) {
		if (rank - i >= 0 && file + i < 8) {
			moves |= 1LL << Board::getSquareID(rank - i, file + i);
		}
	}
	for (int i = 1; i < 8; i++) {
		if (rank - i >= 0 && file - i >= 0) {
			moves |= 1LL << Board::getSquareID(rank - i, file - i);
		}
	}

	mBishopMoves[square] = moves;
}

void MoveTable::generateRookMoves(int square) {
	int64_t moves = 0;
	int rank = Board::getRank(square);
	int file = Board::getFile(square);

	for (int i = 1; i < 8; i++) {
		if (rank + i < 8) {
			moves |= 1LL << Board::getSquareID(rank + i, file);
		}
	}
	for (int i = 1; i < 8; i++) {
		if (rank - i >= 0) {
			moves |= 1LL << Board::getSquareID(rank - i, file);
		}
	}
	for (int i = 1; i < 8; i++) {
		if (file + i < 8) {
			moves |= 1LL << Board::getSquareID(rank, file + i);
		}
	}
	for (int i = 1; i < 8; i++) {
		if (file - i >= 0) {
			moves |= 1LL << Board::getSquareID(rank, file - i);
		}
	}

	mRookMoves[square] = moves;
}

void MoveTable::generateQueenMoves(int square) {
	mQueenMoves[square] = mBishopMoves[square] | mRookMoves[square];
}

void MoveTable::generateKingMoves(int square) {
	int64_t moves = 0;
	int rank = Board::getRank(square);
	int file = Board::getFile(square);

	if (rank + 1 < 8) {
		moves |= 1LL << Board::getSquareID(rank + 1, file);
		if (file + 1 < 8)
			moves |= 1LL << Board::getSquareID(rank + 1, file + 1);
		if (file - 1 >= 0)
			moves |= 1LL << Board::getSquareID(rank + 1, file - 1);
	}
	if (rank - 1 >= 0) {
		moves |= 1LL << Board::getSquareID(rank - 1, file);
		if (file + 1 < 8)
			moves |= 1LL << Board::getSquareID(rank - 1, file + 1);
		if (file - 1 >= 0)
			moves |= 1LL << Board::getSquareID(rank - 1, file - 1);
	}
	if (file + 1 < 8)
		moves |= 1LL << Board::getSquareID(rank, file + 1);
	if (file - 1 >= 0)
		moves |= 1LL << Board::getSquareID(rank, file - 1);

	mKingMoves[GameOptions::Standard][square] = moves;
	mKingMoves[GameOptions::Chess960][square] = moves;


	// standard
	int64_t castling = 0;
	if (rank == 0 && file == 4) {
		castling |= 1LL << Board::getSquareID(0, 6);
		castling |= 1LL << Board::getSquareID(0, 2);
	}
	if (rank == 7 && file == 4) {
		castling |= 1LL << Board::getSquareID(7, 6);
		castling |= 1LL << Board::getSquareID(7, 2);
	}
	mKingMoves[GameOptions::Standard][square] |= castling;

	//chess960
	castling = 0;
	if (rank == 0) {
		castling = 255;
	} else if (rank == 7) {
		castling = 255ll << 56;
	}
	mKingMoves[GameOptions::Chess960][square] |= castling;
}
