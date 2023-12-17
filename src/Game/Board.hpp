//
// Created by MINH NHUT on 11/13/2023.
//

#ifndef CHESS_BOARD_HPP
#define CHESS_BOARD_HPP

#include <string>
#include <vector>

class Board {
   public:
	explicit Board(const std::string& fen);
	Board(const Board& other);

	static int getRank(int square);
	static int getFile(int square);
	static int getSquareID(int rank, int file);
	static bool validSquare(int square);

	[[nodiscard]] int get(int square) const;
	[[nodiscard]] int get(int rank, int file) const;

	// if empty then return -1
	[[nodiscard]] bool getColor(int square) const;
	[[nodiscard]] bool getColor(int rank, int file) const;
	[[nodiscard]] int getType(int square) const;
	[[nodiscard]] int getType(int rank, int file) const;

	[[nodiscard]] int getKing(bool turn) const;  // if not found return -1
	[[nodiscard]] bool getTurn() const;
	[[nodiscard]] int getCastling() const;
	[[nodiscard]] int getEnPassant() const;
	[[nodiscard]] int getHalfMove() const;
	[[nodiscard]] int getFullMove() const;

	void setTurn(bool turn);
	void nextTurn();
	void setCastling(int castling);
	void setEnPassant(int enPassant);
	void setHalfMove(int halfMove);
	void setFullMove(int fullMove);
	void nextFullMove();

	void updateCastling(int piece, bool side);
	void updateEnPassant(int from, int to);
	void updateHalfMove(int from, int to);

	[[nodiscard]] std::string getFEN(bool withMove = true) const;
	[[nodiscard]] int getHashFEN(bool withMove) const;

	void clear();
	void set(int square, int8_t piece);
	void set(int rank, int file, int8_t piece);
	void move(int from, int to);

   private:
	void loadFEN(const std::string& fen);

   private:
	bool mTurn;
	int8_t mCastling, mEnPassant, mHalfMove, mFullMove;
	int8_t mBoard[64]{};
};

#endif  //CHESS_BOARD_HPP
