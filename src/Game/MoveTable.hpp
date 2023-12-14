//
// Created by MINH NHUT on 12/14/2023.
//

#ifndef CHESS_CHILL_MOVETABLE_HPP
#define CHESS_CHILL_MOVETABLE_HPP

#include <cstdint>

class MoveTable {
   public:
	MoveTable() = default;
	MoveTable(const MoveTable&) = delete;
	MoveTable& operator=(const MoveTable&) = delete;

	static MoveTable *getInstance();

	[[nodiscard]] int64_t getMoves(int piece, int square);

   private:
	void generate();

	int64_t getPawnMoves(int square, bool color);
	int64_t getKnightMoves(int square);
	int64_t getBishopMoves(int square);
	int64_t getRookMoves(int square);
	int64_t getQueenMoves(int square);
	int64_t getKingMoves(int square);

	void generatePawnMoves(int square, bool color);
	void generateKnightMoves(int square);
	void generateBishopMoves(int square);
	void generateRookMoves(int square);
	void generateQueenMoves(int square);
	void generateKingMoves(int square);

   private:
	int64_t mPawnMoves[2][64];
	int64_t mKnightMoves[64];
	int64_t mBishopMoves[64];
	int64_t mRookMoves[64];
	int64_t mQueenMoves[64];
	int64_t mKingMoves[64];
	bool mIsGenerated;
};

#endif  //CHESS_CHILL_MOVETABLE_HPP
