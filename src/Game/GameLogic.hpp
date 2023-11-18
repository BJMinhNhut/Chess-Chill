//
// Created by MINH NHUT on 10/25/2023.
//

#ifndef CHESS_GAMELOGIC_HPP
#define CHESS_GAMELOGIC_HPP

#include "Board.hpp"

#include <SFML/System/Time.hpp>

#include <string>
#include <vector>

class GameLogic {
   public:
	static const int BOARD_SIZE;
	enum Status {
		OnGoing,
		Checkmate,
		Stalemate,
		InsufficientMaterial,
		ThreefoldRepetition,
		Resign,
		Timeout,
	};

	enum MoveStatus {
		Normal,
		Capture,
		Castling,
		Promotion,
	};

   public:
	GameLogic();
	GameLogic(const GameLogic &other);

	[[nodiscard]] bool isLegalMove(int from, int to) const;
	[[nodiscard]] bool isKingInCheck() const;
	[[nodiscard]] bool getTurn() const;
	[[nodiscard]] bool isFinished() const;
	[[nodiscard]] Status status() const;
	[[nodiscard]] std::string getWinner() const;

	void makeMove(int from, int to);

	[[nodiscard]] float getRemainingTime(bool turn) const;

   protected:
	virtual void addPiece(int piece, int square);
	virtual void capturePiece(int square);
	virtual void movePiece(int from, int to);
	virtual void postMove();
	virtual void promotePiece(int square, int piece);

	virtual void update(sf::Time dt);

	void loadFEN(const std::string& fen);
	[[nodiscard]] std::string getFEN(bool withMove = true) const;

	[[nodiscard]] MoveStatus lastMoveStatus() const;
	[[nodiscard]] bool isAttacked(int square) const;
	[[nodiscard]] bool isAttacked(int square, bool turn) const;
	[[nodiscard]] bool isKingInCheck(bool turn) const;

   private:
	void move(int from, int to);
	void updateEnPassant(int from, int to);
	void updateHalfMove(int from, int to);
	void updateCastling(int from);
	void updateAttacks(bool turn);
	void updateStatus();

	[[nodiscard]] bool isInsufficientMaterial();
	[[nodiscard]] bool isThreefoldRepetition();

	[[nodiscard]] bool hasLegalMove() const;
	[[nodiscard]] bool isLegalPawnMove(int from, int to) const;
	[[nodiscard]] bool isEnPassant(int from, int to) const;
	static bool isLegalKnightMove(int from, int to);
	[[nodiscard]] bool isLegalBishopMove(int from, int to) const;
	[[nodiscard]] bool isLegalRookMove(int from, int to) const;
	[[nodiscard]] bool isLegalQueenMove(int from, int to) const;
	[[nodiscard]] bool isLegalKingMove(int from, int to) const;
	[[nodiscard]] bool isLegalCastling(int from, int to) const;

   private:
	Board mBoard;
	int64_t mAttackBoard[2];
	bool mTurn;
	int mCastling;
	int mEnPassant;
	int mHalfMove;
	int mFullMove;

	Status mStatus;
	MoveStatus mLastMove;

	std::vector<std::string> mHistory;

	bool mHasClock;
	sf::Time mTime[2];
};

#endif  //CHESS_GAMELOGIC_HPP
