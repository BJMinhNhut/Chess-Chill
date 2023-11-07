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
	GameLogic();

	bool isLegalMove(int from, int to) const;
	bool isKingInCheck() const;
	int getPiece(int box) const;
	bool getTurn() const;

	static int getColor(int piece);
	static int getType(int piece);
	static int getPieceFromChar(char ch);
	static int getBoxID(int row, int column);
	static bool validSquare(int square);

	void makeMove(int from, int to);

   protected:
	virtual void addPiece(int piece, int square);
	virtual void capturePiece(int square);
	virtual void movePiece(int from, int to, bool captured);

	void loadFEN(const std::string& fen);
	std::string getFEN() const;

   private:

	void updateEnPassant(int from, int to);

	bool isLegalPawnMove(int from, int to) const;
	bool isEnPassant(int from, int to) const;
	static bool isLegalKnightMove(int from, int to) ;
	bool isLegalBishopMove(int from, int to) const;
	bool isLegalRookMove(int from, int to) const;
	bool isLegalQueenMove(int from, int to) const;
	bool isLegalKingMove(int from, int to) const;

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
