//
// Created by MINH NHUT on 10/25/2023.
//

#ifndef CHESS_GAMELOGIC_HPP
#define CHESS_GAMELOGIC_HPP

#include "Board.hpp"

#include <string>

class GameLogic {
   public:
	static const int BOARD_SIZE;

   public:
	GameLogic();

	[[nodiscard]] bool isLegalMove(int from, int to) const;
	[[nodiscard]] bool isKingInCheck() const;
	[[nodiscard]] bool getTurn() const;

	void makeMove(int from, int to);

   protected:
	virtual void addPiece(int piece, int square);
	virtual void capturePiece(int square);
	virtual void movePiece(int from, int to, bool captured);

	void loadFEN(const std::string& fen);
	std::string getFEN() const;

	[[nodiscard]] bool isAttacked(int square) const;

   private:
	void updateEnPassant(int from, int to);
	void updateAttacks(bool turn);

	bool isLegalPawnMove(int from, int to) const;
	bool isEnPassant(int from, int to) const;
	static bool isLegalKnightMove(int from, int to);
	bool isLegalBishopMove(int from, int to) const;
	bool isLegalRookMove(int from, int to) const;
	bool isLegalQueenMove(int from, int to) const;
	bool isLegalKingMove(int from, int to) const;
	bool isKingInCheck(bool turn) const;

   private:
	Board mBoard;
	int64_t mAttackBoard[2];
	bool mTurn;
	int mCastling;
	int mEnPassant;
	int mHalfMove;
	int mFullMove;
};

#endif  //CHESS_GAMELOGIC_HPP
