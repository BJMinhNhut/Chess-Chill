//
// Created by MINH NHUT on 11/13/2023.
//

#include "AttackGenerator.hpp"
#include "Piece.hpp"

namespace AttackGenerator {
int64_t generatePawnAttacks(int square, bool color) {
	int64_t attacks = 0;
	int rank = Board::getRank(square);
	int file = Board::getFile(square);
	int direction = color ? -1 : 1;

	if (rank + direction >= 0 && rank + direction < 8) {
		if (file > 0) {
			attacks |= 1LL << Board::getSquareID(rank + direction, file - 1);
		}
		if (file < 7) {
			attacks |= 1LL << Board::getSquareID(rank + direction, file + 1);
		}
	}

	return attacks;
}

int64_t generateKnightAttacks(int square) {
	int64_t attacks = 0;
	int rank = Board::getRank(square);
	int file = Board::getFile(square);

	if (rank + 2 < 8) {
		if (file + 1 < 8) {
			attacks |= 1LL << Board::getSquareID(rank + 2, file + 1);
		}
		if (file - 1 >= 0) {
			attacks |= 1LL << Board::getSquareID(rank + 2, file - 1);
		}
	}
	if (rank - 2 >= 0) {
		if (file + 1 < 8) {
			attacks |= 1LL << Board::getSquareID(rank - 2, file + 1);
		}
		if (file - 1 >= 0) {
			attacks |= 1LL << Board::getSquareID(rank - 2, file - 1);
		}
	}
	if (file + 2 < 8) {
		if (rank + 1 < 8) {
			attacks |= 1LL << Board::getSquareID(rank + 1, file + 2);
		}
		if (rank - 1 >= 0) {
			attacks |= 1LL << Board::getSquareID(rank - 1, file + 2);
		}
	}
	if (file - 2 >= 0) {
		if (rank + 1 < 8) {
			attacks |= 1LL << Board::getSquareID(rank + 1, file - 2);
		}
		if (rank - 1 >= 0) {
			attacks |= 1LL << Board::getSquareID(rank - 1, file - 2);
		}
	}

	return attacks;
}

int64_t generateBishopAttacks(int square, Board& board) {
	int64_t attacks = 0;
	int rank = Board::getRank(square);
	int file = Board::getFile(square);

	for (int i = 1; i < 8; i++) {
		if (rank + i > 7 || file + i > 7)
			break;
		attacks |= 1LL << Board::getSquareID(rank + i, file + i);
		if (board.get(rank + i, file + i) != 0)
			break;
	}
	for (int i = 1; i < 8; i++) {
		if (rank + i > 7 || file - i < 0)
			break;
		attacks |= 1LL << Board::getSquareID(rank + i, file - i);
		if (board.get(rank + i, file - i) != 0)
			break;
	}
	for (int i = 1; i < 8; i++) {
		if (rank - i < 0 || file + i > 7)
			break;
		attacks |= 1LL << Board::getSquareID(rank - i, file + i);
		if (board.get(rank - i, file + i) != 0)
			break;
	}
	for (int i = 1; i < 8; i++) {
		if (rank - i < 0 || file - i < 0)
			break;
		attacks |= 1LL << Board::getSquareID(rank - i, file - i);
		if (board.get(rank - i, file - i) != 0)
			break;
	}
	return attacks;
}

int64_t generateRookAttacks(int square, Board& board) {
	int64_t attacks = 0;
	int rank = Board::getRank(square);
	int file = Board::getFile(square);

	for (int i = 1; i < 8; i++) {
		if (rank + i > 7)
			break;
		attacks |= 1LL << Board::getSquareID(rank + i, file);
		if (board.get(rank + i, file) != 0)
			break;
	}
	for (int i = 1; i < 8; i++) {
		if (rank - i < 0)
			break;
		attacks |= 1LL << Board::getSquareID(rank - i, file);
		if (board.get(rank - i, file) != 0)
			break;
	}
	for (int i = 1; i < 8; i++) {
		if (file + i > 7)
			break;
		attacks |= 1LL << Board::getSquareID(rank, file + i);
		if (board.get(rank, file + i) != 0)
			break;
	}
	for (int i = 1; i < 8; i++) {
		if (file - i < 0)
			break;
		attacks |= 1LL << Board::getSquareID(rank, file - i);
		if (board.get(rank, file - i) != 0)
			break;
	}
	return attacks;
}

int64_t generateQueenAttacks(int square, Board& board) {
	return generateBishopAttacks(square, board) | generateRookAttacks(square, board);
}

int64_t generateKingAttacks(int square) {
	int64_t attacks = 0;
	int rank = Board::getRank(square);
	int file = Board::getFile(square);

	for (int dr = -1; dr <= 1; ++dr)
		if (rank + dr >= 0 && rank + dr < 8) {
			for (int df = -1; df <= 1; ++df)
				if (file + df >= 0 && file + df < 8) {
					if (dr == 0 && df == 0)
						continue;
					attacks |= 1LL << Board::getSquareID(rank + dr, file + df);
				}
		}
	return attacks;
}

}  // namespace AttackGenerator