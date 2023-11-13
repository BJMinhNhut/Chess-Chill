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
	GameLogic(const GameLogic &other);

	[[nodiscard]] bool isLegalMove(int from, int to) const;
	[[nodiscard]] bool isKingInCheck() const;
	[[nodiscard]] bool getTurn() const;

	void makeMove(int from, int to);

   protected:
	virtual void addPiece(int piece, int square);
	virtual void capturePiece(int square);
	virtual void movePiece(int from, int to);
	virtual void postMove(bool captured);

	void loadFEN(const std::string& fen);
	std::string getFEN() const;

	[[nodiscard]] bool isAttacked(int square) const;
	[[nodiscard]] bool isAttacked(int square, bool turn) const;
	[[nodiscard]] bool isKingInCheck(bool turn) const;

   private:
	void updateEnPassant(int from, int to);
	void updateAttacks(bool turn);

	[[nodiscard]] bool isLegalPawnMove(int from, int to) const;
	[[nodiscard]] bool isEnPassant(int from, int to) const;
	static bool isLegalKnightMove(int from, int to);
	[[nodiscard]] bool isLegalBishopMove(int from, int to) const;
	[[nodiscard]] bool isLegalRookMove(int from, int to) const;
	[[nodiscard]] bool isLegalQueenMove(int from, int to) const;
	[[nodiscard]] bool isLegalKingMove(int from, int to) const;

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
