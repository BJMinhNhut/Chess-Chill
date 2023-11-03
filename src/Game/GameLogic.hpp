//
// Created by MINH NHUT on 10/25/2023.
//

#ifndef CHESS_GAMELOGIC_HPP
#define CHESS_GAMELOGIC_HPP

#include <string>

class GameLogic {
   public:
	static const int BOARD_SIZE;
   public:
	GameLogic(const std::string& fen);

	bool isLegalMove(int move) const;
	bool isKingInCheck() const;
	int getPiece(int box) const;
	bool getTurn() const;

	static int getColor(int piece);
	static int getPieceFromChar(char ch);
	static int getBoxID(int row, int column);
	static void getMovePositions(int move, int& start, int& target);

	void makeMove(int move);

   private:
	void loadFEN(const std::string& fen);
	std::string getFEN() const;

	void checkEnPassant(int move);

	bool isLegalPawnMove(int move) const;
	static bool isLegalKnightMove(int move) ;
	bool isLegalBishopMove(int move) const;
	bool isLegalRookMove(int move) const;
	bool isLegalQueenMove(int move) const;
	bool isLegalKingMove(int move) const;

	bool isKingInCheck(bool turn) const;

   private:
	int mBoard[64];
	bool mTurn;
	int mCastling;
	int mEnPassant;
	int mHalfMove;
	int mFullMove;
};

#endif  //CHESS_GAMELOGIC_HPP
